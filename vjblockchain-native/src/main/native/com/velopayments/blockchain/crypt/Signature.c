/**
 * \file Signature.c
 *
 * Class and method exports for Signature.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "SigningKeyPair.h"

jclass Signature = NULL;
jmethodID Signature_init = NULL;
jmethodID Signature_getSignatureBytes = NULL;

static volatile bool Signature_registered = false;

/**
 * Property: SigningKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != Signature \
     && NULL != Signature_init \
     && NULL != Signature_getSignatureBytes)

/**
 * Register the following Signature references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Signature_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register KeyPair once. */
    if (Signature_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/Signature");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    Signature = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == Signature)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    Signature_init =
        (*env)->GetMethodID(
            env, Signature, "<init>", "([B)V");
    if (NULL == Signature_init)
        return 1;

    /* register getSignatureBytes method */
    Signature_getSignatureBytes =
        (*env)->GetMethodID(
            env, Signature, "getSignatureBytes", "()[B");
    if (NULL == Signature_getSignatureBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    Signature_registered = true;
    return 0;
}
