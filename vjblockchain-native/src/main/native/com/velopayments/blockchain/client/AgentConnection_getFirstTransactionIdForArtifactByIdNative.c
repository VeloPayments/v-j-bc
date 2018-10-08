/**
 * \file AgentConnection_getFirstTransactionIdForArtifactByIdNative.c
 *
 * Get the first Transaction ID for the given Artifact ID.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/cert/Certificate.h>
#include <com/velopayments/blockchain/client/AgentConnection.h>
#include <com/velopayments/blockchain/client/AgentConnectionPrivate.h>
#include <com/velopayments/blockchain/init/init.h>
#include <java/lang/IllegalStateException.h>
#include <java/lang/NullPointerException.h>
#include <java/util/Optional.h>
#include <string.h>
#include <util/uuid_conv.h>
#include <vpr/parameters.h>

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    getFirstTransactionIdForArtifactByIdNative
 * Signature: (Ljava/util/UUID;)Ljava/util/Optional;
 */
JNIEXPORT jobject JNICALL Java_com_velopayments_blockchain_client_AgentConnection_getFirstTransactionIdForArtifactByIdNative(
    JNIEnv* env, jobject that, jobject artifactId)
{
    jobject retval = NULL;

    /* function contract enforcement. */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != artifactId);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
        goto exit_return;
    }

    /* verify that the artifactId parameter is not null. */
    if (NULL == artifactId)
    {
        (*env)->ThrowNew(env, NullPointerException, "artifactId");
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

    /* get the bytes for this artifact id. */
    uint8_t artifact_id_bytes[16];
    uuidToBytes(env, artifactId, artifact_id_bytes);

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
    MDB_val key; key.mv_size = 16; key.mv_data = artifact_id_bytes;
    MDB_val val; memset(&val, 0, sizeof(val));
    int dbstat = mdb_get(txn, details->artifact_db, &key, &val);
    if (MDB_NOTFOUND == dbstat)
    {
        retval = (*env)->CallStaticObjectMethod(env, Optional, Optional_empty);
        goto cleanup_txn;
    }
    else if (0 != dbstat)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not query artifact database.");
        goto cleanup_txn;
    }

    /* create a UUID from a given set of ID bytes. */
    artifact_record_t* rec = (artifact_record_t*)val.mv_data;
    jobject txnId = uuidFromBytes(env, rec->first_transaction_uuid);
    if (NULL == txnId)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could create transaction UUID.");
        goto cleanup_txn;
    }

    /* success. */
    retval = (*env)->CallStaticObjectMethod(env, Optional, Optional_of, txnId);

cleanup_txn:
    mdb_txn_abort(txn);

exit_return:
    return retval;
}
