/**
 * \file MessageVerificationException.c
 *
 * Class and method exports for
 * com.velopayments.blockchain.agentd.MessageVerificationException.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "MessageVerificationException.h"

jclass MessageVerificationException = NULL;
jmethodID MessageVerificationException_String = NULL;

static volatile bool MessageVerificationException_registered = false;

/**
 * Property: MessageVerificationException globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != MessageVerificationException \
     && NULL != MessageVerificationException_String)

/**
 * Register the following MessageVerificationException references and make them
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
int MessageVerificationException_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register
     * com.velopayments.blockchain.agentd.MessageVerificationException once. */
    if (MessageVerificationException_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register MessageVerificationException class */
    tempClassID =
        (*env)->FindClass(env,
            "com/velopayments/blockchain/agentd/MessageVerificationException");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    MessageVerificationException =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == MessageVerificationException)
        return 1;

    /* we don't need this local reference any more */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(String) method */
    MessageVerificationException_String =
        (*env)->GetMethodID(
            env, MessageVerificationException, "<init>",
            "(Ljava/lang/String;)V");
    if (NULL == MessageVerificationException_String)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    MessageVerificationException_registered = true;
    return 0;
}
