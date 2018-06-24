/**
 * \file CertificateContract.c
 *
 * Class and method exports for CertificateContract.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "CertificateContract.h"

jclass CertificateContract = NULL;
jmethodID CertificateContract_verify = NULL;

static volatile bool CertificateContract_registered = false;

/**
 * Property: CertificateContract globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != CertificateContract \
     && NULL != CertificateContract_verify)

/**
 * Register the following CertificateContract references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int CertificateContract_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register UnknownEntityException once. */
    if (CertificateContract_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register certificatecontract class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateContract");
    if (NULL == tempClassID)
        return 1;

    /* create global class reference */
    CertificateContract = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == CertificateContract)
        return 1;

    /* release local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register verify method */
    CertificateContract_verify =
        (*env)->GetMethodID(
            env, CertificateContract, "verify",
            "(Lcom/velopayments/blockchain/cert/CertificateParser;)Z");
    if (NULL == CertificateContract_verify)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    CertificateContract_registered = true;
    return 0;
}
