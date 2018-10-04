/**
 * \file AgentConnection_connectNative.c
 *
 * Connect to a local database.
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
 * Method:    connectNative
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_connectNative(
    JNIEnv* env, jobject UNUSED(that), jstring connect)
{
    long retval = 0;
    MDB_txn* txn;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(MODEL_PROP_VALID_VPR_ALLOCATOR(&alloc_opts));
    MODEL_ASSERT(NULL != connect);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vjblockchain not initialized.");
        retval = 0;
        goto exit_return;
    }

    /* create the agent connection details structure. */
    agent_connection_details_t* details = (agent_connection_details_t*)
        allocate(&alloc_opts, sizeof(agent_connection_details_t));
    if (NULL == details)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not allocate memory for agent_connection.");
        retval = 0;
        goto exit_return;
    }

    /* Initialize this structure. */
    memset(details, 0, sizeof(agent_connection_details_t));

    /* Create the lmdb environment. */
    if (0 != mdb_env_create(&details->env))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create database environment.");
        retval = 0;
        goto free_connection;
    }

    /* TODO - the following mapsize value should be pulled from the environment
     * with a default, and not hard-coded.
     */

    /* set the database size to an arbitrarily large value. */
    if (0 != mdb_env_set_mapsize(details->env, 8UL*1024UL*1024UL*1024UL))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not update the mapsize for the database.");
        retval = 0;
        goto free_environment;
    }

    /* We need 5 database handles. */
    if (0 != mdb_env_set_maxdbs(details->env, 5))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not update the maximum database handles.");
        retval = 0;
        goto free_environment;
    }

    /* get the connection name. */
    const char* connectionStringBytes =
        (*env)->GetStringUTFChars(env, connect, NULL);
    if (NULL == connectionStringBytes)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not get the connection string bytes.");
        retval = 0;
        goto free_environment;
    }

    /* open the environment. */
    if (0 != mdb_env_open(details->env, connectionStringBytes, 0, 0600))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not get the connection string bytes.");
        retval = 0;
        (*env)->ReleaseStringUTFChars(env, connect, connectionStringBytes);
        goto free_environment;
    }

    /* clean up string bytes. */
    (*env)->ReleaseStringUTFChars(env, connect, connectionStringBytes);

    /* create a transaction for opening the databases. */
    if (0 != mdb_txn_begin(details->env, NULL, 0, &txn))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not create transaction for opening databases.");
        retval = 0;
        goto free_environment;
    }

    /* open the master database. */
    if (0 != mdb_dbi_open(txn, "master.db", MDB_CREATE, &details->master_db))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not open master database.");
        retval = 0;
        goto rollback_txn;
    }

    /* open the artifact database. */
    if (0 !=
            mdb_dbi_open(txn, "artifact.db", MDB_CREATE, &details->artifact_db))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not open artifact database.");
        retval = 0;
        goto rollback_txn;
    }

    /* open the block database. */
    if (0 != mdb_dbi_open(txn, "block.db", MDB_CREATE, &details->block_db))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not open block database.");
        retval = 0;
        goto rollback_txn;
    }

    /* open the transaction database. */
    if (0 != mdb_dbi_open(txn, "txn.db", MDB_CREATE, &details->txn_db))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not open transaction database.");
        retval = 0;
        goto rollback_txn;
    }

    /* open the block height database. */
    if (0 !=
            mdb_dbi_open(
                txn, "bheight.db", MDB_CREATE, &details->block_height_db))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not open block height database.");
        retval = 0;
        goto rollback_txn;
    }

    /* commit the open. */
    if (0 != mdb_txn_commit(txn))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "Could not commit database open transaction.");
        retval = 0;
        goto rollback_txn;
    }

    /* success */
    retval = (jlong)details;
    goto exit_return;

rollback_txn:
    mdb_txn_abort(txn);

free_environment:
    mdb_env_close(details->env);

free_connection:
    release(&alloc_opts, details);

exit_return:
    return retval;
}
