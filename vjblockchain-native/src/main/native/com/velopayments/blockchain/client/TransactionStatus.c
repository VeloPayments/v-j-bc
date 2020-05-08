/**
 * \file TransactionStatus.c
 *
 * Class and method exports for TransactionStatus.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: TransactionStatus globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->TransactionStatus.classid \
        NULL != inst->TransactionStatus.field_PENDING \
        NULL != inst->TransactionStatus.field_FAILED \
        NULL != inst->TransactionStatus.field_CANCELED \
        NULL != inst->TransactionStatus.field_SUCCEEDED \
        NULL != inst->TransactionStatus.field_UNKNOWN_DISCONNECTED)

/**
 * Register the following TransactionStatus references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 * \param inst  native instance to initialize.
 *
 * \returns 0 on success and non-zero on failure.
 */
int
TransactionStatus_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register TransactionStatus class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/client/TransactionStatus");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to TransactionStatus. */
    inst->TransactionStatus.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->TransactionStatus.classid)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register PENDING field. */
    inst->TransactionStatus.field_PENDING =
        (*env)->GetStaticFieldID(
            env, inst->TransactionStatus.classid, "PENDING",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == inst->TransactionStatus.field_PENDING)
        return 1;

    /* register FAILED field. */
    inst->TransactionStatus.field_FAILED =
        (*env)->GetStaticFieldID(
            env, inst->TransactionStatus.classid, "FAILED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == inst->TransactionStatus.field_FAILED)
        return 1;

    /* register CANCELED field. */
    inst->TransactionStatus.field_CANCELED =
        (*env)->GetStaticFieldID(
            env, inst->TransactionStatus.classid, "CANCELED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == inst->TransactionStatus.field_CANCELED)
        return 1;

    /* register SUCCEEDED field. */
    inst->TransactionStatus.field_SUCCEEDED =
        (*env)->GetStaticFieldID(
            env, inst->TransactionStatus.classid, "SUCCEEDED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == inst->TransactionStatus.field_SUCCEEDED)
        return 1;

    /* register UNKNOWN_DISCONNECTED field. */
    inst->TransactionStatus.field_UNKNOWN_DISCONNECTED =
        (*env)->GetStaticFieldID(
            env, inst->TransactionStatus.classid, "UNKNOWN_DISCONNECTED",
            "Lcom/velopayments/blockchain/client/TransactionStatus;");
    if (NULL == inst->TransactionStatus.field_UNKNOWN_DISCONNECTED)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
