/**
 * \file SimpleStreamCipher_decryptNative.c
 *
 * Decrypt a byte array.
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/compare.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_SimpleStreamCipher
 * Method:    decryptNative
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_crypt_SimpleStreamCipher_decryptNative(
    JNIEnv *env, jobject that, jlong nativeInst, jbyteArray input)
{
    const size_t KEY_IV_SIZE =  16U;
    const size_t KEY_SIZE    =  32U;
    const size_t IV_SIZE     =   8U;
    const size_t MAC_SIZE    =  64U;

    jbyteArray retval = NULL;

    jobject key = NULL;
    jbyteArray keyBytes = NULL;
    jbyte* keyBytesData = NULL;
    vccrypt_buffer_t keyBuffer;
    vccrypt_buffer_t sessionKeyBuffer;
    vccrypt_buffer_t macBuffer;
    vccrypt_mac_context_t mac;
    vccrypt_block_context_t block;
    vccrypt_stream_context_t stream;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != input);

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

    /* verify that the input parameter is not null. */
    if (NULL == input)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "input");
        return NULL;
    }

    /* get the bytes for the input array. */
    jbyte* inputData = 
        (*env)->GetByteArrayElements(env, input, NULL);
    if (NULL == inputData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "inputData");
        return NULL;
    }

    /* get the key field. */
    key =
        (*env)->GetObjectField(
            env, that, native_inst->SimpleStreamCipher.field_key);
    if (NULL == key)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "key");
        goto inputData_dispose;
    }

    /* get the key byte array. */
    keyBytes =
        (*env)->CallObjectMethod(
            env, key, native_inst->Key.getRawBytes);
    if (NULL == keyBytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "keyBytes");
        goto inputData_dispose;
    }

    /* get the raw bytes of this array. */
    keyBytesData = (*env)->GetByteArrayElements(env, keyBytes, NULL);
    if (NULL == keyBytesData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "keyBytesData");
        goto inputData_dispose;
    }

    /* create a vccrypt_buffer for managing these key bytes. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &keyBuffer, &native_inst->alloc_opts,
                    native_inst->crypto_suite.stream_cipher_opts.key_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "key buffer creation failure.");
        goto keyBytesData_dispose;
    }

    /* copy the key data to the key buffer. */
    MODEL_EXEMPT(
        memcpy(keyBuffer.data, keyBytesData,
               native_inst->crypto_suite.stream_cipher_opts.key_size));

    /* Compute the envelope size. */
    size_t envelope_size = KEY_IV_SIZE + KEY_SIZE + IV_SIZE + MAC_SIZE;

    /* Get the input size. */
    size_t input_size = (*env)->GetArrayLength(env, input);

    /* verify that this message is large enough. */
    if (input_size < envelope_size)
    {
        (*env)->ThrowNew(
            env, native_inst->MessageAuthenticationException.classid,
            "bad message size.");
        goto keyBuffer_dispose;
    }

    /* get the plaintext size. */
    size_t plaintext_size = input_size - envelope_size;

    /* create an output array large enough for the plaintext. */
    jbyteArray outputArray = (*env)->NewByteArray(env, plaintext_size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "bad outputArray alloc.");
        goto keyBuffer_dispose;
    }

    /* get output array data. */
    jbyte* outputArrayData =
        (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "outputArray data could not be read.");
        goto keyBuffer_dispose;
    }

    /* create a buffer for the session key. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &sessionKeyBuffer, &native_inst->alloc_opts,
                    native_inst->crypto_suite.stream_cipher_opts.key_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "session key buffer create failure.");
        goto outputArrayData_dispose;
    }

    /* create block cipher instance from the crypto suite. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_block_init(
                    &native_inst->crypto_suite, &block, &keyBuffer, false))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "block cipher init failure.");
        goto sessionKeyBuffer_dispose;
    }

    /* the in pointer is a convenient way to step through the input array. */
    const uint8_t* in = (const uint8_t*)inputData;

    /* the sessionKeyPtr lets us step through the session key array. */
    uint8_t* sessionKeyPtr = (uint8_t*)sessionKeyBuffer.data;

    /* decrypt the key, writing the data to the sessionKeyPtr. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_block_decrypt(&block, in, in + 16, sessionKeyPtr)
     || VCCRYPT_STATUS_SUCCESS !=
            vccrypt_block_decrypt(&block, in + 16, in + 32, sessionKeyPtr + 16))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "block cipher decrypt failure.");
        goto block_dispose;
    }

    /* create the HMAC instance for this. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_mac_init(
                    &native_inst->crypto_suite, &mac, &sessionKeyBuffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "mac init failure.");
        goto block_dispose;
    }

    /* digest the input, less the HMAC. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_mac_digest(&mac, in, input_size - MAC_SIZE))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "message digest failure.");
        goto mac_dispose;
    }

    /* create buffer for receiving the MAC. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_mac_authentication_code(
                    &native_inst->crypto_suite, &macBuffer, false))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "mac buffer init failure.");
        goto mac_dispose;
    }

    /* finalize the mac. */
    if (VCCRYPT_STATUS_SUCCESS != vccrypt_mac_finalize(&mac, &macBuffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "mac finalize failure.");
        goto macBuffer_dispose;
    }

    /* verify the mac against the mac data in the input buffer. */
    if (0 !=
            crypto_memcmp(macBuffer.data, in + input_size - MAC_SIZE, MAC_SIZE))
    {
        (*env)->ThrowNew(
            env, native_inst->MessageAuthenticationException.classid,
            "bad message.");
        goto macBuffer_dispose;
    }

    /* create the stream cipher. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_stream_init(
                    &native_inst->crypto_suite, &stream, &sessionKeyBuffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "stream cipher init failure.");
        goto macBuffer_dispose;
    }

    /* increment the input pointer to the start of the ciphertext IV. */
    in += KEY_IV_SIZE + KEY_SIZE;

    /* start the stream cipher. */
    size_t offset = 0;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_start_decryption(
                &stream, in, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "stream cipher start decryption failure.");
        goto stream_dispose;
    }

    /* decrypt the ciphertext. */
    offset = 0;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_decrypt(
                &stream, in + 8, plaintext_size, outputArrayData, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "stream cipher decryption failure.");
        goto stream_dispose;
    }

    /* success. */
    retval = outputArray;

stream_dispose:
    dispose((disposable_t*)&stream);

macBuffer_dispose:
    dispose((disposable_t*)&macBuffer);

mac_dispose:
    dispose((disposable_t*)&mac);

block_dispose:
    dispose((disposable_t*)&block);

sessionKeyBuffer_dispose:
    dispose((disposable_t*)&sessionKeyBuffer);

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
