/**
 * \file SigningPrivateKey_signNative.c
 *
 * Sign a Message using a private key.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_SigningPublicKey
 * Method:    verifyNative
 * Signature: (Lcom/velopayments/blockchain/crypt/Signature;Lcom/velopayments/blockchain/crypt/Message;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_velopayments_blockchain_crypt_SigningPublicKey_verifyNative(
    JNIEnv* env, jobject that, jobject signature, jobject message)
{
    jboolean retval = JNI_FALSE;

    /* function contract enforcement. */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != signature);
    MODEL_ASSERT(NULL != message);

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vjblockchain not initialized.");
        goto done;
    }
 
    /* verify that the signature parameter is not null. */
    if (NULL == signature)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "signature");
        goto done;
    }

    /* verify that the message parameter is not null. */
    if (NULL == message)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid, "message");
        goto done;
    }

    /* get the raw bytes for this public key. */
    jobject public_key =
        (*env)->CallObjectMethod(
            env, that, native_inst->SigningPublicKey.getRawBytes);
    if (NULL == public_key)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "this.getRawBytes()");
        goto done;
    }

    /* get the size of this public key. */
    jsize public_key_size = (*env)->GetArrayLength(env, public_key);
    if ((size_t)public_key_size !=
                native_inst->crypto_suite.sign_opts.public_key_size)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "public_key_size");
        goto cleanup_public_key;
    }

    /* create key buffer. */
    vccrypt_buffer_t pubkey;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_signature_public_key(
                &native_inst->crypto_suite, &pubkey))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid, "pubkey");
        goto cleanup_public_key;
    }

    /* get public key bytes. */
    jbyte* public_key_bytes =
        (*env)->GetByteArrayElements(env, public_key, NULL);
    if (NULL == public_key_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "public_key_bytes");
        goto cleanup_pubkey;
    }

    /* copy key data into pubkey. */
    memcpy(pubkey.data, public_key_bytes, pubkey.size);

    /* create the signature context. */
    vccrypt_digital_signature_context_t sign;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_digital_signature_init(
                    &native_inst->crypto_suite, &sign))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "signature_init");
        goto cleanup_public_key_bytes;
    }

    /* get the raw array for the message. */
    jobject message_array =
        (*env)->CallObjectMethod(
            env, message, native_inst->Message.getRawBytes);
    if (NULL == message_array)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "message.getRawBytes()");
        goto cleanup_sign;
    }

    /* get the size of the message. */
    jsize message_size = (*env)->GetArrayLength(env, message_array);

    /* get the message bytes. */
    jbyte* message_bytes =
        (*env)->GetByteArrayElements(env, message_array, NULL);
    if (NULL == message_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "message_bytes");
        goto cleanup_message_array;
    }

    /* create the signature buffer. */
    vccrypt_buffer_t sign_buf;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_signature(
                    &native_inst->crypto_suite, &sign_buf))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid, "sign_buf");
        goto cleanup_message_bytes;
    }

    /* get the byte array for the signature. */
    jobject signature_array =
        (*env)->CallObjectMethod(
            env, signature, native_inst->Signature.getSignatureBytes);
    if (NULL == signature_array)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "signature.getSignatureBytes()");
        goto cleanup_sign_buf;
    }

    /* get the size of this signature. */
    jsize signature_array_size = (*env)->GetArrayLength(env, signature_array);
    if ((size_t)signature_array_size !=
                native_inst->crypto_suite.sign_opts.signature_size)
    {
        /* the signature obviously can't match if there is a size mismatch. */
        retval = JNI_FALSE;
        goto cleanup_signature_array;
    }

    /* get the signature bytes. */
    jbyte* signature_array_bytes =
        (*env)->GetByteArrayElements(env, signature_array, NULL);
    if (NULL == signature_array_bytes)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "signature_array_bytes");
        goto cleanup_signature_array;
    }

    /* copy signature data into sign_buf. */
    memcpy(sign_buf.data, signature_array_bytes, sign_buf.size);

    /* verify the signature. */
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_digital_signature_verify(
                &sign, &sign_buf, &pubkey, (uint8_t*)message_bytes,
                message_size))
    {
        /* signature verification failed. */
        retval = JNI_FALSE;
        goto cleanup_signature_array_bytes;
    }

    /* verification success. */
    retval = JNI_TRUE;

    /* fall-through. */

cleanup_signature_array_bytes:
    (*env)->ReleaseByteArrayElements(
        env, signature_array, signature_array_bytes, JNI_ABORT);

cleanup_signature_array:
    (*env)->DeleteLocalRef(env, signature_array);

cleanup_sign_buf:
    dispose((disposable_t*)&sign_buf);

cleanup_message_bytes:
    (*env)->ReleaseByteArrayElements(
        env, message_array, message_bytes, JNI_ABORT);

cleanup_message_array:
    (*env)->DeleteLocalRef(env, message_array);

cleanup_sign:
    dispose((disposable_t*)&sign);

cleanup_public_key_bytes:
    (*env)->ReleaseByteArrayElements(
        env, public_key, public_key_bytes, JNI_ABORT);

cleanup_pubkey:
    dispose((disposable_t*)&pubkey);

cleanup_public_key:
    (*env)->DeleteLocalRef(env, public_key);

done:
    return retval;
}
