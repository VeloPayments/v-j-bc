/**
 * \file EncryptionPrivateKey.c
 *
 * Class and method exports for EncryptionPrivateKey.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "EncryptionPrivateKey.h"

jclass EncryptionPrivateKey;
jmethodID EncryptionPrivateKey_init;
jmethodID EncryptionPrivateKey_getRawBytes;

static volatile bool EncryptionPrivateKey_registered = false;

/**
 * Property: EncryptionPrivateKey globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != EncryptionPrivateKey \
     && NULL != EncryptionPrivateKey_init \
     && NULL != EncryptionPrivateKey_getRawBytes)

/**
 * Register the following EncryptionPrivateKey references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EncryptionPrivateKey_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register EncryptionPrivateKey once. */
    if (EncryptionPrivateKey_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register EncryptionPrivateKey class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/EncryptionPrivateKey");
    if (NULL == tempClassID)
        return 1;

    /* register EncryptionPrivateKey class */
    EncryptionPrivateKey = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == EncryptionPrivateKey)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    EncryptionPrivateKey_init =
        (*env)->GetMethodID(
            env, EncryptionPrivateKey, "<init>",
            "([B)V");
    if (NULL == EncryptionPrivateKey_init)
        return 1;

    /* register getRawBytes method */
    EncryptionPrivateKey_getRawBytes =
        (*env)->GetMethodID(
            env, EncryptionPrivateKey, "getRawBytes",
            "()[B");
    if (NULL == EncryptionPrivateKey_getRawBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    EncryptionPrivateKey_registered = true;
    return 0;
}
