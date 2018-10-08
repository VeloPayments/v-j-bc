/**
 * \file AgentConnection_makeBlockNative.c
 *
 * Make a block using the currently pending transactions.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <com/velopayments/blockchain/cert/Certificate.h>
#include <com/velopayments/blockchain/client/AgentConnection.h>
#include <com/velopayments/blockchain/client/AgentConnectionPrivate.h>
#include <com/velopayments/blockchain/client/TransactionStatus.h>
#include <com/velopayments/blockchain/client/TransactionSubmissionRequest.h>
#include <com/velopayments/blockchain/init/init.h>
#include <cbmc/model_assert.h>
#include <java/lang/IllegalStateException.h>
#include <java/util/LinkedList.h>
#include <java/util/UUID.h>
#include <string.h>
#include <time.h>
#include <util/uuid_conv.h>
#include <vccert/builder.h>
#include <vccert/certificate_types.h>
#include <vccert/fields.h>
#include <vccert/parser.h>
#include <vccrypt/buffer.h>
#include <vpr/parameters.h>

/* forward decls */
static int write_transaction_to_block(
    agent_connection_details_t* details, MDB_txn* txn, JNIEnv* env,
    vccert_parser_options_t* parser_options, vccert_builder_context_t* builder,
    const uint8_t* block_id, jobject entry);
static int get_or_create_previous_block(
    JNIEnv* env, agent_connection_details_t* details, MDB_txn* txn,
    const block_record_t** rec);
static bool dummy_txn_resolver(
    void* options, void* parser, const uint8_t* artifact_id,
    const uint8_t* txn_id, vccrypt_buffer_t* output_buffer, bool* trusted);
static int32_t dummy_artifact_state_resolver(
    void* options, void* parser, const uint8_t* artifact_id,
    vccrypt_buffer_t* txn_id);
static bool dummy_entity_key_resolver(
    void* options, void* parser, uint64_t height, const uint8_t* entity_id,
    vccrypt_buffer_t* pubenckey_buffer, vccrypt_buffer_t* pubsignkey_buffer);
static vccert_contract_fn_t dummy_contract_resolver(
    void* options, void* parser, const uint8_t* type_id,
    const uint8_t* artifact_id);
static int update_or_create_artifact_entry_for_transaction(
    agent_connection_details_t* details, MDB_txn* txn, JNIEnv* env,
    const uint8_t* artifact_id, const uint8_t* block_id,
    const uint8_t* txn_id_bytes);
static int update_previous_transaction_record(
    agent_connection_details_t* details, MDB_txn* txn, JNIEnv* env,
    const uint8_t* previous_txn_id_bytes, const uint8_t* next_txn_id_bytes);

/**
 * The zero UUID.
 */
const uint8_t zero_id[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0 };
/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    makeBlockNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_makeBlockNative(
    JNIEnv* env, jobject that)
{
    bool in_transaction = false;

    /* function contract enforcement. */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
        goto exit_return;
    }

    /* create the parser options structure for this parse. */
    vccert_parser_options_t parser_options;
    if (0 != vccert_parser_options_init(
                &parser_options, &alloc_opts, &crypto_suite,
                &dummy_txn_resolver, &dummy_artifact_state_resolver,
                &dummy_contract_resolver, &dummy_entity_key_resolver, NULL))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "parser_options could not be initialized.");
        goto exit_return;
    }

    /* get the handle for our connection details. */
    jlong handle =
        (*env)->GetLongField(env, that, AgentConnection_field_handle);

    /* transform this value into our connection details struct. */
    agent_connection_details_t* details = (agent_connection_details_t*)handle;
    if (NULL == details || NULL == details->env)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "AgentConnection handle invariant violated.");
        goto cleanup_parser_options;
    }

    /* create a transaction for inserting records into the database. */
    MDB_txn* txn;
    if (0 != mdb_txn_begin(details->env, NULL, 0, &txn))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create transaction "
                         "for updating blockchain.");
        goto cleanup_parser_options;
    }

    /* we are now in a transaction. */
    in_transaction = true;

    /* get the submission list object. */
    jobject sl =
        (*env)->GetObjectField(env, that, AgentConnection_field_submissionList);
    if (NULL == sl)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "AgentConnection submission list invariant violated.");
        goto abort_transaction;
    }

    /* first, we want to create a UUID for the new block. */
    jobject blockId =
        (*env)->CallStaticObjectMethod(
            env, UUID, UUID_randomUUID);
    if (NULL == blockId)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Unable to create UUIDs.");
        goto abort_transaction;
    }

    /* create a block id array from this uuid. */
    uint8_t block_id_bytes[16];
    uuidToBytes(env, blockId, block_id_bytes);

    /* compute the size of the block certificate. */
    jint sl_size = (*env)->CallIntMethod(env, sl, LinkedList_size);
    size_t block_size = 0;
    for (jint i = 0; i < sl_size; ++i)
    {
        jobject entry = (*env)->CallObjectMethod(env, sl, LinkedList_get, i);
        jobject cert = 
            (*env)->CallObjectMethod(
                env, entry, TransactionSubmissionRequest_getTxnData);
        jbyteArray array = (jbyteArray)
            (*env)->CallObjectMethod(
                env, cert, Certificate_toByteArray);
        jsize array_len =
            (*env)->GetArrayLength(env, array);

        block_size += FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + array_len;

        /* clean up references. */
        (*env)->DeleteLocalRef(env, entry);
        (*env)->DeleteLocalRef(env, array);
    }

    /* calculate size of remaining fields. */
    block_size +=
        /* certificate version. */
        FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + sizeof(uint32_t)
        /* transaction timestamp */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + sizeof(uint64_t)
        /* crypto suite. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + sizeof(uint16_t)
        /* certificate type. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + 16
        /* block id. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + 16
        /* previous block id. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + 16
        /* previous block hash. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE +
            crypto_suite.hash_opts.hash_size
        /* block height. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + sizeof(uint64_t)
        /* signer id. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + 16
        /* signature. */
      + FIELD_TYPE_SIZE + FIELD_SIZE_SIZE +
            crypto_suite.sign_opts.signature_size;

    /* create a builder context. */
    vccert_builder_context_t builder;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_init(&builder_opts, &builder, block_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error creating builder context.");
        goto abort_transaction;
    }

    /* get the previous block. */
    const block_record_t* prevblockrec;
    if (0 != get_or_create_previous_block(
                env, details, txn, &prevblockrec))
    {
        goto abort_transaction;
    }

    /* add certificate version. */
    uint32_t cert_version = 0x00010000;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_uint32(
                &builder, VCCERT_FIELD_TYPE_CERTIFICATE_VERSION,
                cert_version))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding certificate version to builder.");
        goto cleanup_builder;
    }

    /* get current time. */
    time_t timestamp = time(NULL);
    if (timestamp < 0)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error getting time for block.");
        goto cleanup_builder;
    }

    /* add time to the builder. */
    uint64_t timestamp64 = timestamp;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_uint64(
                &builder, VCCERT_FIELD_TYPE_CERTIFICATE_VALID_FROM,
                timestamp64))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding timestamp to builder.");
        goto cleanup_builder;
    }

    /* add crypto suite to the builder. */
    uint16_t crypto_suite = 0x0001;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_uint16(
                &builder, VCCERT_FIELD_TYPE_CERTIFICATE_CRYPTO_SUITE,
                crypto_suite))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding crypto suite to builder.");
        goto cleanup_builder;
    }

    /* add certificate type to builder. */
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_buffer(
                &builder, VCCERT_FIELD_TYPE_CERTIFICATE_TYPE,
                vccert_certificate_type_uuid_txn_block,
                sizeof(vccert_certificate_type_uuid_txn_block)))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding certificate type to builder.");
        goto cleanup_builder;
    }

    /* add block id to the builder. */
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_buffer(
                &builder, VCCERT_FIELD_TYPE_BLOCK_UUID,
                block_id_bytes, sizeof(block_id_bytes)))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding block id to builder.");
        goto cleanup_builder;
    }

    /* add previous block id to the builder. */
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_buffer(
                &builder, VCCERT_FIELD_TYPE_PREVIOUS_BLOCK_UUID,
                prevblockrec->block_uuid, sizeof(prevblockrec->block_uuid)))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding previous block id to builder.");
        goto cleanup_builder;
    }

    /* add previous block hash to the builder. */
    /* TODO - re-enable with proper hash. */
    #if 0
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_buffer(
                &builder, VCCERT_FIELD_TYPE_PREVIOUS_BLOCK_HASH,
                previous_block_hash, sizeof(previous_block_hash)))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding previous block hash to builder.");
        goto cleanup_builder;
    }
    #endif

    /* add block height to the builder. */
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_uint64(
                &builder, VCCERT_FIELD_TYPE_BLOCK_HEIGHT,
                prevblockrec->block_height + 1))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding block height to builder.");
        goto cleanup_builder;
    }

    /* iterate through all of the transactions, adding them to this block. */
    for (jint i = 0; i < sl_size; ++i)
    {
        jobject entry = (*env)->CallObjectMethod(env, sl, LinkedList_get, i);

        if (0 != write_transaction_to_block(
                    details, txn, env, &parser_options, &builder,
                    block_id_bytes, entry))
        {
            goto cleanup_builder;
        }

        /* clean up references. */
        (*env)->DeleteLocalRef(env, entry);
    }

    /* TODO - sign block here. */

    /* TODO - hack to just get the block bytes. Replace with sign above. */
    size_t block_cert_size;
    const uint8_t* block_cert_bytes =
        vccert_builder_emit(&builder, &block_cert_size);
    if (NULL == block_cert_bytes)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not emit block.");
        goto abort_transaction;
    }

    /* create block record. */
    size_t blockrec_size = sizeof(block_record_t) + block_cert_size;
    block_record_t* blockrec = (block_record_t*)
        allocate(&alloc_opts, blockrec_size);
    if (NULL == blockrec)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create block record.");
        goto abort_transaction;
    }

    /* populate block record. */
    memset(blockrec, 0, sizeof(block_record_t));
    memcpy(blockrec->block_uuid, block_id_bytes, 16);
    memcpy(blockrec->previous_block_id, prevblockrec->block_uuid, 16);
    memset(blockrec->next_block_id, 0, sizeof(blockrec->next_block_id));
    blockrec->block_height = prevblockrec->block_height + 1;
    blockrec->block_size = block_cert_size;
    memcpy(((uint8_t*)blockrec) + sizeof(block_record_t), block_cert_bytes,
           block_cert_size);

    /* insert block. */
    MDB_val key; key.mv_size = 16; key.mv_data = block_id_bytes;
    MDB_val val; val.mv_size = blockrec_size; val.mv_data = blockrec;
    if (0 != mdb_put(txn, details->block_db, &key, &val, MDB_NOOVERWRITE))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not insert block.");
        goto cleanup_blockrec;
    }

    /* update previous block with next block id. */
    block_record_t* prev = (block_record_t*)
        allocate(
            &alloc_opts, prevblockrec->block_size + sizeof(block_record_t));
    if (NULL == prev)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not allocate space for previous "
                                        "block update.");
        goto cleanup_blockrec;
    }

    /* update previous block. */
    memcpy(prev, prevblockrec,
           sizeof(block_record_t) + prevblockrec->block_size);
    memcpy(prev->next_block_id, block_id_bytes, 16);
    key.mv_size = 16; key.mv_data = prev->block_uuid;
    val.mv_size = sizeof(block_record_t) + prev->block_size;
    val.mv_data = prev;
    if (0 != mdb_put(txn, details->block_db, &key, &val, 0))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not update previous block.");
        goto cleanup_prev;
    }

    /* update latest block id with this block. */
    key.mv_data = AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK;
    key.mv_size = sizeof(AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK);
    val.mv_size = blockrec_size; val.mv_data = blockrec;
    if (0 != mdb_put(txn, details->master_db, &key, &val, 0))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not update latest block.");
        goto cleanup_prev;
    }

    /* update the block height index with the block height for this block. */
    key.mv_size = sizeof(uint64_t); key.mv_data = &blockrec->block_height;
    val.mv_size = sizeof(blockrec->block_uuid);
    val.mv_data = blockrec->block_uuid;
    if (0 != mdb_put(txn, details->block_height_db, &key, &val, 0))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not update block height idx.");
        goto cleanup_prev;
    }

    /* get the success status code field. */
    jobject status_success =
        (*env)->GetStaticObjectField(
            env, TransactionStatus, TransactionStatus_field_SUCCEEDED);
    if (NULL == status_success)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not get success status code.");
        goto cleanup_prev;
    }

    /* commit. */
    if (0 != mdb_txn_commit(txn))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not commit transactions.");
        goto cleanup_prev;
    }

    /* we are no longer in a transaction. */
    in_transaction = false;

    /* update all transactions in the submission list. */
    for (jint i = 0; i < sl_size; ++i)
    {
        jobject entry = (*env)->CallObjectMethod(env, sl, LinkedList_get, i);

        (*env)->CallVoidMethod(env, entry,
                               TransactionSubmissionRequest_setStatus,
                               status_success);

        /* clean up references. */
        (*env)->DeleteLocalRef(env, entry);
    }

    /* clear the submission list. */
    (*env)->CallVoidMethod(env, sl, LinkedList_clear);

cleanup_prev:
    release(&alloc_opts, prev);

cleanup_blockrec:
    release(&alloc_opts, blockrec);

cleanup_builder:
    dispose((disposable_t*)&builder);

abort_transaction:
    if (in_transaction)
    {
        mdb_txn_abort(txn);
        in_transaction = false;
    }

cleanup_parser_options:
    dispose((disposable_t*)&parser_options);

exit_return:
    return;
}

/**
 * \brief Write the given transaction to the block.
 *
 * \param details       The connection details for this block.
 * \param txn           The database transaction.
 * \param env           The JNI Environment.
 * \param builder       The certificate builder for this block.
 * \param block_id      The ID of this block.
 * \param entry         The submission entry for this transaction.
 *
 * \returns 0 on success and non-zero on failure.
 */
static int write_transaction_to_block(
    agent_connection_details_t* details, MDB_txn* txn, JNIEnv* env,
    vccert_parser_options_t* parser_options, vccert_builder_context_t* builder,
    const uint8_t* block_id, jobject entry)
{
    int retval = 0;
    jobject cert = 
        (*env)->CallObjectMethod(
            env, entry, TransactionSubmissionRequest_getTxnData);
    jbyteArray array = (jbyteArray)
        (*env)->CallObjectMethod(
            env, cert, Certificate_toByteArray);
    jsize array_len =
        (*env)->GetArrayLength(env, array);

    /* get the bytes for this array. */
    uint8_t* array_bytes =
        (uint8_t*)(*env)->GetByteArrayElements(env, array, NULL);

    /* add the transaction to the block. */
    if (VCCERT_STATUS_SUCCESS !=
            vccert_builder_add_short_buffer(
                    builder, VCCERT_FIELD_TYPE_WRAPPED_TRANSACTION_TUPLE,
                    array_bytes, array_len))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error adding transaction to builder.");
        retval = -1;
        goto cleanup_array_bytes;
    }

    /* create entry for transaction record. */
    size_t txnrec_size = sizeof(transaction_record_t) + array_len;
    transaction_record_t* txnrec = (transaction_record_t*)
        allocate(&alloc_opts, txnrec_size);
    if (NULL == txnrec)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not allocate memory for txnrec.");
        retval = -2;
        goto cleanup_array_bytes;
    }

    /* Parse the transaction. */
    vccert_parser_context_t parser;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_parser_init(
                parser_options, &parser, array_bytes, array_len))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create transaction parser.");
        retval = -3;
        goto cleanup_txnrec;
    }

    /* Get transaction id for transaction. */
    const uint8_t* txn_id_bytes;
    size_t txn_id_bytes_size = 16;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_parser_find_short(
                &parser, VCCERT_FIELD_TYPE_CERTIFICATE_ID,
                &txn_id_bytes, &txn_id_bytes_size)
        || txn_id_bytes_size != 16)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not find a valid transaction id.");
        retval = -4;
        goto cleanup_parser;
    }

    /* Get the artifact id for transaction. */
    const uint8_t* artifact_id_bytes;
    size_t artifact_id_bytes_size = 16;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_parser_find_short(
                &parser, VCCERT_FIELD_TYPE_ARTIFACT_ID,
                &artifact_id_bytes, &artifact_id_bytes_size)
        || artifact_id_bytes_size != 16)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not find a valid artifact id.");
        retval = -5;
        goto cleanup_parser;
    }

    /* Get the previous transaction UUID. */
    const uint8_t* previous_txn_id_bytes;
    size_t previous_txn_id_bytes_size = 16;
    if (VCCERT_STATUS_SUCCESS !=
            vccert_parser_find_short(
                &parser, VCCERT_FIELD_TYPE_PREVIOUS_CERTIFICATE_ID,
                &previous_txn_id_bytes, &previous_txn_id_bytes_size)
        || previous_txn_id_bytes_size != 16)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not find a valid previous txn id.");
        retval = -6;
        goto cleanup_parser;
    }
                

    /* Verify that transaction id does not already exist. */
    MDB_val key; key.mv_size = 16; key.mv_data = (uint8_t*)txn_id_bytes;
    MDB_val val;
    if (MDB_NOTFOUND != mdb_get(txn, details->txn_db, &key, &val))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Duplicate transaction id.");
        retval = -7;
        goto cleanup_parser;
    }

    /* populate transaction record. */
    memset(txnrec, 0, sizeof(transaction_record_t));
    memcpy(txnrec->transaction_uuid, txn_id_bytes, 16);
    memcpy(txnrec->block_uuid, block_id, 16);
    memcpy(txnrec->prev_transaction_uuid, previous_txn_id_bytes, 16);
    txnrec->transaction_size = array_len;
    memcpy(((uint8_t*)txnrec) + sizeof(transaction_record_t), array_bytes,
           array_len);

    /* insert the transaction to the database. */
    key.mv_size = 16; key.mv_data = (uint8_t*)txn_id_bytes;
    val.mv_size = txnrec_size; val.mv_data = txnrec;
    if (0 != mdb_put(txn, details->txn_db, &key, &val, MDB_NOOVERWRITE))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not insert transaction.");
        retval = -8;
        goto cleanup_parser;
    }

    /* update or create the artifact entry for this block. */
    if (0 != update_or_create_artifact_entry_for_transaction(
                details, txn, env, artifact_id_bytes, block_id, txn_id_bytes))
    {
        retval = -9;
        goto cleanup_parser;
    }

    /* update the previous transaction id's next value with this txn id. */
    if (0 != update_previous_transaction_record(
                details, txn, env, previous_txn_id_bytes, txn_id_bytes))
    {
        retval = -10;
        goto cleanup_parser;
    }

cleanup_parser:
        dispose((disposable_t*)&parser);

cleanup_txnrec:
        release(&alloc_opts, txnrec);

cleanup_array_bytes:
        (*env)->ReleaseByteArrayElements(env, array, (jbyte*)array_bytes, 0);

        /* clean up array. */
        (*env)->DeleteLocalRef(env, array);

        return retval;
}

/**
 * \brief Get or create the previous block.
 *
 * The previous block is stored in the master datastore under the key,
 * AGENT_CONNECTION_MASTER_KEY_PREVIOUS_DATABASE.  If this value is not found in
 * the data store, then the root block is created and inserted.
 *
 * \param details       The connection details.
 * \param txn           The database transaction under which queries to the
 *                      master database occur.
 * \param rec           The block record for this entry.
 */
static int get_or_create_previous_block(
    JNIEnv* env, agent_connection_details_t* details, MDB_txn* txn,
    const block_record_t** rec)
{
    int retval = 0;

    MDB_val key, val;
    key.mv_size = sizeof(AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK);
    key.mv_data = AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK;
    retval = mdb_get(txn, details->master_db, &key, &val);
    if (0 == retval)
    {
        *rec = (const block_record_t*)val.mv_data;
        goto exit_return;
    }
    else if(MDB_NOTFOUND != retval)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error reading latest block.");
        goto exit_return;
    }

    /* TODO - create proper root block instead of this hack. */

    /* stick this hack block in the master db. */
    block_record_t* blockrec =
        allocate(&alloc_opts, sizeof(block_record_t) + 256);
    if (NULL == blockrec)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error allocating block.");
        retval = 1;
        goto exit_return;
    }

    memset(blockrec, 0, sizeof(block_record_t) + 256);
    memcpy(blockrec->block_uuid, vccert_certificate_type_uuid_root_block,
           sizeof(blockrec->block_uuid));
    blockrec->block_size = 256;
    key.mv_size = sizeof(AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK);
    key.mv_data = AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK;
    val.mv_size = sizeof(block_record_t) + 256;
    val.mv_data = blockrec;
    retval = mdb_put(txn, details->master_db, &key, &val, MDB_NOOVERWRITE);
    if (0 != retval)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error inserting root block into the master db.");
        goto cleanup_blockrec;
    }

    /* stick this hack block in the block db. */
    key.mv_size = 16;
    key.mv_data = blockrec->block_uuid;
    val.mv_size = sizeof(block_record_t) + 256;
    val.mv_data = blockrec;
    retval = mdb_put(txn, details->block_db, &key, &val, MDB_NOOVERWRITE);
    if (0 != retval)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error inserting root block into block db.");
        goto cleanup_blockrec;
    }

    /* grab the data from the database. */
    key.mv_size = sizeof(AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK);
    key.mv_data = AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK;
    retval = mdb_get(txn, details->master_db, &key, &val);
    if (0 == retval)
    {
        *rec = (const block_record_t*)val.mv_data;
        goto cleanup_blockrec;
    }
    else
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Error reading latest block.");
        goto cleanup_blockrec;
    }

cleanup_blockrec:
    release(&alloc_opts, blockrec);

exit_return:
    return retval;
}

/**
 * Dummy transaction resolver.
 */
static bool dummy_txn_resolver(
                    void* UNUSED(options), void* UNUSED(parser),
                    const uint8_t* UNUSED(artifact_id),
                    const uint8_t* UNUSED(txn_id),
                    vccrypt_buffer_t* UNUSED(output_buffer),
                    bool* UNUSED(trusted))
{
    return false;
}

/**
 * Dummy artifact state resolver.
 */
static int32_t dummy_artifact_state_resolver(
                    void* UNUSED(options), void* UNUSED(parser),
                    const uint8_t* UNUSED(artifact_id),
                    vccrypt_buffer_t* UNUSED(txn_id))
{
    return -1;
}

/**
 * Dummy entity key resolver.
 */
static bool dummy_entity_key_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                uint64_t UNUSED(height), const uint8_t* UNUSED(entity_id),
                vccrypt_buffer_t* UNUSED(pubenckey_buffer),
                vccrypt_buffer_t* UNUSED(pubsignkey_buffer))
{
    return false;
}

/**
 * Dummy contract resolver.
 */
static vccert_contract_fn_t dummy_contract_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                const uint8_t* UNUSED(type_id),
                const uint8_t* UNUSED(artifact_id))
{
    return NULL;
}

/**
 * Update or create the artifact entry for this transaction.
 */
static int update_or_create_artifact_entry_for_transaction(
    agent_connection_details_t* details, MDB_txn* txn, JNIEnv* env,
    const uint8_t* artifact_id, const uint8_t* block_id,
    const uint8_t* txn_id_bytes)
{
    int retval = 0;

    /* query the artifact database for this artifact ID. */
    MDB_val key; key.mv_size = 16; key.mv_data = (void*)artifact_id;
    MDB_val val; memset(&val, 0, sizeof(val));
    retval = mdb_get(txn, details->artifact_db, &key, &val);
    if (MDB_NOTFOUND == retval)
    {
        /* if not found, create the entry and insert it. */
        artifact_record_t artifact_rec;
        memset(&artifact_rec, 0, sizeof(artifact_rec));
        memcpy(artifact_rec.artifact_uuid, artifact_id, 16);
        memcpy(artifact_rec.first_block_uuid, block_id, 16);
        memcpy(artifact_rec.first_transaction_uuid, txn_id_bytes, 16);
        memcpy(artifact_rec.last_block_uuid, block_id, 16);
        memcpy(artifact_rec.last_transaction_uuid, txn_id_bytes, 16);

        /* insert this record. */
        key.mv_size = 16; key.mv_data = (void*)artifact_id;
        val.mv_size = sizeof(artifact_record_t); val.mv_data = &artifact_rec;
        retval = mdb_put(txn, details->artifact_db, &key, &val, 0);
    }
    else if (0 == retval)
    {
        /* if found, update it with this transaction id and block id. */
        artifact_record_t* artifact_rec = (artifact_record_t*)val.mv_data;
        artifact_record_t newrec;
        memcpy(&newrec, artifact_rec, sizeof(artifact_record_t));
        memcpy(newrec.last_block_uuid, block_id, 16);
        memcpy(newrec.last_transaction_uuid, txn_id_bytes, 16);
        key.mv_size = 16; key.mv_data = (void*)artifact_id;
        val.mv_size = sizeof(artifact_record_t); val.mv_data = &newrec;
        retval = mdb_put(txn, details->artifact_db, &key, &val, 0);
    }

    /* if the get / insert failed, bubble up an error to the caller. */
    if (0 != retval)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not update artifact data.");

        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Update the previous transaction record with the next transaction uuid.
 */
static int update_previous_transaction_record(
    agent_connection_details_t* details, MDB_txn* txn, JNIEnv* env,
    const uint8_t* previous_txn_id_bytes, const uint8_t* next_txn_id_bytes)
{
    int retval = 0;

    /* if this transaction value is all zeroes, then there is no previous
     * transaction ID. */
    if (!memcmp(zero_id, previous_txn_id_bytes, 16))
    {
        /* nothing to do, so success. */
        retval = 0;
        goto exit_return;
    }

    /* query the transaction database for the previous transaction ID. */
    MDB_val key; key.mv_size = 16; key.mv_data = (void*)previous_txn_id_bytes;
    MDB_val val; memset(&val, 0, sizeof(val));
    retval = mdb_get(txn, details->txn_db, &key, &val);
    if (0 != retval)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not query previous txn.");
        retval = 1;
        goto exit_return;
    }

    /* get the transaction record. */
    transaction_record_t* txn_rec = (transaction_record_t*)val.mv_data;

    /* allocate replacement buffer for this transaction. */
    size_t new_rec_size =
        sizeof(transaction_record_t) + txn_rec->transaction_size;
    transaction_record_t* new_rec = (transaction_record_t*)
        allocate(&alloc_opts, new_rec_size);

    /* copy the original record. */
    memcpy(new_rec, txn_rec, new_rec_size);

    /* update this new record with the next transaction id. */
    memcpy(new_rec->next_transaction_uuid, next_txn_id_bytes, 16);

    /* write this new record to the database. */
    key.mv_size = 16; key.mv_data = (void*)previous_txn_id_bytes;
    val.mv_size = new_rec_size; val.mv_data = new_rec;
    retval = mdb_put(txn, details->txn_db, &key, &val, 0);
    if (0 != retval)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Could not update previous txn.");
        goto cleanup_new_rec;
    }

    /* success. */
    retval = 0;

cleanup_new_rec:
    release(&alloc_opts, new_rec);

exit_return:
    return 0;
}
