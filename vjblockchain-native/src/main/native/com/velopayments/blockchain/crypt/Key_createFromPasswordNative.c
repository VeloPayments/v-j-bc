/**
 * \file Key_createFromPasswordNative.c
 *
 * Generate a key from a password using the Velo crypto suite.
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/key_derivation.h>
#include <vccrypt/mac.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

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

    vccrypt_buffer_t password_buffer;
    vccrypt_buffer_t salt_buffer;
    vccrypt_buffer_t key_buffer;
    vccrypt_key_derivation_context_t kd_ctx;
    vccrypt_key_derivation_options_t kd_options;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vjblockchain not initialized.");
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
                &kd_options, &native_inst->alloc_opts,
                VCCRYPT_KEY_DERIVATION_ALGORITHM_PBKDF2, hmac_alg))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "key derivation options initialization failure.");
        return NULL;
    }

    /* initialize key derivation instance */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_key_derivation_init(&kd_ctx, &kd_options))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "key derivation instance initialization failure.");
        goto kd_options_dispose;
    }

    /* get the buffer for the password bytes */
    jbyte* password_array = (*env)->GetByteArrayElements(env, password, NULL);
    if (NULL == password_array)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "password read failure.");
        goto kd_dispose;
    }

    /* initialize password crypto buffer */
    size_t password_sz = (*env)->GetArrayLength(env, password);
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &password_buffer, &native_inst->alloc_opts, password_sz))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not initialize password crypto buffer.");
        goto password_dispose; 
    }

    /* copy password into password crypto buffer. */
    MODEL_EXEMPT(
            memcpy(password_buffer.data, password_array, password_sz));

    /* get the buffer for the salt bytes */
    jbyte* salt_array = (*env)->GetByteArrayElements(env, salt, NULL);
    if (NULL == salt_array)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "salt data read failure.");
        goto password_buffer_dispose;
    }

    /* initialize salt crypto buffer */
    size_t salt_sz = (*env)->GetArrayLength(env, salt);
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                    &salt_buffer, &native_inst->alloc_opts, salt_sz))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not initialize salt crypto buffer.");
        goto salt_dispose; 
    }

    /* copy salt into salt crypto buffer. */
    MODEL_EXEMPT(
            memcpy(salt_buffer.data, salt_array, salt_sz));

    /* initialize key crypto buffer. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_buffer_init(
                &key_buffer, &native_inst->alloc_opts,
                kd_options.hmac_digest_length))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "could not initialize key crypto buffer.");
        goto salt_buffer_dispose;
    }

    /* derive the cryptographic key */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_key_derivation_derive_key(&key_buffer, &kd_ctx,
                &password_buffer, &salt_buffer, iterations))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "key derivation failure.");
        goto key_buffer_dispose;
    }

    /* create the key array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, key_buffer.size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "outputArray creation failure.");
        goto key_buffer_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
            (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
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

salt_buffer_dispose:
    dispose((disposable_t*)&salt_buffer);

salt_dispose:
    (*env)->ReleaseByteArrayElements(env, salt, salt_array, 0);

password_buffer_dispose:
    dispose((disposable_t*)&password_buffer);

password_dispose:
    (*env)->ReleaseByteArrayElements(env, password, password_array, 0);

kd_dispose:
    dispose((disposable_t*)&kd_ctx);

kd_options_dispose:
    dispose((disposable_t*)&kd_options);

    return retval;
}
