/**
 * \file IllegalStateException.c
 *
 * Class and method exports for java.lang.IllegalStateException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "IllegalStateException.h"

jclass IllegalStateException = NULL;
jmethodID IllegalStateException_init = NULL;
jmethodID IllegalStateException_init_String = NULL;
jmethodID IllegalStateException_String_Throwable = NULL;
jmethodID IllegalStateException_Throwable = NULL;

static volatile bool IllegalStateException_registered = false;

/**
 * Property: IllegalStateException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != IllegalStateException \
     && NULL != IllegalStateException_init \
     && NULL != IllegalStateException_init_String \
     && NULL != IllegalStateException_String_Throwable \
     && NULL != IllegalStateException_Throwable)

/**
 * Register the following IllegalStateException references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int IllegalStateException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.lang.IllegalStateException once. */
    if (IllegalStateException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register IllegalStateException class */
    tempClassID = (*env)->FindClass(env, "java/lang/IllegalStateException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class */
    IllegalStateException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == IllegalStateException)
        return 1;

    /* we're done with this local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    IllegalStateException_init =
        (*env)->GetMethodID(env, IllegalStateException, "<init>", "()V");
    if (NULL == IllegalStateException_init)
        return 1;

    /* register init(String) method */
    IllegalStateException_init_String =
        (*env)->GetMethodID(
            env, IllegalStateException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == IllegalStateException_init_String)
        return 1;

    /* register init(String, Throwable) method */
    IllegalStateException_String_Throwable =
        (*env)->GetMethodID(
            env, IllegalStateException, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == IllegalStateException_String_Throwable)
        return 1;

    /* register init(Throwable) method */
    IllegalStateException_Throwable =
        (*env)->GetMethodID(
            env, IllegalStateException, "<init>", "(Ljava/lang/Throwable;)V");
    if (NULL == IllegalStateException_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    IllegalStateException_registered = true;
    return 0;
}
