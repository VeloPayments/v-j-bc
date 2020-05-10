/**
 * \file Key_createRandomNative.c
 *
 * Generate a random symmetric key using the Velo crypto suite.
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_Key
 * Method:    createRandomNative
 * Signature: ()Lcom/velopayments/blockchain/crypt/Key;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_crypt_Key_createRandomNative(
    JNIEnv *env, jclass UNUSED(clazz), jlong nativeInst)
{
    jobject retval = NULL;

    vccrypt_buffer_t keyBuffer;
    vccrypt_prng_context_t prng;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);

    /* get a pointer to the native instance. */
    vjblockchain_native_instance* native_inst =
        (vjblockchain_native_instance*)nativeInst;

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vjblockchain not initialized.");
        return NULL;
    }

    /* initialize key buffer. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &keyBuffer, &native_inst->alloc_opts,
                    native_inst->crypto_suite.stream_cipher_opts.key_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not initialize a crypto buffer.");
        return NULL;
    }

    /* create a prng instance for generating the key. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_prng_init(
                    &native_inst->crypto_suite, &prng))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "prng instance creation failure.");
        goto buffer_dispose;
    }

    /* read data from the PRNG. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_prng_read(&prng, &keyBuffer, keyBuffer.size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "prng read failure.");
        goto prng_dispose;
    }

    /* create the key array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, keyBuffer.size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "outputArray creation failure.");
        goto prng_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
        (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "outputArray data read failure.");
        goto outputArray_dispose;
    }

    /* write to the output array. */
    MODEL_EXEMPT(
        memcpy(outputArrayData, keyBuffer.data, keyBuffer.size));

    /* commit the data to the output array. */
    (*env)->ReleaseByteArrayElements(env, outputArray, outputArrayData, 0);

    /* create a Key instance. */
    retval =
        (*env)->NewObject(
            env, native_inst->Key.classid,
            native_inst->Key.init, outputArray);

    /* fall through. */

outputArray_dispose:
    (*env)->DeleteLocalRef(env, outputArray);

prng_dispose:
    dispose((disposable_t*)&prng);

buffer_dispose:
    dispose((disposable_t*)&keyBuffer);

    return retval;
}
