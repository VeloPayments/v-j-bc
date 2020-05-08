/**
 * \file UnknownEntityException.c
 *
 * Class and method exports for UnknownEntityException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: UnknownEntityException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->UnknownEntityException.classid \
     && NULL != inst->UnknownEntityException.init_String \
     && NULL != inst->UnknownEntityException.init_UUID)

/**
 * Register the following UnknownEntityException references and make them
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
UnknownEntityException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register UnknownEntityException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/UnknownEntityException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    inst->UnknownEntityException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->UnknownEntityException.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    inst->UnknownEntityException.init_String =
        (*env)->GetMethodID(
            env, inst->UnknownEntityException.classid,
            "<init>", "(Ljava/lang/String;)V");
    if (NULL == inst->UnknownEntityException.init_String)
        return 1;

    /* register init(UUID) method */
    inst->UnknownEntityException.init_UUID =
        (*env)->GetMethodID(
            env, inst->UnknownEntityException.classid,
            "<init>", "(Ljava/util/UUID;)V");
    if (NULL == inst->UnknownEntityException.init_UUID)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
