/**
 * \file Key.c
 *
 * Class and method exports for Key.  This source file contains a registration
 * method that registers the class and method references just once, making each
 * of these a global reference.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: Key globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->Key.classid \
     && NULL != inst->Key.init \
     && NULL != inst->Key.getRawBytes \
     && NULL != inst->Key.key)

/**
 * Register the following Key references and make them global.
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
Key_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register Key class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/Key");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to Key. */
    inst->Key.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->Key.classid)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register Key_init method. */
    inst->Key.init =
        (*env)->GetMethodID(env, inst->Key.classid, "<init>", "([B)V");
    if (NULL == inst->Key.init)
        return 1;

    /* register Key_getRawBytes method. */
    inst->Key.getRawBytes =
        (*env)->GetMethodID(env, inst->Key.classid, "getRawBytes", "()[B");
    if (NULL == inst->Key.getRawBytes)
        return 1;

    /* register key field */
    inst->Key.key =
        (*env)->GetFieldID(
            env, inst->Key.classid, "key", "[B");
    if (NULL == inst->Key.key)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
