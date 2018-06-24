/**
 * \file UnknownArtifactTypeException.c
 *
 * Class and method exports for UnknownArtifactTypeException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "UnknownArtifactTypeException.h"

jclass UnknownArtifactTypeException = NULL;
jmethodID UnknownArtifactTypeException_init_String = NULL;
jmethodID UnknownArtifactTypeException_init_UUID = NULL;

static volatile bool UnknownArtifactTypeException_registered = false;

/**
 * Property: UnknownArtifactTypeException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != UnknownArtifactTypeException \
     && NULL != UnknownArtifactTypeException_init_String \
     && NULL != UnknownArtifactTypeException_init_UUID)

/**
 * Register the following UnknownArtifactTypeException references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int UnknownArtifactTypeException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register UnknownEntityException once. */
    if (UnknownArtifactTypeException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register UnknownArtifactTypeException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/UnknownArtifactTypeException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    UnknownArtifactTypeException =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == UnknownArtifactTypeException)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    UnknownArtifactTypeException_init_String =
        (*env)->GetMethodID(
            env, UnknownArtifactTypeException, "<init>",
            "(Ljava/lang/String;)V");
    if (NULL == UnknownArtifactTypeException_init_String)
        return 1;

    /* register init(UUID) method */
    UnknownArtifactTypeException_init_UUID =
        (*env)->GetMethodID(
            env, UnknownArtifactTypeException, "<init>", "(Ljava/util/UUID;)V");
    if (NULL == UnknownArtifactTypeException_init_UUID)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    UnknownArtifactTypeException_registered = true;
    return 0;
}
