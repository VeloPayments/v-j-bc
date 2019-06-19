/**
 * \file OuterEnvelopeReader_decryptPayloadSize.c
 *
 * Decrypt the payload size from an encrypted response
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
 * Method:    decryptPayloadSize
 * Signature: ([B[B)I
 */
JNIEXPORT jint JNICALL 
Java_com_velopayments_blockchain_agentd_OuterEnvelopeReader_decryptPayloadSize(
        JNIEnv* env, jobject UNUSED(outer_env_reader), 
        jbyteArray shared_secret, jbyteArray header)
{
    jint retval = -1;
    jbyte* shared_secret_bytes;
    vccrypt_buffer_t shared_secret_buffer;
    jbyte* header_bytes;
    vccrypt_buffer_t header_buffer;


    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != shared_secret);
    MODEL_ASSERT(NULL != header);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
                env, IllegalStateException, "vjblockchain not initialized.");
        goto done;
    }

    /* verify that the shared_secret parameter is not null. */
    if (NULL == shared_secret)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "shared_secret");
        goto done;
    }

    /* verify that the header parameter is not null. */
    if (NULL == header)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "header");
        goto done;
    }

    /* validate the header size.  The header should be 5 bytes. */
    size_t header_size = (*env)->GetArrayLength(env, header);
    if (header_size != 5)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "invalid header size");
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
        memcpy(shared_secret_buffer.data, shared_secret_bytes, shared_secret_size));

    /* get the raw bytes of the header */
    header_bytes = (*env)->GetByteArrayElements(
            env, header, NULL);
    if (NULL == header_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "header_bytes");
        goto shared_secret_buffer_dispose;
    }

    /* create a buffer to hold the header */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(&header_buffer, &alloc_opts, header_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "header buffer create failure.");
        goto header_bytes_dispose;
    }

    /* copy the header to the buffer. */
    MODEL_EXEMPT(
        memcpy(header_buffer.data, header_bytes, header_size));

    /* attempt to allocate space for the decrypted header. */
    vccrypt_buffer_t dheader_buffer;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(&dheader_buffer, &alloc_opts, header_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "decrypted header buffer create failure.");
        goto header_buffer_dispose;
    }

    /* create a pointer for convenience */
    uint8_t* dheader = (uint8_t*)dheader_buffer.data;

    /* create a stream cipher */
    vccrypt_stream_context_t stream;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_stream_init(&crypto_suite, &stream, &shared_secret_buffer))
    {
        (*env)->ThrowNew(env, IllegalStateException, "stream context failure.");
        goto dheader_buffer_dispose;
    }

    /* start decryption of the stream. */
    uint64_t server_iv = 0x8000000000000001;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_continue_decryption(&stream, &server_iv, 
                sizeof(server_iv), 0))
    {
        (*env)->ThrowNew(env, IllegalStateException, "continue_decryption failure");
        goto stream_cipher_dispose;
    }

    /* decrypt enough of the header to determine the type and size. */
    size_t offset = 0;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_decrypt(
                &stream, header_buffer.data, header_size, dheader_buffer.data, 
                &offset))
    {
        (*env)->ThrowNew(env, IllegalStateException, "decryption failure");
        goto stream_cipher_dispose;
    }

    /* verify that the type is IPC_DATA_TYPE_AUTHED_PACKET. */
    uint8_t IPC_DATA_TYPE_AUTHED_PACKET = 0x30; 
    if (IPC_DATA_TYPE_AUTHED_PACKET != dheader[0])
    {
        (*env)->ThrowNew(env, IllegalStateException, "invalid packet type");
        goto stream_cipher_dispose;
    }

    uint32_t nsize;
    memcpy(&nsize, dheader + 1, sizeof(nsize));
    uint32_t payload_size = ntohl(nsize);
    if (payload_size > 10ULL * 1024ULL * 1024ULL /* 10 MB */)
    {
        (*env)->ThrowNew(env, IllegalStateException, "invalid packet size");
        goto stream_cipher_dispose;
    }

    retval = payload_size;

    /* clean up resources and return */


stream_cipher_dispose:
    dispose((disposable_t*)&stream);

dheader_buffer_dispose:
    dispose((disposable_t*)&dheader_buffer);

header_buffer_dispose:
    dispose((disposable_t*)&header_buffer);

header_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, header, header_bytes, 0);

shared_secret_buffer_dispose:
    dispose((disposable_t*)&shared_secret_buffer);

shared_secret_bytes_dispose:
    (*env)->ReleaseByteArrayElements(
            env, shared_secret, shared_secret_bytes, 0);

done:
    return retval;
}
