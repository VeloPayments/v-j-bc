/**
 * \file GenericStreamCipher_encryptNative.c
 *
 * Generate a digital signature keypair using the Velo crypto suite.
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_GenericStreamCipher
 * Method:    encryptNative
 * Signature: ([B[B[B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_crypt_GenericStreamCipher_encryptNative(
        JNIEnv *env, jclass UNUSED(clazz), jbyteArray key, jbyteArray iv,
        jbyteArray input)
{
    jbyteArray retval = NULL;

    const size_t IV_SIZE = 8;

    vccrypt_buffer_t keyBuffer;
    vccrypt_stream_context_t stream;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(NULL != iv);
    MODEL_ASSERT(NULL != input);

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the key parameter is not null. */
    if (NULL == key)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "key");
        return NULL;
    }

    /* verify that the iv parameter is not null. */
    if (NULL == iv)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "iv");
        return NULL;
    }

    /* verify that the input parameter is not null. */
    if (NULL == input)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "input");
        return NULL;
    }

    /* verify the secret key size */
    size_t key_size = (*env)->GetArrayLength(env, key);
    if (key_size != native_inst->crypto_suite.stream_cipher_opts.key_size)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "Invalid key size.");
        return NULL;
    }

    /* create buffer to hold the key. */
    if (0 != vccrypt_buffer_init(
                    &keyBuffer, &native_inst->alloc_opts, key_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "key buffer create failure.");
        return NULL;
    }

    /* get the buffer for the key array. */
    jbyte* keyArrayData = (*env)->GetByteArrayElements(env, key, NULL);
    if (NULL == keyArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "key data read failure.");
        goto keyBuffer_dispose;
    }

    /* copy the key data to the vccrypt key buffer. */
    MODEL_EXEMPT(
            memcpy(keyBuffer.data, keyArrayData, key_size));

    /* get the buffer for the iv array. */
    jbyte* ivArrayData = (*env)->GetByteArrayElements(env, iv, NULL);
    if (NULL == ivArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "iv data read failure.");
        goto keyArrayData_dispose;
    }

    /* get the buffer for the input array. */
    jbyte* inputArrayData = (*env)->GetByteArrayElements(env, input, NULL);
    if (NULL == inputArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "input data read failure.");
        goto ivArrayData_dispose;
    }

    /* create the stream cipher instance. */
    if (0 !=
            vccrypt_suite_stream_init(
                &native_inst->crypto_suite, &stream, &keyBuffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "stream context failure.");
        goto inputArrayData_dispose;
    }

    /* the total size of the output array */
    size_t input_size = (*env)->GetArrayLength(env, input);
    size_t output_size =
            IV_SIZE +
            input_size;

    /* create the output byte array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, output_size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "bad outputArray alloc.");
        goto stream_cipher_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
            (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "outputArray data could not be read.");
        goto stream_cipher_dispose;
    }

    /* clear array to make output predictable.*/
    MODEL_EXEMPT(
            memset(outputArrayData, 0, output_size));

    uint8_t* out = (uint8_t*)outputArrayData;
    size_t offset = 0;

    /* start an encryption stream with the given output array. */
    if (0 !=
            vccrypt_stream_start_encryption(
                &stream, ivArrayData, 8, out, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not start encryption stream.");
        goto stream_cipher_dispose;
    }

    /* encrypt the data */
    if (0 !=
            vccrypt_stream_encrypt(
                &stream, inputArrayData, input_size, out, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not encrypt input data.");
        goto stream_cipher_dispose;
    }

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, outputArray, outputArrayData, 0);

    /* Success... set our return value. */
    retval = outputArray;

stream_cipher_dispose:
    dispose((disposable_t*)&stream);

inputArrayData_dispose:
    (*env)->ReleaseByteArrayElements(env, input, inputArrayData, JNI_ABORT);

ivArrayData_dispose:
    (*env)->ReleaseByteArrayElements(env, iv, ivArrayData, JNI_ABORT);

keyArrayData_dispose:
    (*env)->ReleaseByteArrayElements(env, key, keyArrayData, JNI_ABORT);

keyBuffer_dispose:
    dispose((disposable_t*)&keyBuffer);

    return retval;
}
