/**
 * \file IllegalStateException.c
 *
 * Class and method exports for java.lang.IllegalStateException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: IllegalStateException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->IllegalStateException.classid \
     && NULL != inst->IllegalStateException.init \
     && NULL != inst->IllegalStateException.init_String \
     && NULL != inst->IllegalStateException.init_String_Throwable \
     && NULL != inst->IllegalStateException.init_Throwable)

/**
 * Register the following IllegalStateException references and make them global.
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
IllegalStateException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register IllegalStateException class */
    tempClassID = (*env)->FindClass(env, "java/lang/IllegalStateException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class */
    inst->IllegalStateException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->IllegalStateException.classid)
        return 1;

    /* we're done with this local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->IllegalStateException.init =
        (*env)->GetMethodID(
            env, inst->IllegalStateException.classid, "<init>", "()V");
    if (NULL == inst->IllegalStateException.init)
        return 1;

    /* register init(String) method */
    inst->IllegalStateException.init_String =
        (*env)->GetMethodID(
            env, inst->IllegalStateException.classid,
            "<init>", "(Ljava/lang/String;)V");
    if (NULL == inst->IllegalStateException.init_String)
        return 1;

    /* register init(String, Throwable) method */
    inst->IllegalStateException.init_String_Throwable =
        (*env)->GetMethodID(
            env, inst->IllegalStateException.classid,
            "<init>", "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == inst->IllegalStateException.init_String_Throwable)
        return 1;

    /* register init(Throwable) method */
    inst->IllegalStateException.init_Throwable =
        (*env)->GetMethodID(
            env, inst->IllegalStateException.classid,
            "<init>", "(Ljava/lang/Throwable;)V");
    if (NULL == inst->IllegalStateException.init_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
