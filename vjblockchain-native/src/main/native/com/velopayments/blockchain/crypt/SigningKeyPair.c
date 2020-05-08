/**
 * \file SigningKeyPair.c
 *
 * Class and method exports for SigningKeyPair.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: SigningKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->SigningKeyPair.classid \
     && NULL != inst->SigningKeyPair.init)

/**
 * Register the following SigningKeyPair references and make them global.
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
SigningKeyPair_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/SigningKeyPair");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    inst->SigningKeyPair.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->SigningKeyPair.classid)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->SigningKeyPair.init =
        (*env)->GetMethodID(
            env, inst->SigningKeyPair.classid, "<init>",
            "(Lcom/velopayments/blockchain/crypt/SigningPublicKey;"
             "Lcom/velopayments/blockchain/crypt/SigningPrivateKey;)V");
    if (NULL == inst->SigningKeyPair.init)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
