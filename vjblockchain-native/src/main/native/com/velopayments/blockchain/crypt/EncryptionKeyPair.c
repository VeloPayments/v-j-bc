/**
 * \file EncryptionKeyPair.c
 *
 * Class and method exports for EncryptionKeyPair.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: EncryptionKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->EncryptionKeyPair.classid \
     && NULL != inst->EncryptionKeyPair.init \
     && NULL != inst->EncryptionKeyPair.getPublicKey \
     && NULL != inst->EncryptionKeyPair.getPrivateKey)

/**
 * Register the following EncryptionKeyPair references and make them global.
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
EncryptionKeyPair_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register EncryptionKeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/EncryptionKeyPair");
    if (NULL == tempClassID)
        return 1;

    /* register EncryptionKeyPair class */
    inst->EncryptionKeyPair.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->EncryptionKeyPair.classid)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->EncryptionKeyPair.init =
        (*env)->GetMethodID(
            env, inst->EncryptionKeyPair.classid, "<init>",
            "(Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;"
             "Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;)V");
    if (NULL == inst->EncryptionKeyPair.init)
        return 1;

    /* register getPublicKey method */
    inst->EncryptionKeyPair.getPublicKey =
        (*env)->GetMethodID(
            env, inst->EncryptionKeyPair.classid, "getPublicKey",
            "()Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;");
    if (NULL == inst->EncryptionKeyPair.getPublicKey)
        return 1;

    /* register getPrivateKey method */
    inst->EncryptionKeyPair.getPrivateKey =
        (*env)->GetMethodID(
            env, inst->EncryptionKeyPair.classid, "getPrivateKey",
            "()Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;");
    if (NULL == inst->EncryptionKeyPair.getPrivateKey)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
