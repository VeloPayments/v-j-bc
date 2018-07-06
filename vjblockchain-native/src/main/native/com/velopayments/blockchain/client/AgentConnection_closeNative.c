/**
 * \file AgentConnection_closeNative.c
 *
 * Close a local database connection.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/client/AgentConnection.h>
#include <com/velopayments/blockchain/client/AgentConnectionPrivate.h>
#include <com/velopayments/blockchain/init/init.h>
#include <java/lang/IllegalStateException.h>
#include <string.h>
#include <vpr/parameters.h>

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    closeNative
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_closeNative(
    JNIEnv* env, jobject UNUSED(that), jlong handle)
{
    /* function contract enforcement. */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(MODEL_PROP_VALID_VPR_ALLOCATOR(&alloc_opts));
    MODEL_ASSERT(0 != handle);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
    }

    /* get the agent connection details structure. */
    agent_connection_details_t* details = (agent_connection_details_t*)handle;
    if (NULL == details)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Attempt to close invalid handle.");
    }

    /* force sync the database. */
    mdb_env_sync(details->env, 1);

    /* close dbi handles. */
    mdb_dbi_close(details->env, details->master_db);
    mdb_dbi_close(details->env, details->txn_db);
    mdb_dbi_close(details->env, details->block_db);

    /* close database environment. */
    mdb_env_close(details->env);

    /* release details. */
    memset(details, 0, sizeof(agent_connection_details_t));
    release(&alloc_opts, details);
}
