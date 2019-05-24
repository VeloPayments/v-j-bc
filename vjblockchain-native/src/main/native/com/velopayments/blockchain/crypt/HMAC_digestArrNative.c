/**
 * \file HMAC_digestArrNative.c
 *
 * HMAC the message using key
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

#include <stdio.h>

/*
 * Class:     com_velopayments_blockchain_crypt_HMAC
 * Method:    digestArrNative
 * Signature: ([B[[BZ)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_crypt_HMAC_digestArrNative(
        JNIEnv *env, jobject UNUSED(hmac), jbyteArray key,
        jobjectArray messages, jboolean hmacShort)
{
    jbyteArray retval = NULL;

    jbyte* key_bytes = NULL;
    vccrypt_buffer_t key_buffer;
    vccrypt_mac_context_t mac;
    vccrypt_buffer_t mac_buffer;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(NULL != messages);

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

    /* verify that the message parameter is not null. */
    if (NULL == messages)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "messages");
        return NULL;
    }

    /* get the raw bytes of the key array. */
    key_bytes = (*env)->GetByteArrayElements(env, key, NULL);
    if (NULL == key_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "key_bytes");
        goto done;
    }

    /* create a vccrypt_buffer for managing these key bytes. */
    size_t key_size = (*env)->GetArrayLength(env, key);
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_buffer_init(
                &key_buffer, &alloc_opts, key_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key buffer creation failure.");
        goto key_bytes_dispose;
    }

    /* copy the key data to the key buffer. */
    memcpy(key_buffer.data, key_bytes, key_size);

    /* initialize HMAC */
    if (VCCRYPT_STATUS_SUCCESS !=
        hmacShort ?
        vccrypt_suite_mac_short_init(&crypto_suite, &mac, &key_buffer)
                  : vccrypt_suite_mac_init(&crypto_suite, &mac, &key_buffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not initialize mac.");
        goto key_buffer_dispose;
    }

    //////////////////////

//    jbyte* message_bytes = NULL;


    /* get the raw bytes of the  message */
    /*message_bytes = (*env)->GetByteArrayElements(env, message, NULL);
    if (NULL == message_bytes)
    {
        (*env)->ThrowNew(
                env, NullPointerException, "message_bytes");
        goto key_buffer_dispose;
    }*/

    /* digest */
    /*size_t message_size = (*env)->GetArrayLength(env, message);
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_mac_digest(&mac, (uint8_t*)message_bytes, message_size))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not digest.");
        goto message_bytes_dispose;
    }*/

    ///////////////////////


    /* create a buffer to receive the MAC */
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_suite_buffer_init_for_mac_authentication_code(
                &crypto_suite,&mac_buffer,(bool)hmacShort))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not finalize.");
//        goto message_bytes_dispose;
        goto key_buffer_dispose;
    }

    /* finalize */
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_mac_finalize(&mac,&mac_buffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "could not finalize.");
        goto mac_buffer_dispose;
    }

    /* create a byte array for the return value */
    retval = (*env)->NewByteArray(env, mac_buffer.size);
    if (NULL == retval)
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "return value array could not be allocated.");

        goto mac_buffer_dispose;
    }

    /* get the buffer of this array. */
    jbyte* retval_bytes = (*env)->GetByteArrayElements(env, retval, NULL);
    if (NULL == retval_bytes)
    {
        (*env)->ThrowNew(env, IllegalStateException,
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

//message_bytes_dispose:
//    (*env)->ReleaseByteArrayElements(env, message, message_bytes, 0);

key_buffer_dispose:
    dispose((disposable_t*)&key_buffer);

key_bytes_dispose:
    (*env)->ReleaseByteArrayElements(env, key, key_bytes, 0);


done:

    return retval;
}
