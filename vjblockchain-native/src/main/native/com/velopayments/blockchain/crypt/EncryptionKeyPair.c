/**
 * \file EncryptionKeyPair.c
 *
 * Class and method exports for EncryptionKeyPair.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "EncryptionKeyPair.h"

jclass EncryptionKeyPair;
jmethodID EncryptionKeyPair_init;
jmethodID EncryptionKeyPair_getPublicKey;
jmethodID EncryptionKeyPair_getPrivateKey;

static volatile bool EncryptionKeyPair_registered = false;

/**
 * Property: EncryptionKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != EncryptionKeyPair \
     && NULL != EncryptionKeyPair_init \
     && NULL != EncryptionKeyPair_getPublicKey \
     && NULL != EncryptionKeyPair_getPrivateKey)

/**
 * Register the following EncryptionKeyPair references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EncryptionKeyPair_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register EncryptionKeyPair once. */
    if (EncryptionKeyPair_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register EncryptionKeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/EncryptionKeyPair");
    if (NULL == tempClassID)
        return 1;

    /* register EncryptionKeyPair class */
    EncryptionKeyPair = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == EncryptionKeyPair)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    EncryptionKeyPair_init =
        (*env)->GetMethodID(
            env, EncryptionKeyPair, "<init>",
            "(Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;"
             "Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;)V");
    if (NULL == EncryptionKeyPair_init)
        return 1;

    /* register getPublicKey method */
    EncryptionKeyPair_getPublicKey =
        (*env)->GetMethodID(
            env, EncryptionKeyPair, "getPublicKey",
            "()Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;");
    if (NULL == EncryptionKeyPair_getPublicKey)
        return 1;

    /* register getPrivateKey method */
    EncryptionKeyPair_getPrivateKey =
        (*env)->GetMethodID(
            env, EncryptionKeyPair, "getPrivateKey",
            "()Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;");
    if (NULL == EncryptionKeyPair_getPrivateKey)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    EncryptionKeyPair_registered = true;
    return 0;
}
