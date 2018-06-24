/**
 * \file IllegalArgumentException.c
 *
 * Class and method exports for java.lang.IllegalArgumentException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "IllegalArgumentException.h"

jclass IllegalArgumentException = NULL;
jmethodID IllegalArgumentException_init = NULL;
jmethodID IllegalArgumentException_String = NULL;
jmethodID IllegalArgumentException_String_Throwable = NULL;
jmethodID IllegalArgumentException_Throwable = NULL;

static volatile bool IllegalArgumentException_registered = false;

/**
 * Property: IllegalArgumentException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != IllegalArgumentException \
     && NULL != IllegalArgumentException_init \
     && NULL != IllegalArgumentException_String \
     && NULL != IllegalArgumentException_String_Throwable \
     && NULL != IllegalArgumentException_Throwable)

/**
 * Register the following IllegalArgumentException references and make them
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
int IllegalArgumentException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.lang.IllegalArgumentException once. */
    if (IllegalArgumentException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register IllegalArgumentException class */
    tempClassID = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    IllegalArgumentException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == IllegalArgumentException)
        return 1;

    /* we don't need this local reference any more */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    IllegalArgumentException_init =
        (*env)->GetMethodID(env, IllegalArgumentException, "<init>", "()V");
    if (NULL == IllegalArgumentException_init)
        return 1;

    /* register init(String) method */
    IllegalArgumentException_String =
        (*env)->GetMethodID(
            env, IllegalArgumentException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == IllegalArgumentException_String)
        return 1;

    /* register init(String, Throwable) method */
    IllegalArgumentException_String_Throwable =
        (*env)->GetMethodID(
            env, IllegalArgumentException, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == IllegalArgumentException_String_Throwable)
        return 1;

    /* register init(Throwable) method */
    IllegalArgumentException_Throwable =
        (*env)->GetMethodID(env, IllegalArgumentException, "<init>",
            "(Ljava/lang/Throwable;)V");
    if (NULL == IllegalArgumentException_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    IllegalArgumentException_registered = true;
    return 0;
}
