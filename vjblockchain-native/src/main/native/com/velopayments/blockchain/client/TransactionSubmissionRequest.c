/**
 * \file TransactionSubmissionRequest.c
 *
 * Class and method exports for TransactionSubmissionRequest.  This source file
 * contains a registration method that registers the class and method references
 * just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "AgentConnection.h"

jclass TransactionSubmissionRequest = NULL;
jmethodID TransactionSubmissionRequest_init = NULL;
jmethodID TransactionSubmissionRequest_getStatus = NULL;
jmethodID TransactionSubmissionRequest_setStatus = NULL;
jmethodID TransactionSubmissionRequest_getStatusFuture = NULL;
jmethodID TransactionSubmissionRequest_getTxnData = NULL;

static volatile bool TransactionSubmissionRequest_registered = false;

/**
 * Property: TransactionSubmissionRequest globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != TransactionSubmissionRequest \
        NULL != TransactionSubmissionRequest_init \
        NULL != TransactionSubmissionRequest_getStatus \
        NULL != TransactionSubmissionRequest_setStatus \
        NULL != TransactionSubmissionRequest_getStatusFuture \
        NULL != TransactionSubmissionRequest_getTxnData)

/**
 * Register the following TransactionSubmissionRequest references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int TransactionSubmissionRequest_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register TransactionSubmissionRequest once. */
    if (TransactionSubmissionRequest_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register TransactionSubmissionRequest class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/client/TransactionSubmissionRequest");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to AgentConnection. */
    TransactionSubmissionRequest =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == TransactionSubmissionRequest)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method. */
    TransactionSubmissionRequest_init =
        (*env)->GetMethodID(
            env, TransactionSubmissionRequest, "<init>",
            "(Lcom/velopayments/blockchain/cert/Certificate;)V");
    if (NULL == TransactionSubmissionRequest_init)
        return 1;

    /* register getStatus method. */
    TransactionSubmissionRequest_getStatus =
        (*env)->GetMethodID(
            env, TransactionSubmissionRequest, "getStatus",
            "()Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == TransactionSubmissionRequest_getStatus)
        return 1;

    /* register setStatus method. */
    TransactionSubmissionRequest_setStatus =
        (*env)->GetMethodID(
            env, TransactionSubmissionRequest, "setStatus",
            "(Lcom/velopayments/blockchain/client/TransactionStatus;)V");
    if (NULL == TransactionSubmissionRequest_setStatus)
        return 1;

    /* register getStatusFuture method. */
    TransactionSubmissionRequest_getStatusFuture =
        (*env)->GetMethodID(
            env, TransactionSubmissionRequest, "getStatusFuture",
            "()Ljava/util/concurrent/CompletableFuture;");
    if (NULL == TransactionSubmissionRequest_getStatusFuture)
        return 1;

    /* register getTxnData method. */
    TransactionSubmissionRequest_getTxnData =
        (*env)->GetMethodID(
            env, TransactionSubmissionRequest, "getTxnData",
            "()Lcom/velopayments/blockchain/cert/Certificate;");
    if (NULL == TransactionSubmissionRequest_getTxnData)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    TransactionSubmissionRequest_registered = true;
    return 0;
}
