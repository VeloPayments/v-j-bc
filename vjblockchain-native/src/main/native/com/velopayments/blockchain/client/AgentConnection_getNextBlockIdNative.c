/**
 * \file AgentConnection_getNextBlockIdNative.c
 *
 * Given a block UUID, return the next block UUID in the blockchain.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/client/AgentConnection.h>
#include <com/velopayments/blockchain/client/AgentConnectionPrivate.h>
#include <com/velopayments/blockchain/init/init.h>
#include <java/lang/IllegalStateException.h>
#include <java/lang/NullPointerException.h>
#include <java/util/Optional.h>
#include <java/util/UUID.h>
#include <string.h>
#include <util/uuid_conv.h>
#include <vpr/parameters.h>

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    getNextBlockIdNative
 * Signature: (Ljava/util/UUID;)Ljava/util/Optional;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_getNextBlockIdNative(
    JNIEnv* env, jobject that, jobject blockId)
{
    jobject retval = NULL;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != blockId);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
        goto exit_return;
    }

    /* verify that the blockId parameter is not null. */
    if (NULL == blockId)
    {
        (*env)->ThrowNew(env, NullPointerException, "blockId");
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
        goto exit_return;
    }

    /* create a transaction for querying the database. */
    MDB_txn* txn;
    if (0 != mdb_txn_begin(details->env, NULL, MDB_RDONLY, &txn))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create transaction "
                         "for querying blockchain.");
        goto exit_return;
    }

    /* convert the blockId into a C array. */
    uint8_t block_id_bytes[16];
    uuidToBytes(env, blockId, block_id_bytes);

    /* query the database. */
    MDB_val key; key.mv_size = 16; key.mv_data = block_id_bytes;
    MDB_val val; memset(&val, 0, sizeof(val));
    int dbstat = mdb_get(txn, details->block_db, &key, &val);
    if (MDB_NOTFOUND == dbstat)
    {
        retval =
            (*env)->CallStaticObjectMethod(env, Optional, Optional_empty);
        goto cleanup_txn;
    }
    else if (0 != dbstat)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not query block database.");
        goto cleanup_txn;
    }

    /* convert to a block record. */
    block_record_t* rec = (block_record_t*)val.mv_data;

    /* is this value zero? */
    uint8_t zero_uuid[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (!memcmp(rec->next_block_id, zero_uuid, 16))
    {
        retval =
            (*env)->CallStaticObjectMethod(env, Optional, Optional_empty);
        goto cleanup_txn;
    }

    /* If not, the UUID is valid and can be returned to the caller. */
    jobject uuid = uuidFromBytes(env, rec->next_block_id);
    retval = (*env)->CallStaticObjectMethod(env, Optional, Optional_of, uuid);

cleanup_txn:
    mdb_txn_abort(txn);

exit_return:
    return retval;
}
