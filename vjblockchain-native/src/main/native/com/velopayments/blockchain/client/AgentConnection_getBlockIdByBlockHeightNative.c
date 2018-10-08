/**
 * \file AgentConnection_getBlockIdByBlockHeightNative.c
 *
 * Get the Block ID associated with a given block height.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/cert/Certificate.h>
#include <com/velopayments/blockchain/client/AgentConnection.h>
#include <com/velopayments/blockchain/client/AgentConnectionPrivate.h>
#include <com/velopayments/blockchain/init/init.h>
#include <java/lang/IllegalArgumentException.h>
#include <java/lang/IllegalStateException.h>
#include <java/util/Optional.h>
#include <string.h>
#include <util/uuid_conv.h>
#include <vccert/certificate_types.h>
#include <vpr/parameters.h>

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    getBlockIdByBlockHeightNative
 * Signature: (J)Ljava/util/Optional;
 */
JNIEXPORT jobject JNICALL Java_com_velopayments_blockchain_client_AgentConnection_getBlockIdByBlockHeightNative(
    JNIEnv* env, jobject that, jlong height)
{
    jobject retval = NULL;

    /* function contract enforcement. */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(height >= 0);

    /* verify the block height. */
    if (height < 0)
    {
        (*env)->ThrowNew(env, IllegalArgumentException,
                         "Height must be greater than or equal to zero.");
        goto exit_return;
    }

    /* get the block height as a 64-bit unsigned value. */
    uint64_t block_height = (uint64_t)height;

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
        goto exit_return;
    }

    /* if the block height is zero, then return the root block ID. */
    if (0 == block_height)
    {
        jobject blockId =
            uuidFromBytes(env, vccert_certificate_type_uuid_root_block);
        if (NULL == blockId)
        {
            (*env)->ThrowNew(env, IllegalStateException,
                             "Could not create block id.");
            goto exit_return;
        }

        retval =
            (*env)->CallStaticObjectMethod(env, Optional, Optional_of, blockId);

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
    MDB_val key; key.mv_size = 8; key.mv_data = &block_height;
    MDB_val val; memset(&val, 0, sizeof(val));
    int dbstat = mdb_get(txn, details->block_height_db, &key, &val);
    if (MDB_NOTFOUND == dbstat)
    {
        retval = (*env)->CallStaticObjectMethod(env, Optional, Optional_empty);
        goto cleanup_txn;
    }
    else if (0 != dbstat)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not query transaction database.");
        goto cleanup_txn;
    }

    /* create a UUID from a given set of ID bytes. */
    jobject blockId = uuidFromBytes(env, (const uint8_t*)val.mv_data);
    if (NULL == blockId)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create block id.");
        goto cleanup_txn;
    }

    /* success. */
    retval =
        (*env)->CallStaticObjectMethod(env, Optional, Optional_of, blockId);

cleanup_txn:
    mdb_txn_abort(txn);

exit_return:
    return retval;
}
