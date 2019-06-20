/**
 * \file OuterEnvelopeReader_decryptPayloadNative.c
 *
 * Decrypt the from an encrypted response
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <arpa/inet.h>

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/mac.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"
#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_agentd_OuterEnvelopeReader
 * Method:    decryptPayloadNative
 * Signature: ([B[B)[B
 */
JNIEXPORT jbyteArray JNICALL 
Java_com_velopayments_blockchain_agentd_OuterEnvelopeReader_decryptPayloadNative(
        JNIEnv* env, jobject UNUSED(outer_env_reader), 
        jbyteArray shared_secret, jbyteArray payload)
{
    jbyteArray retval = NULL;
    jbyte* shared_secret_bytes;
    vccrypt_buffer_t shared_secret_buffer;
    jbyte* payload_bytes;
    vccrypt_buffer_t payload_buffer;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != shared_secret);
    MODEL_ASSERT(NULL != payload);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
                env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the shared_secret parameter is not null. */
    if (NULL == shared_secret)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "shared_secret");
        goto done;
    }

    /* verify that the payload parameter is not null. */
    if (NULL == payload)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "payload");
        goto done;
    }

    /* get the raw bytes of the shared secret */
    shared_secret_bytes = (*env)->GetByteArrayElements(
            env, shared_secret, NULL);
    if (NULL == shared_secret_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "shared_secret_bytes");
        goto done;
    }

    /* create a buffer to hold the shared secret */
    size_t shared_secret_size = (*env)->GetArrayLength(env, shared_secret);
    if (VCCRYPT_STATUS_SUCCESS != 
            vccrypt_buffer_init(&shared_secret_buffer, &alloc_opts, 
                shared_secret_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "shared secret buffer create failure.");
        goto shared_secret_bytes_dispose;
    }

    /* copy the shared secret to the buffer. */
    MODEL_EXEMPT(
        memcpy(shared_secret_buffer.data, shared_secret_bytes, 
            shared_secret_size));

    /* get the raw bytes of the payload */
    payload_bytes = (*env)->GetByteArrayElements(env, payload, NULL);
    if (NULL == payload_bytes)
    {
        (*env)->ThrowNew(env, NullPointerException, "payload_bytes");
        goto shared_secret_buffer_dispose;
    }

    /* create a buffer to hold the payload */
    size_t payload_size = (*env)->GetArrayLength(env, payload);
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(&payload_buffer, &alloc_opts, payload_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "payload buffer create failure.");
        goto payload_bytes_dispose;
    }

    /* copy the payload to the buffer. */
    MODEL_EXEMPT(
        memcpy(payload_buffer.data, payload_bytes, payload_size));

    /* attempt to allocate space for the decrypted payload. */
    vccrypt_buffer_t dpayload_buffer;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(&dpayload_buffer, &alloc_opts, payload_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "decrypted payload buffer create failure.");
        goto payload_buffer_dispose;
    }

    /* create a stream cipher */
    vccrypt_stream_context_t stream;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_stream_init(&crypto_suite, &stream, 
                &shared_secret_buffer))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "stream context failure.");
        goto dpayload_buffer_dispose;
    }

    /* start decryption of the stream. */
    uint64_t server_iv = 0x8000000000000001;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_continue_decryption(&stream, &server_iv, 
                sizeof(server_iv), 5)) // TODO
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "continue_decryption failure");
        goto stream_cipher_dispose;
    }

    /* decrypt the payload. */
    size_t offset = 0;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_decrypt(
                &stream, payload_buffer.data, payload_size, 
                dpayload_buffer.data, &offset))
    {
        (*env)->ThrowNew(env, IllegalStateException, "decryption failure");
        goto stream_cipher_dispose;
    }

    /* create a byte array for the return value */
    retval = (*env)->NewByteArray(env, dpayload_buffer.size);
    if (NULL == retval)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "return value array could not be allocated.");

        goto stream_cipher_dispose;
    }

    /* get the buffer of this array. */
    jbyte* retval_bytes = (*env)->GetByteArrayElements(env, retval, NULL);
    if (NULL == retval_bytes)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "return value array data could not be dereferenced.");

        goto stream_cipher_dispose;
    }

    /* copy the data to the buffer. */
    MODEL_EXEMPT(
            memcpy(retval_bytes, dpayload_buffer.data,
                    dpayload_buffer.size));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, retval, retval_bytes, 0);

    /* clean up resources and return */

stream_cipher_dispose:
    dispose((disposable_t*)&stream);

dpayload_buffer_dispose:
    dispose((disposable_t*)&dpayload_buffer);

payload_buffer_dispose:
    dispose((disposable_t*)&payload_buffer);

payload_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, payload, payload_bytes, 0);

shared_secret_buffer_dispose:
    dispose((disposable_t*)&shared_secret_buffer);

shared_secret_bytes_dispose:
    (*env)->ReleaseByteArrayElements(
            env, shared_secret, shared_secret_bytes, 0);

done:
    return retval;
}

