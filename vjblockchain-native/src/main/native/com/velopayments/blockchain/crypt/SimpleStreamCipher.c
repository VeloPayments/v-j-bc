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

#include "../init/init.h"

/**
 * Property: SimpleStreamCipher globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->SimpleStreamCipher.classid \
     && NULL != inst->SimpleStreamCipher.init \
     && NULL != inst->SimpleStreamCipher.getKey \
     && NULL != inst->SimpleStreamCipher.encrypt \
     && NULL != inst->SimpleStreamCipher.decrypt \
     && NULL != inst->SimpleStreamCipher.field_key)

/**
 * Register the following SimpleStreamCipher references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 * \param inst  native instance to initialize.
 *
 * \returns 0 on success and non-zero on failure.
 */
int
SimpleStreamCipher_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register SimpleStreamCipher class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/crypt/SimpleStreamCipher");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to SimpleStreamCipher. */
    inst->SimpleStreamCipher.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->SimpleStreamCipher.classid)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register constructor method. */
    inst->SimpleStreamCipher.init =
        (*env)->GetMethodID(
            env, inst->SimpleStreamCipher.classid, "<init>",
            "(Lcom/velopayments/blockchain/crypt/Key;)V");
    if (NULL == inst->SimpleStreamCipher.init)
        return 1;

    /* register getKey method. */
    inst->SimpleStreamCipher.getKey =
        (*env)->GetMethodID(
            env, inst->SimpleStreamCipher.classid, "getKey",
            "()Lcom/velopayments/blockchain/crypt/Key;");
    if (NULL == inst->SimpleStreamCipher.getKey)
        return 1;

    /* register encrypt method. */
    inst->SimpleStreamCipher.encrypt =
        (*env)->GetMethodID(
            env, inst->SimpleStreamCipher.classid, "encrypt", "([B)[B");
    if (NULL == inst->SimpleStreamCipher.encrypt)
        return 1;

    /* register decrypt method. */
    inst->SimpleStreamCipher.decrypt =
        (*env)->GetMethodID(
            env, inst->SimpleStreamCipher.classid, "decrypt", "([B)[B");
    if (NULL == inst->SimpleStreamCipher.decrypt)
        return 1;

    /* register key field */
    inst->SimpleStreamCipher.field_key =
        (*env)->GetFieldID(
            env, inst->SimpleStreamCipher.classid, "key",
            "Lcom/velopayments/blockchain/crypt/Key;");
    if (NULL == inst->SimpleStreamCipher.field_key)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
