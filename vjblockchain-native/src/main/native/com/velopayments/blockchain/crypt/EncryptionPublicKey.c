/**
 * \file EncryptionPublicKey.c
 *
 * Class and method exports for EncryptionPublicKey.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "EncryptionPublicKey.h"

jclass EncryptionPublicKey;
jmethodID EncryptionPublicKey_init;
jmethodID EncryptionPublicKey_getRawBytes;

static volatile bool EncryptionPublicKey_registered = false;

/**
 * Property: EncryptionPublicKey globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != EncryptionPublicKey \
     && NULL != EncryptionPublicKey_init \
     && NULL != EncryptionPublicKey_getRawBytes)

/**
 * Register the following EncryptionPublicKey references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EncryptionPublicKey_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register EncryptionPublicKey once. */
    if (EncryptionPublicKey_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register EncryptionPublicKey class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/EncryptionPublicKey");
    if (NULL == tempClassID)
        return 1;

    /* register EncryptionPublicKey class */
    EncryptionPublicKey = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == EncryptionPublicKey)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    EncryptionPublicKey_init =
        (*env)->GetMethodID(
            env, EncryptionPublicKey, "<init>",
            "([B)V");
    if (NULL == EncryptionPublicKey_init)
        return 1;

    /* register getRawBytes method */
    EncryptionPublicKey_getRawBytes =
        (*env)->GetMethodID(
            env, EncryptionPublicKey, "getRawBytes",
            "()[B");
    if (NULL == EncryptionPublicKey_getRawBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    EncryptionPublicKey_registered = true;
    return 0;
}
