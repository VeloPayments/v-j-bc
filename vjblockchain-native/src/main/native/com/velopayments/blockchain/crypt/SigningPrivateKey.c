/**
 * \file SigningPrivateKey.c
 *
 * Class and method exports for SigningPrivateKey.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "SigningKeyPair.h"

jclass SigningPrivateKey = NULL;
jmethodID SigningPrivateKey_init = NULL;
jmethodID SigningPrivateKey_getRawBytes = NULL;

static volatile bool SigningPrivateKey_registered = false;

/**
 * Property: SigningKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != SigningPrivateKey \
     && NULL != SigningPrivateKey_init \
     && NULL != SigningPrivateKey_getRawBytes)

/**
 * Register the following SigningPrivateKey references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int SigningPrivateKey_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register KeyPair once. */
    if (SigningPrivateKey_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/SigningPrivateKey");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    SigningPrivateKey = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == SigningPrivateKey)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    SigningPrivateKey_init =
        (*env)->GetMethodID(
            env, SigningPrivateKey, "<init>", "([B)V");
    if (NULL == SigningPrivateKey_init)
        return 1;

    /* register getRawBytes method */
    SigningPrivateKey_getRawBytes =
        (*env)->GetMethodID(
            env, SigningPrivateKey, "getRawBytes", "()[B");
    if (NULL == SigningPrivateKey_getRawBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    SigningPrivateKey_registered = true;
    return 0;
}
