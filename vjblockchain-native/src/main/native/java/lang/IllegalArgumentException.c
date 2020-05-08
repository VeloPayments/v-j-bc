/**
 * \file IllegalArgumentException.c
 *
 * Class and method exports for java.lang.IllegalArgumentException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: IllegalArgumentException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->IllegalArgumentException.classid \
     && NULL != inst->IllegalArgumentException.init \
     && NULL != inst->IllegalArgumentException.String \
     && NULL != inst->IllegalArgumentException.String_Throwable \
     && NULL != inst->IllegalArgumentException.Throwable)

/**
 * Register the following IllegalArgumentException references and make them
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
IllegalArgumentException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register IllegalArgumentException class */
    tempClassID = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    inst->IllegalArgumentException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->IllegalArgumentException.classid)
        return 1;

    /* we don't need this local reference any more */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->IllegalArgumentException.init =
        (*env)->GetMethodID(
            env, inst->IllegalArgumentException.classid,
            "<init>", "()V");
    if (NULL == inst->IllegalArgumentException.init)
        return 1;

    /* register init(String) method */
    inst->IllegalArgumentException.init_String =
        (*env)->GetMethodID(
            env, inst->IllegalArgumentException.classid,
            "<init>", "(Ljava/lang/String;)V");
    if (NULL == inst->IllegalArgumentException.init_String)
        return 1;

    /* register init(String, Throwable) method */
    inst->IllegalArgumentException.init_String_Throwable =
        (*env)->GetMethodID(
            env, inst->IllegalArgumentException.classid, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == inst->IllegalArgumentException.init_String_Throwable)
        return 1;

    /* register init(Throwable) method */
    inst->IllegalArgumentException.init_Throwable =
        (*env)->GetMethodID(env, inst->IllegalArgumentException.classid,
            "<init>", "(Ljava/lang/Throwable;)V");
    if (NULL == inst->IllegalArgumentException.init_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
