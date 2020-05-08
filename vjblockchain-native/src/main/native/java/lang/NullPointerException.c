/**
 * \file NullPointerException.c
 *
 * Class and method exports for java.lang.NullPointerException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: NullPointerException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->NullPointerException.classid \
     && NULL != inst->NullPointerException.init \
     && NULL != inst->NullPointerException.init_String)

/**
 * Register the following NullPointerException references and make them global.
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
NullPointerException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register NullPointerException class */
    tempClassID = (*env)->FindClass(env, "java/lang/NullPointerException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class */
    inst->NullPointerException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->NullPointerException.classid)
        return 1;

    /* we're done with this local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->NullPointerException.init =
        (*env)->GetMethodID(env, inst->NullPointerException.classid,
        "<init>", "()V");
    if (NULL == inst->NullPointerException.init)
        return 1;

    /* register init(String) method */
    inst->NullPointerException.init_String =
        (*env)->GetMethodID(
            env, inst->NullPointerException.classid,
            "<init>", "(Ljava/lang/String;)V");
    if (NULL == inst->NullPointerException.init_String)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
