/**
 * \file InvalidKeySizeException.c
 *
 * Class and method exports for InvalidKeySizeException.  This source file
 * contains a registration method that registers the class and method references
 * just once, making each of these a global reference.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: InvalidKeySizeException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->InvalidKeySizeException.classid \
     && NULL != inst->InvalidKeySizeException.init_String \
     && NULL != inst->InvalidKeySizeException.init_String_Throwable)

/**
 * Register the following InvalidKeySizeException references and make them
 * global.
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
InvalidKeySizeException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register InvalidKeySizeException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/InvalidKeySizeException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class. */
    inst->InvalidKeySizeException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->InvalidKeySizeException.classid)
        return 1;

    /* we're done with our local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    inst->InvalidKeySizeException.init_String =
        (*env)->GetMethodID(
            env, inst->InvalidKeySizeException.classid,
            "<init>", "(Ljava/lang/String;)V");
    if (NULL == inst->InvalidKeySizeException.init_String)
        return 1;

    /* register init(Throwable) method */
    inst->InvalidKeySizeException.init_String_Throwable =
        (*env)->GetMethodID(
            env, inst->InvalidKeySizeException.classid, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == inst->InvalidKeySizeException.init_String_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
