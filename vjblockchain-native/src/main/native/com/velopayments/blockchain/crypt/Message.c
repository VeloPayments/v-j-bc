/**
 * \file Message.c
 *
 * Class and method exports for Message.  This source file contains
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
    (   NULL != inst->Message.classid \
     && NULL != inst->Message.init \
     && NULL != inst->Message.getRawBytes)

/**
 * Register the following Message references and make them global.
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
Message_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/Message");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    inst->Message.classid = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->Message.classid)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->Message.init =
        (*env)->GetMethodID(
            env, inst->Message.classid, "<init>", "([B)V");
    if (NULL == inst->Message.init)
        return 1;

    /* register getRawBytes method */
    inst->Message.getRawBytes =
        (*env)->GetMethodID(
            env, inst->Message.classid, "getRawBytes", "()[B");
    if (NULL == inst->Message.getRawBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
