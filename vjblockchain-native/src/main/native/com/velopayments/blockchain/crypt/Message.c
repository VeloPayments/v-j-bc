/**
 * \file Message.c
 *
 * Class and method exports for Message.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "SigningKeyPair.h"

jclass Message = NULL;
jmethodID Message_init = NULL;
jmethodID Message_getRawBytes = NULL;

static volatile bool Message_registered = false;

/**
 * Property: SigningKeyPair globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != Message \
     && NULL != Message_init \
     && NULL != Message_getRawBytes)

/**
 * Register the following Message references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Message_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register KeyPair once. */
    if (Message_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register KeyPair class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/Message");
    if (NULL == tempClassID)
        return 1;

    /* register KeyPair class */
    Message = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == Message)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    Message_init =
        (*env)->GetMethodID(
            env, Message, "<init>", "([B)V");
    if (NULL == Message_init)
        return 1;

    /* register getRawBytes method */
    Message_getRawBytes =
        (*env)->GetMethodID(
            env, Message, "getRawBytes", "()[B");
    if (NULL == Message_getRawBytes)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    Message_registered = true;
    return 0;
}
