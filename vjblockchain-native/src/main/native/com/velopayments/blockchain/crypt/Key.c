/**
 * \file Key.c
 *
 * Class and method exports for Key.  This source file contains a registration
 * method that registers the class and method references just once, making each
 * of these a global reference.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "Key.h"

jclass Key = NULL;
jmethodID Key_init = NULL;
jmethodID Key_getRawBytes = NULL;
jfieldID Key_key = NULL;

static volatile bool Key_registered = false;

/**
 * Property: Key globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != Key \
     && NULL != Key_init \
     && NULL != Key_getRawBytes \
     && NULL != Key_key)

/**
 * Register the following Key references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Key_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register Key once. */
    if (Key_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register Key class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/Key");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to Key. */
    Key = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == Key)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register Key_init method. */
    Key_init = (*env)->GetMethodID(env, Key, "<init>", "([B)V");
    if (NULL == Key_init)
        return 1;

    /* register Key_getRawBytes method. */
    Key_getRawBytes = (*env)->GetMethodID(env, Key, "getRawBytes", "()[B");
    if (NULL == Key_getRawBytes)
        return 1;

    /* register key field */
    Key_key = (*env)->GetFieldID(env, Key, "key", "[B");
    if (NULL == Key_key)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    Key_registered = true;
    return 0;
}
