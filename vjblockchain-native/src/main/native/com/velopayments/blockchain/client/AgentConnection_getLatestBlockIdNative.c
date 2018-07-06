/**
 * \file AgentConnection_getLatestBlockIdNative.c
 *
 * Get the latest block's UUID.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/client/AgentConnection.h>
#include <com/velopayments/blockchain/client/AgentConnectionPrivate.h>
#include <com/velopayments/blockchain/init/init.h>
#include <java/lang/IllegalStateException.h>
#include <java/util/Optional.h>
#include <java/util/UUID.h>
#include <string.h>
#include <util/uuid_conv.h>
#include <vpr/parameters.h>

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    getLatestBlockIdNative
 * Signature: ()Ljava/util/UUID;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_getLatestBlockIdNative(
    JNIEnv* env, jobject that)
{
    jobject retval = NULL;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
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

    /* query the database. */
    MDB_val key; key.mv_data = AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK;
    key.mv_size = sizeof(AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK);
    MDB_val val; memset(&val, 0, sizeof(val));
    int dbstat = mdb_get(txn, details->master_db, &key, &val);
    if (MDB_NOTFOUND == dbstat)
    {
        retval =
            (*env)->NewObject(
                env, UUID, UUID_init, 0, 0);
        goto cleanup_txn;
    }
    else if (0 != dbstat)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not query block database.");
        goto cleanup_txn;
    }

    block_record_t* rec = (block_record_t*)val.mv_data;
    retval = uuidFromBytes(env, rec->block_uuid);

cleanup_txn:
    mdb_txn_abort(txn);

exit_return:
    return retval;
}
