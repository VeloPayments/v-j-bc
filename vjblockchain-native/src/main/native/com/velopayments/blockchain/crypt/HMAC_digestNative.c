/**
 * \file HMAC_digestNative.c
 *
 * HMAC the message using key
 *
 * \copyright 2018-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/mac.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../init/init.h"

#include <stdio.h>

/*
 * Class:     com_velopayments_blockchain_crypt_HMAC
 * Method:    digestNative
 * Signature: ([B[BZ)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_crypt_HMAC_digestNative(
        JNIEnv *env, jobject UNUSED(hmac), jbyteArray key,
        jbyteArray message, jboolean hmacShort)
{
    jbyteArray retval = NULL;

    jbyte* key_bytes = NULL;
    vccrypt_buffer_t key_buffer;
    vccrypt_mac_context_t mac;
    vccrypt_buffer_t mac_buffer;
    jbyte* message_bytes = NULL;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(NULL != message);

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

    /* verify that the message parameter is not null. */
    if (NULL == message)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "message");
        return NULL;
    }

    /* get the raw bytes of the key array. */
    key_bytes = (*env)->GetByteArrayElements(env, key, NULL);
    if (NULL == key_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "key_bytes");
        goto done;
    }

    /* create a vccrypt_buffer for managing these key bytes. */
    size_t key_size = (*env)->GetArrayLength(env, key);
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &key_buffer, &native_inst->alloc_opts, key_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "key buffer creation failure.");
        goto key_bytes_dispose;
    }

    /* copy the key data to the key buffer. */
    MODEL_EXEMPT(
        memcpy(key_buffer.data, key_bytes, key_size));

    /* initialize HMAC */
    if (hmacShort)
    {
        if (VCCRYPT_STATUS_SUCCESS !=
                    vccrypt_suite_mac_short_init(
                            &native_inst->crypto_suite, &mac, &key_buffer))
        {
            (*env)->ThrowNew(
                env, native_inst->IllegalStateException.classid,
                "could not initialize short mac.");
            goto key_buffer_dispose;
        }
    }
    else
    {
        if (VCCRYPT_STATUS_SUCCESS !=
                    vccrypt_suite_mac_init(
                            &native_inst->crypto_suite, &mac, &key_buffer))
        {
            (*env)->ThrowNew(
                env, native_inst->IllegalStateException.classid,
                "could not initialize mac.");
            goto key_buffer_dispose;
        }
    }

    /* get the raw bytes of the  message */
    message_bytes = (*env)->GetByteArrayElements(env, message, NULL);
    if (NULL == message_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "message_bytes");
        goto key_buffer_dispose;
    }

    /* digest */
    size_t message_size = (*env)->GetArrayLength(env, message);
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_mac_digest(&mac, (uint8_t*)message_bytes, message_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not digest.");
        goto message_bytes_dispose;
    }

    /* create a buffer to receive the MAC */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_mac_authentication_code(
                    &native_inst->crypto_suite,&mac_buffer,(bool)hmacShort))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not finalize.");
        goto message_bytes_dispose;
    }

    /* finalize */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_mac_finalize(&mac,&mac_buffer))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not finalize.");
        goto mac_buffer_dispose;
    }

    /* create a byte array for the return value */
    retval = (*env)->NewByteArray(env, mac_buffer.size);
    if (NULL == retval)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "return value array could not be allocated.");
        goto mac_buffer_dispose;
    }

    /* get the buffer of this array. */
    jbyte* retval_bytes = (*env)->GetByteArrayElements(env, retval, NULL);
    if (NULL == retval_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "return value array data could not be dereferenced.");
        goto mac_buffer_dispose;
    }

    /* copy the data to the buffer. */
    MODEL_EXEMPT(
            memcpy(retval_bytes, mac_buffer.data, mac_buffer.size));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, retval, retval_bytes, 0);

    /* clean up resources and return */

mac_buffer_dispose:
    dispose((disposable_t*)&mac_buffer);

message_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, message, message_bytes, 0);

key_buffer_dispose:
    dispose((disposable_t*)&key_buffer);

key_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, key, key_bytes, 0);


done:

    return retval;
}
