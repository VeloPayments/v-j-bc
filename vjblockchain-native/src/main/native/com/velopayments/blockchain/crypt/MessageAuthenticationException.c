/**
 * \file MessageAuthenticationException.c
 *
 * Class and method exports for MessageAuthenticationException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "MessageAuthenticationException.h"

jclass MessageAuthenticationException = NULL;
jmethodID MessageAuthenticationException_init_String = NULL;
jmethodID MessageAuthenticationException_init_String_Throwable = NULL;

static volatile bool MessageAuthenticationException_registered = false;

/**
 * Property: MessageAuthenticationException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != MessageAuthenticationException \
     && NULL != MessageAuthenticationException_init_String \
     && NULL != MessageAuthenticationException_init_String_Throwable)

/**
 * Register the following MessageAuthenticationException references and make
 * them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int MessageAuthenticationException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register MessageAuthenticationException once. */
    if (MessageAuthenticationException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register MessageAuthenticationException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/MessageAuthenticationException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class. */
    MessageAuthenticationException =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == MessageAuthenticationException)
        return 1;

    /* we're done with our local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    MessageAuthenticationException_init_String =
        (*env)->GetMethodID(
            env, MessageAuthenticationException, "<init>",
            "(Ljava/lang/String;)V");
    if (NULL == MessageAuthenticationException_init_String)
        return 1;

    /* register init(Throwable) method */
    MessageAuthenticationException_init_String_Throwable =
        (*env)->GetMethodID(
            env, MessageAuthenticationException, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == MessageAuthenticationException_init_String_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    MessageAuthenticationException_registered = true;
    return 0;
}
