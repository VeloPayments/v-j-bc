/**
 * \file Integer.c
 *
 * Class and method exports for java.lang.Integer.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: Integer globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->Integer.classid \
     && NULL != inst->Integer.valueOf \
     && NULL != inst->Integer.intValue)

/**
 * Register the following Integer references and make them global.
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
Integer_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register Integer class */
    tempClassID = (*env)->FindClass(env, "java/lang/Integer");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class */
    inst->Integer.classid = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->Integer.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register valueOf method */
    inst->Integer.valueOf =
        (*env)->GetStaticMethodID(
            env, inst->Integer.classid, "valueOf", "(I)Ljava/lang/Integer;");
    if (NULL == inst->Integer.valueOf)
        return 1;

    /* register intValue method */
    inst->Integer.intValue =
        (*env)->GetMethodID(
            env, inst->Integer.classid, "intValue", "()I");
    if (NULL == inst->Integer.intValue)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
