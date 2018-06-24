/**
 * \file SigningKeyPair.c
 *
 * Class and method exports for SigningKeyPair.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "SigningKeyPair.h"

jclass SigningKeyPair = NULL;
jmethodID SigningKeyPair_init = NULL;

static volatile bool SigningKeyPair_registered = false;

/**
 * Property: SigningKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != SigningKeyPair \
     && NULL != SigningKeyPair_init)

/**
 * Register the following SigningKeyPair references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int SigningKeyPair_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register KeyPair once. */
    if (SigningKeyPair_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/SigningKeyPair");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    SigningKeyPair = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == SigningKeyPair)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    SigningKeyPair_init =
        (*env)->GetMethodID(
            env, SigningKeyPair, "<init>",
            "(Lcom/velopayments/blockchain/crypt/SigningPublicKey;"
             "Lcom/velopayments/blockchain/crypt/SigningPrivateKey;)V");
    if (NULL == SigningKeyPair_init)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    SigningKeyPair_registered = true;
    return 0;
}
