/**
 * \file InvalidKeySizeException.c
 *
 * Class and method exports for InvalidKeySizeException.  This source file
 * contains a registration method that registers the class and method references
 * just once, making each of these a global reference.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "InvalidKeySizeException.h"

jclass InvalidKeySizeException = NULL;
jmethodID InvalidKeySizeException_init_String = NULL;
jmethodID InvalidKeySizeException_init_String_Throwable = NULL;

static volatile bool InvalidKeySizeException_registered = false;

/**
 * Property: InvalidKeySizeException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != InvalidKeySizeException \
     && NULL != InvalidKeySizeException_init_String \
     && NULL != InvalidKeySizeException_init_String_Throwable)

/**
 * Register the following InvalidKeySizeException references and make them
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
int InvalidKeySizeException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register InvalidKeySizeException once. */
    if (InvalidKeySizeException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register InvalidKeySizeException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/InvalidKeySizeException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class. */
    InvalidKeySizeException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == InvalidKeySizeException)
        return 1;

    /* we're done with our local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    InvalidKeySizeException_init_String =
        (*env)->GetMethodID(
            env, InvalidKeySizeException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == InvalidKeySizeException_init_String)
        return 1;

    /* register init(Throwable) method */
    InvalidKeySizeException_init_String_Throwable =
        (*env)->GetMethodID(
            env, InvalidKeySizeException, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == InvalidKeySizeException_init_String_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    InvalidKeySizeException_registered = true;
    return 0;
}
