/**
 * \file AttestationException.c
 *
 * Class and method exports for AttestationException.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "AttestationException.h"

jclass AttestationException = NULL;
jmethodID AttestationException_init_String = NULL;
jmethodID AttestationException_init_String_Throwable = NULL;

static volatile bool AttestationException_registered = false;

/**
 * Property: AttestationException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != AttestationException \
     && NULL != AttestationException_init_String \
     && NULL != AttestationException_init_String_Throwable)

/**
 * Register the following AttestationException references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int AttestationException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register AttestationException once. */
    if (AttestationException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register AttestationException class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/AttestationException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class. */
    AttestationException = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == AttestationException)
        return 1;

    /* we're done with our local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    AttestationException_init_String =
        (*env)->GetMethodID(
            env, AttestationException, "<init>", "(Ljava/lang/String;)V");
    if (NULL == AttestationException_init_String)
        return 1;

    /* register init(Throwable) method */
    AttestationException_init_String_Throwable =
        (*env)->GetMethodID(
            env, AttestationException, "<init>",
            "(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (NULL == AttestationException_init_String_Throwable)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    AttestationException_registered = true;
    return 0;
}
