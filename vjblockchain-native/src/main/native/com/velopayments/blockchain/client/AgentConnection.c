/**
 * \file AgentConnection.c
 *
 * Class and method exports for AgentConnection.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "AgentConnection.h"

jclass AgentConnection = NULL;
jmethodID AgentConnection_init = NULL;
jmethodID AgentConnection_close = NULL;
jmethodID AgentConnection_submit = NULL;
jmethodID AgentConnection_getLatestBlockId = NULL;
jmethodID AgentConnection_getBlockById = NULL;
jmethodID AgentConnection_getTransactionById = NULL;
jfieldID AgentConnection_field_submissionList = NULL;
jfieldID AgentConnection_field_handle = NULL;

static volatile bool AgentConnection_registered = false;

/**
 * Property: AgentConnection globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != AgentConnection \
        NULL != AgentConnection_init \
        NULL != AgentConnection_close \
        NULL != AgentConnection_submit \
        NULL != AgentConnection_getLatestBlockId \
        NULL != AgentConnection_getBlockById \
        NULL != AgentConnection_getTransactionById \
        NULL != AgentConnection_field_submissionList \
        NULL != AgentConnection_field_handle)

/**
 * Register the following AgentConnection references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int AgentConnection_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register AgentConnection once. */
    if (AgentConnection_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register AgentConnection class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/client/AgentConnection");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to AgentConnection. */
    AgentConnection = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == AgentConnection)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method. */
    AgentConnection_init =
        (*env)->GetMethodID(
            env, AgentConnection, "<init>",
            "(Ljava/lang/String;Ljava/util/UUID;"
                "Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;)V");
    if (NULL == AgentConnection_init)
        return 1;

    /* register close method. */
    AgentConnection_close =
        (*env)->GetMethodID(
            env, AgentConnection, "close", "()V");
    if (NULL == AgentConnection_close)
        return 1;

    /* register submit method. */
    AgentConnection_submit =
        (*env)->GetMethodID(
            env, AgentConnection, "submit",
            "([B)Ljava/util/concurrent/Future;");
    if (NULL == AgentConnection_submit)
        return 1;

    /* register getLatestBlockId method. */
    AgentConnection_getLatestBlockId =
        (*env)->GetMethodID(
            env, AgentConnection, "getLatestBlockId",
            "()Ljava/util/UUID;");
    if (NULL == AgentConnection_getLatestBlockId)
        return 1;

    /* register getBlockById method. */
    AgentConnection_getBlockById =
        (*env)->GetMethodID(
            env, AgentConnection, "getBlockById",
            "(Ljava/util/UUID;)Ljava/util/Optional;");
    if (NULL == AgentConnection_getBlockById)
        return 1;

    /* register getTransactionById method. */
    AgentConnection_getTransactionById =
        (*env)->GetMethodID(
            env, AgentConnection, "getTransactionById",
            "(Ljava/util/UUID;)Ljava/util/Optional;");
    if (NULL == AgentConnection_getTransactionById)
        return 1;

    /* register submissionList field. */
    AgentConnection_field_submissionList =
        (*env)->GetFieldID(
            env, AgentConnection, "submissionList", "Ljava/util/List;");
    if (NULL == AgentConnection_field_submissionList)
        return 1;

    /* register handle field. */
    AgentConnection_field_handle =
        (*env)->GetFieldID(
            env, AgentConnection, "handle", "J");
    if (NULL == AgentConnection_field_handle)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    AgentConnection_registered = true;
    return 0;
}
