/**
 * \file UnknownArtifactException.c
 *
 * Class and method exports for UnknownArtifactException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "UnknownArtifactException.h"

jclass UnknownArtifactException = NULL;
jmethodID UnknownArtifactException_init_String = NULL;
jmethodID UnknownArtifactException_init_UUID = NULL;

static volatile bool UnknownArtifactException_registered = false;

/**
 * Property: UnknownArtifactException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != UnknownArtifactException \
     && NULL != UnknownArtifactException_init_String \
     && NULL != UnknownArtifactException_init_UUID)

/**
 * Register the following UnknownArtifactException references and make them
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
int UnknownArtifactException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register UnknownArtifactException once. */
    if (UnknownArtifactException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register UnknownArtifactException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/UnknownArtifactException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    UnknownArtifactException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == UnknownArtifactException)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    UnknownArtifactException_init_String =
        (*env)->GetMethodID(
            env, UnknownArtifactException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == UnknownArtifactException_init_String)
        return 1;

    /* register init(UUID) method */
    UnknownArtifactException_init_UUID =
        (*env)->GetMethodID(
            env, UnknownArtifactException, "<init>", "(Ljava/util/UUID;)V");
    if (NULL == UnknownArtifactException_init_UUID)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    UnknownArtifactException_registered = true;
    return 0;
}
