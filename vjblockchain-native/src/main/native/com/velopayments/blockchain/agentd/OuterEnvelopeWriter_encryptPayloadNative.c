/**
 * \file OuterEnvelopeWriter_encryptPayload.c
 *
 * Encrypt the payload for an authenticated request.
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <arpa/inet.h>

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/mac.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_agentd_OuterEnvelopeWriter
 * Method:    encryptPayloadNative
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL 
Java_com_velopayments_blockchain_agentd_OuterEnvelopeWriter_encryptPayloadNative(
        JNIEnv* env, jobject UNUSED(outer_env), jlong nativeInst,
        jbyteArray shared_secret, jlong iv, jbyteArray payload)
{
    jbyteArray retval = NULL;
    jbyte* shared_secret_bytes;
    vccrypt_buffer_t shared_secret_buffer;
    jbyte* payload_bytes;
    vccrypt_buffer_t payload_buffer;

    uint8_t type = 0x30; // IPC_DATA_TYPE_AUTHED_PACKET
    size_t payload_size = (*env)->GetArrayLength(env, payload);

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);
    MODEL_ASSERT(NULL != shared_secret);
    MODEL_ASSERT(NULL != payload);

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

    /* verify that the shared_secret parameter is not null. */
    if (NULL == shared_secret)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "shared_secret");
        return NULL;
    }

    /* verify that the payload parameter is not null. */
    if (NULL == payload)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "payload");
        return NULL;
    }

    /* create a buffer for holding the digest */
    const size_t MAC_SIZE    =  32U;
    vccrypt_buffer_t digest;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &digest, &native_inst->alloc_opts, MAC_SIZE))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "digest buffer create failure.");
        goto done;
    }

    /* create a packet buffer */
    uint32_t nsize = htonl(payload_size);
    size_t packet_size =
        sizeof(type) + sizeof(nsize) + digest.size + payload_size;

    vccrypt_buffer_t packet;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &packet, &native_inst->alloc_opts, packet_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "packet buffer create failure.");
        goto digest_dispose;
    }

    /* get the raw bytes of the shared secret */
    shared_secret_bytes = (*env)->GetByteArrayElements(
            env, shared_secret, NULL);
    if (NULL == shared_secret_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "shared_secret_bytes");
        goto packet_dispose;
    }

    /* create a buffer to hold the shared secret */
    size_t shared_secret_size = (*env)->GetArrayLength(env, shared_secret);
    if (VCCRYPT_STATUS_SUCCESS != 
            vccrypt_buffer_init(
                    &shared_secret_buffer, &native_inst->alloc_opts, 
                    shared_secret_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "shared secret buffer create failure.");
        goto shared_secret_bytes_dispose;
    }

    /* copy the shared secret to the buffer. */
    MODEL_EXEMPT(
        memcpy(
            shared_secret_buffer.data, shared_secret_bytes,
            shared_secret_size));

    /* create a stream cipher */
    vccrypt_stream_context_t stream;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_stream_init(
                    &native_inst->crypto_suite, &stream, &shared_secret_buffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "stream context failure.");
        goto shared_secret_buffer_dispose;
    }

    /* create a mac instance for building the packet authentication code. */
    vccrypt_mac_context_t mac;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_mac_short_init(
                    &native_inst->crypto_suite, &mac, &shared_secret_buffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "mac init failure.");
        goto stream_cipher_dispose;
    }

    /* start the stream cipher. */
    uint64_t client_iv = (uint64_t)iv;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_continue_encryption(&stream, &client_iv, 
                sizeof(client_iv), 0))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "continue_encryption failure");
        goto mac_dispose;
    }

    /* treat the packet as a byte array for convenience. */
    uint8_t* bpacket = (uint8_t*)packet.data;
    size_t offset = 0;

    /* encrypt the type. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_encrypt(
                &stream, &type, sizeof(type), bpacket, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid, 
            "encryption failure - type");
        goto mac_dispose;
    }

    /* encrypt the size. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_encrypt(
                &stream, &nsize, sizeof(nsize), bpacket, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid, 
            "encryption failure - size");
        goto mac_dispose;
    }

    /* get the raw bytes of the payload */
    payload_bytes = (*env)->GetByteArrayElements(
            env, payload, NULL);
    if (NULL == payload_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "payload_bytes");
        goto mac_dispose;
    }

    /* create a buffer to hold the payload */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &payload_buffer, &native_inst->alloc_opts, payload_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "payload buffer create failure.");
        goto payload_bytes_dispose;
    }

    /* copy the payload to the buffer. */
    MODEL_EXEMPT(
        memcpy(payload_buffer.data, payload_bytes, payload_size));

    /* encrypt the payload. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_stream_encrypt(
                &stream, payload_buffer.data, payload_size, 
                bpacket + digest.size, &offset))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid, 
            "encryption failure - payload");
        goto payload_buffer_dispose;
    }

    /* digest the packet header and payload. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_mac_digest(
                &mac, bpacket, sizeof(type) + sizeof(nsize))
     || VCCRYPT_STATUS_SUCCESS !=
            vccrypt_mac_digest(
                &mac, bpacket + sizeof(type) + sizeof(nsize) + digest.size,
                payload_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "digest failure");
        goto payload_buffer_dispose;
    }

    /* finalize the digest. */
    if (VCCRYPT_STATUS_SUCCESS != vccrypt_mac_finalize(&mac, &digest))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "finalize failure");
        goto payload_buffer_dispose;
    }

    /* copy the digest to the packet. */
    MODEL_EXEMPT(
        memcpy(
            bpacket + sizeof(type) + sizeof(nsize), digest.data, digest.size));

    /* create a byte array for the return value */
    retval = (*env)->NewByteArray(env, packet.size);
    if (NULL == retval)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "return value array could not be allocated.");
        goto payload_buffer_dispose;
    }

    /* get the buffer of this array. */
    jbyte* retval_bytes = (*env)->GetByteArrayElements(env, retval, NULL);
    if (NULL == retval_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "return value array data could not be dereferenced.");
        goto payload_buffer_dispose;
    }

    /* copy the data to the buffer. */
    MODEL_EXEMPT(
            memcpy(retval_bytes, packet.data, packet.size));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, retval, retval_bytes, 0);

    /* clean up resources and return */
payload_buffer_dispose:
    dispose((disposable_t*)&payload_buffer);

payload_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, payload, payload_bytes, 0);

mac_dispose:
    dispose((disposable_t*)&mac);

stream_cipher_dispose:
    dispose((disposable_t*)&stream);

shared_secret_buffer_dispose:
    dispose((disposable_t*)&shared_secret_buffer);

shared_secret_bytes_dispose:
    (*env)->ReleaseByteArrayElements(
            env, shared_secret, shared_secret_bytes, 0);

packet_dispose:
    dispose((disposable_t*)&packet);

digest_dispose:
    dispose((disposable_t*)&digest);

done:
    return retval;
}
