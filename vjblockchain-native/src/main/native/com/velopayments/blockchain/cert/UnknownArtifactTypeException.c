/**
 * \file UnknownArtifactTypeException.c
 *
 * Class and method exports for UnknownArtifactTypeException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: UnknownArtifactTypeException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->UnknownArtifactTypeException.classid \
     && NULL != inst->UnknownArtifactTypeException.init_String \
     && NULL != inst->UnknownArtifactTypeException.init_UUID)

/**
 * Register the following UnknownArtifactTypeException references and make them
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
UnknownArtifactTypeException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register UnknownArtifactTypeException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/UnknownArtifactTypeException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    inst->UnknownArtifactTypeException.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->UnknownArtifactTypeException.classid)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    inst->UnknownArtifactTypeException.init_String =
        (*env)->GetMethodID(
            env, inst->UnknownArtifactTypeException.classid, "<init>",
            "(Ljava/lang/String;)V");
    if (NULL == inst->UnknownArtifactTypeException.init_String)
        return 1;

    /* register init(UUID) method */
    inst->UnknownArtifactTypeException.init_UUID =
        (*env)->GetMethodID(
            env, inst->UnknownArtifactTypeException.classid,
            "<init>", "(Ljava/util/UUID;)V");
    if (NULL == inst->UnknownArtifactTypeException.init_UUID)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
