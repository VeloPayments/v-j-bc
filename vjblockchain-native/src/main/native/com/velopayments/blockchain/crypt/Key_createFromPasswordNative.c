/**
 * \file Key_createFromPasswordNative.c
 *
 * Generate a key from a password using the Velo crypto suite.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/key_derivation.h>
#include <vccrypt/mac.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "Key.h"
#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"


/*
 * Class:     com_velopayments_blockchain_crypt_Key
 * Method:    createFromPasswordNative
 * Signature: ([B[BIZ)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_crypt_Key_createFromPasswordNative(
        JNIEnv *env, jclass UNUSED(clazz), jbyteArray password, jbyteArray salt,
        jint iterations, jboolean sha512)
{
    jobject retval = NULL;

    vccrypt_buffer_t key_buffer;
    vccrypt_key_derivation_context_t kd_ctx;
    vccrypt_key_derivation_options_t kd_options;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
                env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /*
     * create key derivation options
     *
     * Note: this is done outside of the crypto suite here, because the
     * crypto suite uses SHA 512/256 as a default, which isn't available
     * in the java.crypto packages.  Once we have that HMAC available via
     * Java it won't be necessary to allow the use of HMACs other than
     * what is available via the crypto suite.
     */
    int hmac_alg = sha512 ?
            VCCRYPT_MAC_ALGORITHM_SHA_2_512_HMAC :
            VCCRYPT_MAC_ALGORITHM_SHA_2_512_256_HMAC;

    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_key_derivation_options_init(
                &kd_options, &alloc_opts,
                VCCRYPT_KEY_DERIVATION_ALGORITHM_PBKDF2,
                hmac_alg))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key derivation options initialization failure.");
        return NULL;
    }

    /* initialize key derivation instance */
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_key_derivation_init(&kd_options, &kd_ctx))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key derivation instance initialization failure.");
        goto kd_options_dispose;
    }

    /* get the buffer for the password bytes */
    jbyte* password_array = (*env)->GetByteArrayElements(env, password, NULL);
    if (NULL == password_array)
    {
        (*env)->ThrowNew(env, NullPointerException, "password read failure.");
        goto kd_dispose;
    }

    /* get the buffer for the salt bytes */
    jbyte* salt_array = (*env)->GetByteArrayElements(env, salt, NULL);
    if (NULL == salt_array)
    {
        (*env)->ThrowNew(env, NullPointerException, "salt data read failure.");
        goto password_dispose;
    }

    /* initialize key buffer. */
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_buffer_init(
                &key_buffer, &alloc_opts, kd_options.hmac_digest_length))
    {
        (*env)->ThrowNew(
                env, IllegalStateException,
                "could not initialize a crypto buffer.");
        goto salt_dispose;
    }

    /* derive the cryptographic key */
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_key_derivation_derive_key(&kd_ctx,
                (char*)password_array, (*env)->GetArrayLength(env, password),
                (uint8_t*)salt_array, (*env)->GetArrayLength(env, salt),
                key_buffer.data, key_buffer.size,
                iterations))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                "key derivation failure.");
        goto key_buffer_dispose;
    }

    /* create the key array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, key_buffer.size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "outputArray creation failure.");
        goto key_buffer_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
            (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "outputArray data read failure.");
        goto key_buffer_dispose;
    }

    /* write to the output array. */
    MODEL_EXEMPT(
            memcpy(outputArrayData, key_buffer.data, key_buffer.size));

    /* success - set the output array to the return value. */
    retval = outputArray;

    /* fall through */
    (*env)->ReleaseByteArrayElements(env, outputArray, outputArrayData, 0);

key_buffer_dispose:
    dispose((disposable_t*)&key_buffer);

salt_dispose:
    (*env)->ReleaseByteArrayElements(env, salt, salt_array, 0);

password_dispose:
    (*env)->ReleaseByteArrayElements(env, password, password_array, 0);

kd_dispose:
    dispose((disposable_t*)&kd_ctx);

kd_options_dispose:
    dispose((disposable_t*)&kd_options);

    return retval;
}
