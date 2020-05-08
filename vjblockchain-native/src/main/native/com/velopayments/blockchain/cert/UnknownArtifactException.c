/**
 * \file UnknownArtifactException.c
 *
 * Class and method exports for UnknownArtifactException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: UnknownArtifactException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->UnknownArtifactException.classid \
     && NULL != inst->UnknownArtifactException.init_String \
     && NULL != inst->UnknownArtifactException.init_UUID)

/**
 * Register the following UnknownArtifactException references and make them
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
UnknownArtifactException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register UnknownArtifactException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/UnknownArtifactException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    inst->UnknownArtifactException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->UnknownArtifactException.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    inst->UnknownArtifactException.init_String =
        (*env)->GetMethodID(
            env, inst->UnknownArtifactException.classid,
            "<init>", "(Ljava/lang/String;)V");
    if (NULL == inst->UnknownArtifactException.init_String)
        return 1;

    /* register init(UUID) method */
    inst->UnknownArtifactException.init_UUID =
        (*env)->GetMethodID(
            env, inst->UnknownArtifactException.classid,
            "<init>", "(Ljava/util/UUID;)V");
    if (NULL == inst->UnknownArtifactException.init_UUID)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
