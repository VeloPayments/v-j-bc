/**
 * \file EncryptedCertificateReader_encryptData.c
 *
 * Encrypt and MAC data using a stream cipher and an HMAC.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/stream_cipher.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"
#include "../../../../com/velopayments/blockchain/init/init.h"

#include <stdio.h>

/*
 * Class:     com_velopayments_blockchain_document_EncryptedDocumentBuilder
 * Method:    encryptData
 * Signature: ([B[B[BI)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_document_EncryptedDocumentBuilder_encryptData(
    JNIEnv *env, jclass UNUSED(clazz), jbyteArray key, jbyteArray iv,
    jbyteArray input, jint offset)
{
    jbyteArray retval = NULL;

    const size_t IV_SIZE = 8;

    vccrypt_buffer_t keyBuffer;
    vccrypt_stream_options_t stream_opts;
    vccrypt_stream_context_t stream;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(NULL != iv);
    MODEL_ASSERT(NULL != input);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the key parameter is not null. */
    if (NULL == key)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "key");
        return NULL;
    }

    /* verify that the iv parameter is not null. */
    if (NULL == iv)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "iv");
        return NULL;
    }

    /* verify that the input parameter is not null. */
    if (NULL == input)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "input");
        return NULL;
    }

    /* create buffer to hold the key. */
    if (0 != vccrypt_buffer_init(&keyBuffer, &alloc_opts, 32))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key buffer create failure.");
        return NULL;
    }

    /* get the buffer for the key array. */
    jbyte* keyArrayData = (*env)->GetByteArrayElements(env, key, NULL);
    if (NULL == keyArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "key data read failure.");
        goto keyBuffer_dispose;
    }

    /* copy the key data to the vccrypt key buffer. */
    MODEL_EXEMPT(
        memcpy(keyBuffer.data, keyArrayData, 32));

    /* get the buffer for the iv array. */
    jbyte* ivArrayData = (*env)->GetByteArrayElements(env, iv, NULL);
    if (NULL == ivArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "iv data read failure.");
        goto keyArrayData_dispose;
    }

    /* get the buffer for the input array. */
    jbyte* inputArrayData = (*env)->GetByteArrayElements(env, input, NULL);
    if (NULL == inputArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "input data read failure.");
        goto ivArrayData_dispose;
    }

    /* TODO - use the suite to initialize the stream cipher. */
    /* create the stream cipher options structure. */
    if (0 != vccrypt_stream_options_init(
                &stream_opts, &alloc_opts,
                VCCRYPT_STREAM_ALGORITHM_AES_256_2X_CTR))
    {
        (*env)->ThrowNew(env, IllegalStateException, "stream options failure.");
        goto inputArrayData_dispose;
    }

    /* create the stream cipher instance. */
    if (0 != vccrypt_stream_init(&stream_opts, &stream, &keyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException, "stream context failure.");
        goto stream_cipher_options_dispose;
    }

    /* the total size of the output array */
    size_t input_size = (*env)->GetArrayLength(env, input);
    size_t output_size =
        (0 == offset ? IV_SIZE : 0) +
        input_size;

    /* create the output byte array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, output_size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(env, NullPointerException, "bad outputArray alloc.");
        goto stream_cipher_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
        (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "outputArray data could not be read.");
        goto stream_cipher_dispose;
    }

    /* clear array to make output predictable.*/
    MODEL_EXEMPT(
        memset(outputArrayData, 0, output_size));

    uint8_t* out = (uint8_t*)outputArrayData;
    size_t sz_offset = 0;

    /* if offset is 0, start an encryption stream with the given output
       array. */
    if (0 == offset && 0 != vccrypt_stream_start_encryption(
                &stream, ivArrayData, 8, out, &sz_offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not start encryption stream.");
        goto stream_cipher_dispose;
    }

    /* encrypt the data */
    if (0 != vccrypt_stream_encrypt(
                &stream, inputArrayData, input_size, out, &sz_offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not encrypt input data.");
        goto stream_cipher_dispose;
    }

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, outputArray, outputArrayData, 0);

    /* Success... set our return value. */
    retval = outputArray;

stream_cipher_dispose:
    dispose((disposable_t*)&stream);

stream_cipher_options_dispose:
    dispose((disposable_t*)&stream_opts);

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

