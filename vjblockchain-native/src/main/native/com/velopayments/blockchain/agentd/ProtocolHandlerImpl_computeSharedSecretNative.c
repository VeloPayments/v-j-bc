/**
 * \file ProtocolHandlerImpl_computeSharedSecretNative.c
 *
 * Compute the shared secret for the handshake request.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

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
 * Class:     com_velopayments_blockchain_agentd_ProtocolHandlerImpl
 * Method:    computeSharedSecretNative
 * Signature: ([B[B[B[B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_agentd_ProtocolHandlerImpl_computeSharedSecretNative(
        JNIEnv* env, jclass UNUSED(proto_handler), jbyteArray client_private_key,
        jbyteArray server_public_key, jbyteArray server_nonce,
        jbyteArray client_nonce)
{
    jbyteArray retval = NULL;
    jbyte* client_private_key_bytes;
    vccrypt_buffer_t client_private_key_buffer;
    jbyte* server_public_key_bytes;
    vccrypt_buffer_t server_public_key_buffer;
    jbyte* server_nonce_bytes;
    vccrypt_buffer_t server_nonce_buffer;
    jbyte* client_nonce_bytes;
    vccrypt_buffer_t client_nonce_buffer;
    vccrypt_buffer_t shared_secret_buffer;
    vccrypt_key_agreement_context_t agreement;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != client_private_key);
    MODEL_ASSERT(NULL != server_public_key);
    MODEL_ASSERT(NULL != server_nonce);
    MODEL_ASSERT(NULL != client_nonce);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
                env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the client_private_key parameter is not null. */
    if (NULL == client_private_key)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "client_private_key");
        return NULL;
    }

    /* verify that the server_public_key parameter is not null. */
    if (NULL == server_public_key)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "server_public_key");
        return NULL;
    }

    /* verify that the server_nonce parameter is not null. */
    if (NULL == server_nonce)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "server_nonce");
        return NULL;
    }

    /* verify that the client_nonce parameter is not null. */
    if (NULL == client_nonce)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "client_nonce");
        return NULL;
    }

    /* get the raw bytes of the client private key */
    client_private_key_bytes = (*env)->GetByteArrayElements(
            env, client_private_key, NULL);
    if (NULL == client_private_key_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "client_private_key_bytes");
        goto done;
    }

    /* create a vccrypt_buffer for managing the client private key */
    size_t client_private_key_size = (*env)->GetArrayLength(
            env, client_private_key);
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_buffer_init(
                &client_private_key_buffer, &alloc_opts,
                client_private_key_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "client private key buffer creation failure.");
        goto client_private_key_bytes_dispose;
    }

    /* copy the client private key data to the buffer. */
    memcpy(client_private_key_buffer.data, client_private_key_bytes,
            client_private_key_size);


    /* get the raw bytes of the server public key */
    server_public_key_bytes = (*env)->GetByteArrayElements(
            env, server_public_key, NULL);
    if (NULL == server_public_key_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "server_public_key_bytes");
        goto client_private_key_buffer_dispose;
    }

    /* create a vccrypt_buffer for managing the server public key */
    size_t server_public_key_size = (*env)->GetArrayLength(
            env, server_public_key);
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_buffer_init(
                &server_public_key_buffer, &alloc_opts,
                server_public_key_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "server public key buffer creation failure.");
        goto server_public_key_bytes_dispose;
    }

    /* copy the server public key data to the buffer. */
    memcpy(server_public_key_buffer.data, server_public_key_bytes,
           server_public_key_size);

    /* get the raw bytes of the server nonce */
    server_nonce_bytes = (*env)->GetByteArrayElements(env, server_nonce, NULL);
    if (NULL == server_nonce_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "server_nonce_bytes");
        goto server_public_key_buffer_dispose;
    }

    /* create a vccrypt_buffer for managing the server nonce */
    size_t server_nonce_size = (*env)->GetArrayLength(
            env, server_nonce);
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_buffer_init(
                &server_nonce_buffer, &alloc_opts,
                server_nonce_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "server nonce buffer creation failure.");
        goto server_nonce_bytes_dispose;
    }

    /* copy the server nonce data to the buffer. */
    memcpy(server_nonce_buffer.data, server_nonce_bytes, server_nonce_size);

    /* get the raw bytes of the client nonce */
    client_nonce_bytes = (*env)->GetByteArrayElements(env, client_nonce, NULL);
    if (NULL == client_nonce_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "client_nonce_bytes");
        goto server_nonce_buffer_dispose;
    }

    /* create a vccrypt_buffer for managing the client nonce */
    size_t client_nonce_size = (*env)->GetArrayLength(
            env, client_nonce);
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_buffer_init(
                &client_nonce_buffer, &alloc_opts,
                client_nonce_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "client nonce buffer creation failure.");
        goto client_nonce_bytes_dispose;
    }

    /* copy the client nonce data to the buffer. */
    memcpy(client_nonce_buffer.data, client_nonce_bytes, client_nonce_size);

    /* create buffer for shared secret */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_shared_secret(
            &crypto_suite, &shared_secret_buffer))
    {
        goto client_nonce_buffer_dispose;
    }

    /* create the key agreement instance. */
    if (0 != vccrypt_suite_cipher_key_agreement_init(
                    &crypto_suite, &agreement))
    {
        goto shared_secret_buffer_dispose;
    }

    /* compute the shared secret */
    if (0 != vccrypt_key_agreement_short_term_secret_create(
            &agreement, &client_private_key_buffer,
            &server_public_key_buffer, &server_nonce_buffer,
            &client_nonce_buffer, &shared_secret_buffer))
    {
        goto key_agreement_dispose;
    }

    /* create a byte array for the return value */
    retval = (*env)->NewByteArray(env, shared_secret_buffer.size);
    if (NULL == retval)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "return value array could not be allocated.");

        goto key_agreement_dispose;
    }

    /* get the buffer of this array. */
    jbyte* retval_bytes = (*env)->GetByteArrayElements(env, retval, NULL);
    if (NULL == retval_bytes)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "return value array data could not be dereferenced.");

        goto key_agreement_dispose;
    }

    /* copy the data to the buffer. */
    MODEL_EXEMPT(
            memcpy(retval_bytes, shared_secret_buffer.data,
                    shared_secret_buffer.size));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, retval, retval_bytes, 0);

    /* clean up resources and return */

key_agreement_dispose:
    dispose((disposable_t*)&agreement);

shared_secret_buffer_dispose:
    dispose((disposable_t*)&shared_secret_buffer);

client_nonce_buffer_dispose:
    dispose((disposable_t*)&client_nonce_buffer);

client_nonce_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, client_nonce, client_nonce_bytes, 0);

server_nonce_buffer_dispose:
    dispose((disposable_t*)&server_nonce_buffer);

server_nonce_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, server_nonce, server_nonce_bytes, 0);

server_public_key_buffer_dispose:
    dispose((disposable_t*)&server_public_key_buffer);

server_public_key_bytes_dispose:
    (*env)->ReleaseByteArrayElements(
            env, server_public_key, server_public_key_bytes, 0);

client_private_key_buffer_dispose:
    dispose((disposable_t*)&client_private_key_buffer);

client_private_key_bytes_dispose:
    (*env)->ReleaseByteArrayElements(
            env, client_private_key, client_private_key_bytes, 0);


done:

    return retval;
}
