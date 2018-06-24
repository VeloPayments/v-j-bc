/**
 * \file UnknownEntityException.c
 *
 * Class and method exports for UnknownEntityException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "UnknownEntityException.h"

jclass UnknownEntityException = NULL;
jmethodID UnknownEntityException_init_String = NULL;
jmethodID UnknownEntityException_init_UUID = NULL;

static volatile bool UnknownEntityException_registered = false;

/**
 * Property: UnknownEntityException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != UnknownEntityException \
     && NULL != UnknownEntityException_init_String \
     && NULL != UnknownEntityException_init_UUID)

/**
 * Register the following UnknownEntityException references and make them
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
int UnknownEntityException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register UnknownEntityException once. */
    if (UnknownEntityException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register UnknownEntityException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/UnknownEntityException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    UnknownEntityException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == UnknownEntityException)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    UnknownEntityException_init_String =
        (*env)->GetMethodID(
            env, UnknownEntityException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == UnknownEntityException_init_String)
        return 1;

    /* register init(UUID) method */
    UnknownEntityException_init_UUID =
        (*env)->GetMethodID(
            env, UnknownEntityException, "<init>", "(Ljava/util/UUID;)V");
    if (NULL == UnknownEntityException_init_UUID)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    UnknownEntityException_registered = true;
    return 0;
}
