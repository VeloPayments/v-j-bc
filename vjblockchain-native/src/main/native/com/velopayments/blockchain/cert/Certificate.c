/**
 * \file Certificate.c
 *
 * Class and method exports for Certificate.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "Certificate.h"

jclass Certificate = NULL;
jmethodID Certificate_fromByteArray = NULL;
jmethodID Certificate_toByteArray = NULL;

static volatile bool Certificate_registered = false;

/**
 * Property: Certificate globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != Certificate \
        NULL != Certificate_fromByteArray \
        NULL != Certificate_toByteArray)

/**
 * Register the following Certificate references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Certificate_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register Certificate once. */
    if (Certificate_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register Certificate class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/Certificate");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to Certificate. */
    Certificate = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == Certificate)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register fromByteArray method. */
    Certificate_fromByteArray =
        (*env)->GetStaticMethodID(
            env, Certificate, "fromByteArray",
            "([B)Lcom/velopayments/blockchain/cert/Certificate;");
    if (NULL == Certificate_fromByteArray)
        return 1;

    /* register toByteArray method. */
    Certificate_toByteArray =
        (*env)->GetMethodID(
            env, Certificate, "toByteArray",
            "()[B");
    if (NULL == Certificate_toByteArray)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    Certificate_registered = true;
    return 0;
}
