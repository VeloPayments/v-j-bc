/**
 * \file SimpleStreamCipher_encryptNative.c
 *
 * Encrypt a byte array.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "InvalidKeySizeException.h"
#include "Key.h"
#include "SimpleStreamCipher.h"
#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"

/*
 * Class:     com_velopayments_blockchain_crypt_SimpleStreamCipher
 * Method:    encryptNative
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_crypt_SimpleStreamCipher_encryptNative(
    JNIEnv *env, jobject that, jbyteArray input)
{
    const size_t KEY_IV_SIZE =  16U;
    const size_t KEY_SIZE    =  32U;
    const size_t IV_SIZE     =   8U;
    const size_t MAC_SIZE    =  64U;

    jbyteArray retval = NULL;

    jobject key = NULL;
    jbyteArray keyBytes = NULL;
    jbyte* keyBytesData = NULL;
    vccrypt_prng_context_t prng;
    vccrypt_buffer_t keyBuffer;
    vccrypt_buffer_t sessionKeyBuffer;
    vccrypt_buffer_t sessionIVBuffer;
    vccrypt_buffer_t streamIVBuffer;
    vccrypt_buffer_t macBuffer;
    vccrypt_mac_context_t mac;
    vccrypt_block_context_t block;
    vccrypt_stream_context_t stream;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != input);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the input parameter is not null. */
    if (NULL == input)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "input");
        return NULL;
    }

    /* get the bytes for the input array. */
    jbyte* inputData = 
        (*env)->GetByteArrayElements(env, input, NULL);
    if (NULL == inputData)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "inputData");
        return NULL;
    }

    /* get the key field. */
    key = (*env)->GetObjectField(env, that, SimpleStreamCipher_field_key);
    if (NULL == key)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "key");
        goto inputData_dispose;
    }

    /* get the key byte array. */
    keyBytes = (*env)->CallObjectMethod(env, key, Key_getRawBytes);
    if (NULL == keyBytes)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "keyBytes");
        goto inputData_dispose;
    }

    /* get the raw bytes of this array. */
    keyBytesData = (*env)->GetByteArrayElements(env, keyBytes, NULL);
    if (NULL == keyBytesData)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "keyBytesData");
        goto inputData_dispose;
    }

    /* create a vccrypt_buffer for managing these key bytes. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                &keyBuffer, &alloc_opts,
                crypto_suite.stream_cipher_opts.key_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key buffer creation failure.");
        goto keyBytesData_dispose;
    }

    /* copy the key data to the key buffer. */
    memcpy(keyBuffer.data, keyBytesData,
            crypto_suite.stream_cipher_opts.key_size);

    /* calculate space for output buffer. */
    size_t input_size = (*env)->GetArrayLength(env, input);
    size_t output_size =
        KEY_IV_SIZE + KEY_SIZE + IV_SIZE + input_size + MAC_SIZE;

    /* allocate output buffer. */
    jbyteArray outputArray = (*env)->NewByteArray(env, output_size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(env, NullPointerException, "bad outputArray alloc.");
        goto keyBuffer_dispose;
    }

    /* get output array data. */
    jbyte* outputArrayData =
        (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "outputArray data could not be read.");
        goto keyBuffer_dispose;
    }

    /* create PRNG instance. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_prng_init(&crypto_suite, &prng))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "prng instance creation failure.");
        goto outputArrayData_dispose;
    }

    /* create a buffer for the session key. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                &sessionKeyBuffer, &alloc_opts,
                crypto_suite.stream_cipher_opts.key_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "session key buffer create failure.");
        goto prng_dispose;
    }

    /* create a key IV buffer for the session key iv. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                &sessionIVBuffer, &alloc_opts, 16))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "session iv buffer create failure.");
        goto sessionKeyBuffer_dispose;
    }

    /* create a stream IV buffer for the stream IV. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                &streamIVBuffer, &alloc_opts, 8))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "stream iv buffer create failure.");
        goto sessionIVBuffer_dispose;
    }

    /* create a random session key. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_prng_read(&prng, &sessionKeyBuffer, sessionKeyBuffer.size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "prng read failure.");
        goto streamIVBuffer_dispose;
    }

    /* create a random session key IV. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_prng_read(&prng, &sessionIVBuffer, sessionIVBuffer.size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "prng read failure.");
        goto streamIVBuffer_dispose;
    }

    /* create a random stream IV. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_prng_read(&prng, &streamIVBuffer, streamIVBuffer.size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "prng read failure.");
        goto streamIVBuffer_dispose;
    }

    /* create the HMAC instance for this. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_mac_init(&crypto_suite, &mac, &sessionKeyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "mac init failure.");
        goto streamIVBuffer_dispose;
    }

    /* create block cipher instance for this. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_block_init(&crypto_suite, &block, &keyBuffer, true))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "block cipher init failure.");
        goto mac_dispose;
    }

    /* the out pointer is a convenient way to step through the output array. */
    uint8_t* out = (uint8_t*)outputArrayData;

    /* the keyin pointer lets us step through the session key data. */
    uint8_t* keyin = (uint8_t*)sessionKeyBuffer.data;

    /* copy the iv to the output. */
    memcpy(out, sessionIVBuffer.data, sessionIVBuffer.size);

    /* encrypt the key, writing the data to the output. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_block_encrypt(
                &block, sessionIVBuffer.data, keyin, out + 16)
     || VCCRYPT_STATUS_SUCCESS !=
            vccrypt_block_encrypt(
                &block, out + 16, keyin + 16, out + 32))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "block cipher encrypt failure.");
        goto block_dispose;
    }

    /* digest all of this output data. */
    if (VCCRYPT_STATUS_SUCCESS != vccrypt_mac_digest(&mac, out, 48))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "message digest failure.");
        goto block_dispose;
    }

    /* commit the first 48 bytes of the message. */
    out += 48;

    /* create the stream cipher. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_stream_init(&crypto_suite, &stream, &sessionKeyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "stream cipher init failure.");
        goto block_dispose;
    }

    /* start the stream cipher. */
    size_t offset = 0;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_start_encryption(
                &stream, streamIVBuffer.data, streamIVBuffer.size, out,
                &offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "stream cipher start encryption failure.");
        goto stream_dispose;
    }

    /* encrypt the input data. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_encrypt(
                &stream, inputData, input_size, out, &offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "stream cipher encryption failure.");
        goto stream_dispose;
    }

    /* digest the ciphertext. */
    if (VCCRYPT_STATUS_SUCCESS != vccrypt_mac_digest(&mac, out, offset))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "message digest failure.");
        goto stream_dispose;
    }

    /* update the output pointer. */
    out += offset;

    /* create buffer for receiving the MAC. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_mac_authentication_code(
                &crypto_suite, &macBuffer, false))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "mac buffer init failure.");
        goto stream_dispose;
    }

    /* finalize the mac. */
    if (VCCRYPT_STATUS_SUCCESS != vccrypt_mac_finalize(&mac, &macBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "mac finalize failure.");
        goto macBuffer_dispose;
    }

    /* copy the mac to the output buffer. */
    memcpy(out, macBuffer.data, macBuffer.size);

    /* success. */
    retval = outputArray;

macBuffer_dispose:
    dispose((disposable_t*)&macBuffer);

stream_dispose:
    dispose((disposable_t*)&stream);

block_dispose:
    dispose((disposable_t*)&block);

mac_dispose:
    dispose((disposable_t*)&mac);

streamIVBuffer_dispose:
    dispose((disposable_t*)&streamIVBuffer);

sessionIVBuffer_dispose:
    dispose((disposable_t*)&sessionIVBuffer);

sessionKeyBuffer_dispose:
    dispose((disposable_t*)&sessionKeyBuffer);

prng_dispose:
    dispose((disposable_t*)&prng);

outputArrayData_dispose:
    (*env)->ReleaseByteArrayElements(env, outputArray, outputArrayData, 0);

keyBuffer_dispose:
    dispose((disposable_t*)&keyBuffer);

keyBytesData_dispose:
    (*env)->ReleaseByteArrayElements(env, keyBytes, keyBytesData, 0);

inputData_dispose:
    (*env)->ReleaseByteArrayElements(env, input, inputData, 0);

    return retval;
}
