/**
 * \file EncryptedDocumentReader_decryptNative.c
 *
 * Decrypt and verify data using a stream cipher and an HMAC.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/stream_cipher.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"


/*
 * Class:     com_velopayments_blockchain_document_EncryptedDocumentReader
 * Method:    decryptNative
 * Signature: ([B[B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_document_EncryptedDocumentReader_decryptNative
  (JNIEnv* env, jclass UNUSED(clazz), jbyteArray secretKey, jbyteArray input)
{
    jbyteArray retval = NULL;

    const size_t IV_SIZE = 8;

    vccrypt_buffer_t keyBuffer;
    vccrypt_stream_options_t stream_opts;
    vccrypt_stream_context_t stream;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != secretKey);
    MODEL_ASSERT(NULL != input);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException,"vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the secretKey parameter is not null. */
    if (NULL == secretKey)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "secretKey");
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
    jbyte* secretKeyArrayData =
        (*env)->GetByteArrayElements(env, secretKey, NULL);
    if (NULL == secretKeyArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "key data read failure.");
        goto keyBuffer_dispose;
    }

    /* copy the key data to the vccrypt key buffer. */
    MODEL_EXEMPT(
        memcpy(keyBuffer.data, secretKeyArrayData, 32));

    /* get the buffer for the input array. */
    jbyte* inputArrayData = (*env)->GetByteArrayElements(env, input, NULL);
    if (NULL == inputArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "input data read failure.");
        goto secretKeyArrayData_dispose;
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
    size_t output_size = input_size - IV_SIZE;

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

    const uint8_t* in = (const uint8_t*)inputArrayData;
    uint8_t* out = (uint8_t*)outputArrayData;
    size_t offset = 0;
    size_t input_offset = 0;

    /* start a decryption stream with the given output array. */
    if (0 != vccrypt_stream_start_decryption(&stream, in, &input_offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not start decryption stream.");
        goto stream_cipher_dispose;
    }

    /* decrypt the data */
    if (0 != vccrypt_stream_decrypt(
                &stream, in + input_offset, input_size - input_offset,
                out, &offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not decrypt input data.");
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

secretKeyArrayData_dispose:
    (*env)->ReleaseByteArrayElements(
                env, secretKey, secretKeyArrayData, JNI_ABORT);

keyBuffer_dispose:
    dispose((disposable_t*)&keyBuffer);

    return retval;
}
