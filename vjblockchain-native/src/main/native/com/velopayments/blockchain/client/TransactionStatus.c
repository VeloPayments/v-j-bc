/**
 * \file TransactionStatus.c
 *
 * Class and method exports for TransactionStatus.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "TransactionStatus.h"

jclass TransactionStatus = NULL;
jfieldID TransactionStatus_field_PENDING = NULL;
jfieldID TransactionStatus_field_FAILED = NULL;
jfieldID TransactionStatus_field_CANCELED = NULL;
jfieldID TransactionStatus_field_SUCCEEDED = NULL;
jfieldID TransactionStatus_field_UNKNOWN_DISCONNECTED = NULL;

static volatile bool TransactionStatus_registered = false;

/**
 * Property: TransactionStatus globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != TransactionStatus \
        NULL != TransactionStatus_field_PENDING \
        NULL != TransactionStatus_field_FAILED \
        NULL != TransactionStatus_field_CANCELED \
        NULL != TransactionStatus_field_SUCCEEDED \
        NULL != TransactionStatus_field_UNKNOWN_DISCONNECTED)

/**
 * Register the following TransactionStatus references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int TransactionStatus_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register TransactionStatus once. */
    if (TransactionStatus_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register TransactionStatus class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/client/TransactionStatus");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to TransactionStatus. */
    TransactionStatus = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == TransactionStatus)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register PENDING field. */
    TransactionStatus_field_PENDING =
        (*env)->GetStaticFieldID(
            env, TransactionStatus, "PENDING",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == TransactionStatus_field_PENDING)
        return 1;

    /* register FAILED field. */
    TransactionStatus_field_FAILED =
        (*env)->GetStaticFieldID(
            env, TransactionStatus, "FAILED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == TransactionStatus_field_FAILED)
        return 1;

    /* register CANCELED field. */
    TransactionStatus_field_CANCELED =
        (*env)->GetStaticFieldID(
            env, TransactionStatus, "CANCELED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == TransactionStatus_field_CANCELED)
        return 1;

    /* register SUCCEEDED field. */
    TransactionStatus_field_SUCCEEDED =
        (*env)->GetStaticFieldID(
            env, TransactionStatus, "SUCCEEDED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == TransactionStatus_field_SUCCEEDED)
        return 1;

    /* register UNKNOWN_DISCONNECTED field. */
    TransactionStatus_field_UNKNOWN_DISCONNECTED =
        (*env)->GetStaticFieldID(
            env, TransactionStatus, "UNKNOWN_DISCONNECTED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == TransactionStatus_field_UNKNOWN_DISCONNECTED)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    TransactionStatus_registered = true;
    return 0;
}
