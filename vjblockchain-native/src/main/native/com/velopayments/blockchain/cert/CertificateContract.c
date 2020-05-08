/**
 * \file CertificateContract.c
 *
 * Class and method exports for CertificateContract.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: CertificateContract globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->CertificateContract.classid \
     && NULL != inst->CertificateContract.verify)

/**
 * Register the following CertificateContract references and make them global.
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
CertificateContract_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register certificatecontract class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateContract");
    if (NULL == tempClassID)
        return 1;

    /* create global class reference */
    inst->CertificateContract.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->CertificateContract.classid)
        return 1;

    /* release local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register verify method */
    inst->CertificateContract.verify =
        (*env)->GetMethodID(
            env, inst->CertificateContract.classid, "verify",
            "(Lcom/velopayments/blockchain/cert/CertificateParser;)Z");
    if (NULL == inst->CertificateContract.verify)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
