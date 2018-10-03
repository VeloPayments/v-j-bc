/**
 * \file NullPointerException.c
 *
 * Class and method exports for java.lang.NullPointerException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "NullPointerException.h"

jclass NullPointerException = NULL;
jmethodID NullPointerException_init = NULL;
jmethodID NullPointerException_init_String = NULL;

static volatile bool NullPointerException_registered = false;

/**
 * Property: NullPointerException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != NullPointerException \
     && NULL != NullPointerException_init \
     && NULL != NullPointerException_init_String)

/**
 * Register the following NullPointerException references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int NullPointerException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.lang.NullPointerException once. */
    if (NullPointerException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register NullPointerException class */
    tempClassID = (*env)->FindClass(env, "java/lang/NullPointerException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class */
    NullPointerException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == NullPointerException)
        return 1;

    /* we're done with this local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    NullPointerException_init =
        (*env)->GetMethodID(env, NullPointerException, "<init>", "()V");
    if (NULL == NullPointerException_init)
        return 1;

    /* register init(String) method */
    NullPointerException_init_String =
        (*env)->GetMethodID(
            env, NullPointerException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == NullPointerException_init_String)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    NullPointerException_registered = true;
    return 0;
}
