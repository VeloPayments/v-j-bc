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

#include "Message.h"
#include "SigningPrivateKey.h"
#include "Signature.h"
#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"

/*
 * Class:     com_velopayments_blockchain_crypt_SigningPrivateKey
 * Method:    signNative
 * Signature: (Lcom/velopayments/blockchain/crypt/Message;)Lcom/velopayments/blockchain/crypt/Signature;
 */
JNIEXPORT jobject JNICALL Java_com_velopayments_blockchain_crypt_SigningPrivateKey_signNative(
    JNIEnv* env, jobject that, jobject message)
{
    jobject retval = NULL;

    /* function contract enforcement. */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != message);

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }
    
    /* verify that the message parameter is not null. */
    if (NULL == message)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "message");
        return NULL;
    }

    /* get the raw bytes for this private key. */
    jobject private_key =
        (*env)->CallObjectMethod(env, that, SigningPrivateKey_getRawBytes);
    if (NULL == private_key)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "this.getRawBytes()");
        return NULL;
    }

    /* get the size of this private key. */
    jsize private_key_size = (*env)->GetArrayLength(env, private_key);
    if ((size_t)private_key_size !=
            native_inst->crypto_suite.sign_opts.private_key_size)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "private_key_size");
        goto cleanup_private_key;
    }

    /* create key buffer. */
    vccrypt_buffer_t privkey;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_signature_private_key(
                    &native_inst->crypto_suite, &privkey))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "privkey");
        goto cleanup_private_key;
    }

    /* get private key bytes. */
    jbyte* private_key_bytes =
        (*env)->GetByteArrayElements(env, private_key, NULL);
    if (NULL == private_key_bytes)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "private_key_bytes");
        goto cleanup_privkey;
    }

    /* copy key data into privkey. */
    memcpy(privkey.data, private_key_bytes, privkey.size);

    /* create the signature context. */
    vccrypt_digital_signature_context_t sign;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_digital_signature_init(
                    &native_inst->crypto_suite, &sign))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "signature_init");
        goto cleanup_private_key_bytes;
    }

    /* get the raw array for the message. */
    jobject message_array =
        (*env)->CallObjectMethod(env, message, Message_getRawBytes);
    if (NULL == message_array)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "message.getRawBytes()");
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
            env, NullPointerException, "message_bytes");
        goto cleanup_message_array;
    }

    /* create the signature buffer. */
    vccrypt_buffer_t signature;
    if (VCCRYPT_STATUS_SUCCESS !=
            vccrypt_suite_buffer_init_for_signature(
                    &native_inst->crypto_suite, &signature))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "signature");
        goto cleanup_message_bytes;
    }

    /* sign the message using the signature context. */
    if (VCCRYPT_STATUS_SUCCESS !=
        vccrypt_digital_signature_sign(
            &sign, &signature, &privkey, (const uint8_t*)message_bytes,
            message_size))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "signature_sign");
        goto cleanup_signature;
    }

    /* create a byte buffer. */
    jbyteArray sign_out = (*env)->NewByteArray(env, signature.size);
    if (NULL == sign_out)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "sign_out");
        goto cleanup_signature;
    }

    /* get the raw bytes for this buffer. */
    jbyte* sign_out_bytes = (*env)->GetByteArrayElements(env, sign_out, NULL);
    if (NULL == sign_out_bytes)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "sign_out_bytes");
        goto cleanup_sign_out;
    }

    /* copy the raw bytes of the signature into this byte buffer. */
    MODEL_EXEMPT(
        memcpy(sign_out_bytes, signature.data, signature.size));

    /* complete the array write. */
    (*env)->ReleaseByteArrayElements(env, sign_out, sign_out_bytes, 0);

    /* create a signature object using this byte buffer. */
    retval =
        (*env)->NewObject(
            env, Signature, Signature_init, sign_out);
    if (NULL == retval)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Signature_new");
        goto cleanup_sign_out;
    }

    /* fall-through. */

cleanup_sign_out:
    (*env)->DeleteLocalRef(env, sign_out);

cleanup_signature:
    dispose((disposable_t*)&signature);

cleanup_message_bytes:
    (*env)->ReleaseByteArrayElements(
        env, message_array, message_bytes, JNI_ABORT);

cleanup_message_array:
    (*env)->DeleteLocalRef(env, message_array);

cleanup_sign:
    dispose((disposable_t*)&sign);

cleanup_private_key_bytes:
    (*env)->ReleaseByteArrayElements(
        env, private_key, private_key_bytes, JNI_ABORT);

cleanup_privkey:
    dispose((disposable_t*)&privkey);

cleanup_private_key:
    (*env)->DeleteLocalRef(env, private_key);

    return retval;
}
