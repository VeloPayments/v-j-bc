/**
 * \file SigningPublicKey.c
 *
 * Class and method exports for SigningPublicKey.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "SigningKeyPair.h"

jclass SigningPublicKey = NULL;
jmethodID SigningPublicKey_init = NULL;
jmethodID SigningPublicKey_getRawBytes = NULL;

static volatile bool SigningPublicKey_registered = false;

/**
 * Property: SigningKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != SigningPublicKey \
     && NULL != SigningPublicKey_init \
     && NULL != SigningPublicKey_getRawBytes)

/**
 * Register the following SigningPublicKey references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int SigningPublicKey_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register KeyPair once. */
    if (SigningPublicKey_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/SigningPublicKey");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    SigningPublicKey = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == SigningPublicKey)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    SigningPublicKey_init =
        (*env)->GetMethodID(
            env, SigningPublicKey, "<init>", "([B)V");
    if (NULL == SigningPublicKey_init)
        return 1;

    /* register getRawBytes method */
    SigningPublicKey_getRawBytes =
        (*env)->GetMethodID(
            env, SigningPublicKey, "getRawBytes", "()[B");
    if (NULL == SigningPublicKey_getRawBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    SigningPublicKey_registered = true;
    return 0;
}
