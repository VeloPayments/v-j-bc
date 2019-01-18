/**
 * \file SimpleStreamCipher.c
 *
 * Class and method exports for SimpleStreamCipher.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "SimpleStreamCipher.h"

jclass SimpleStreamCipher = NULL;
jmethodID SimpleStreamCipher_init = NULL;
jmethodID SimpleStreamCipher_getKey = NULL;
jmethodID SimpleStreamCipher_encrypt = NULL;
jmethodID SimpleStreamCipher_decrypt = NULL;
jfieldID SimpleStreamCipher_field_key = NULL;

static volatile bool SimpleStreamCipher_registered = false;

/**
 * Property: SimpleStreamCipher globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != SimpleStreamCipher \
     && NULL != SimpleStreamCipher_init \
     && NULL != SimpleStreamCipher_getKey \
     && NULL != SimpleStreamCipher_encrypt \
     && NULL != SimpleStreamCipher_decrypt \
     && NULL != SimpleStreamCipher_field_key)

/**
 * Register the following SimpleStreamCipher references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int SimpleStreamCipher_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register SimpleStreamCipher once. */
    if (SimpleStreamCipher_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register SimpleStreamCipher class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/SimpleStreamCipher");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to SimpleStreamCipher. */
    SimpleStreamCipher = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == SimpleStreamCipher)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register constructor method. */
    SimpleStreamCipher_init =
        (*env)->GetMethodID(env, SimpleStreamCipher, "<init>",
        "(Lcom/velopayments/blockchain/crypt/Key;)V");
    if (NULL == SimpleStreamCipher_init)
        return 1;

    /* register getKey method. */
    SimpleStreamCipher_getKey =
        (*env)->GetMethodID(env, SimpleStreamCipher, "getKey",
        "()Lcom/velopayments/blockchain/crypt/Key;");
    if (NULL == SimpleStreamCipher_getKey)
        return 1;

    /* register encrypt method. */
    SimpleStreamCipher_encrypt =
        (*env)->GetMethodID(env, SimpleStreamCipher, "encrypt", "([B)[B");
    if (NULL == SimpleStreamCipher_encrypt)
        return 1;

    /* register decrypt method. */
    SimpleStreamCipher_decrypt =
        (*env)->GetMethodID(env, SimpleStreamCipher, "decrypt", "([B)[B");
    if (NULL == SimpleStreamCipher_decrypt)
        return 1;

    /* register key field */
    SimpleStreamCipher_field_key =
        (*env)->GetFieldID(
            env, SimpleStreamCipher, "key",
            "Lcom/velopayments/blockchain/crypt/Key;");
    if (NULL == SimpleStreamCipher_field_key)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    SimpleStreamCipher_registered = true;
    return 0;
}
