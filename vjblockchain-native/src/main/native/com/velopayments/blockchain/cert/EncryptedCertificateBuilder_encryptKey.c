/**
 * \file EncryptedCertificateBuilder_encryptKey.c
 *
 * Encrypt and MAC the key for the encrypted data.
 *
 * \copyright 2018-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/block_cipher.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_cert_EncryptedCertificateBuilder
 * Method:    encryptKey
 * Signature: (Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;[B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_cert_EncryptedCertificateBuilder_encryptKey(
    JNIEnv *env, jclass UNUSED(clazz), jlong nativeInst, jobject localPrivate,
    jobject peerPublic, jbyteArray key)
{
    jbyteArray retval = NULL;

    const size_t IV_SIZE = 16;

    vccrypt_buffer_t keyBuffer;
    vccrypt_buffer_t privateKeyBuffer;
    vccrypt_buffer_t publicKeyBuffer;
    vccrypt_buffer_t ivBuffer;
    vccrypt_block_context_t block;
    vccrypt_key_agreement_context_t ka;
    vccrypt_prng_context_t prng;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);
    MODEL_ASSERT(NULL != localPrivate);
    MODEL_ASSERT(NULL != peerPublic);
    MODEL_ASSERT(NULL != key);

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

    /* verify that the localPrivate parameter is not NULL. */
    if (NULL == localPrivate)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "localPrivate");
        return NULL;
    }

    /* verify that the peerPublic parameter is not NULL. */
    if (NULL == peerPublic)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "peerPublic");
        return NULL;
    }

    /* verify that the key parameter is not NULL. */
    if (NULL == key)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "key");
        return NULL;
    }

    /* get the buffer for the key array. */
    jbyte* keyArrayData = (*env)->GetByteArrayElements(env, key, NULL);
    if (NULL == keyArrayData)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "key data read failure.");
        return NULL;
    }

    /* get the byte array for the private key. */
    jbyteArray privateKeyArray = (*env)->CallObjectMethod(
            env, localPrivate, native_inst->EncryptionPrivateKey.getRawBytes);
    if (NULL == privateKeyArray)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "localPrivate.getRawBytes()");
        goto keyArrayData_dispose;
    }

    /* get the buffer for the key array. */
    jbyte* privateKeyArrayData =
        (*env)->GetByteArrayElements(env, privateKeyArray, NULL);
    if (NULL == privateKeyArrayData)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "private key data read failure.");
        goto privateKeyArray_dispose;
    }

    /* create a vccrypt buffer for this private key data. */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_private_key(
                    &native_inst->crypto_suite, &privateKeyBuffer))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "private key buffer creation failure.");
        goto privateKeyArrayData_dispose;
    }

    /* copy the private key data to the private key buffer. */
    MODEL_EXEMPT(
        memcpy(privateKeyBuffer.data, privateKeyArrayData, 32));

    /* get the byte array for the public key. */
    jbyteArray publicKeyArray = (*env)->CallObjectMethod(
            env, peerPublic, native_inst->EncryptionPublicKey.getRawBytes);
    if (NULL == publicKeyArray)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "publicKeyArray.getRawBytes()");
        goto privateKeyBuffer_dispose;
    }

    /* get the buffer for the key array. */
    jbyte* publicKeyArrayData =
        (*env)->GetByteArrayElements(env, publicKeyArray, NULL);
    if (NULL == publicKeyArrayData)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "public key data read failure.");
        goto publicKeyArray_dispose;
    }

    /* create a vccrypt buffer for this public key data. */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_public_key(
                    &native_inst->crypto_suite, &publicKeyBuffer))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "public key buffer creation failure.");
        goto publicKeyArrayData_dispose;
    }

    /* copy the public key data to the public key buffer. */
    MODEL_EXEMPT(
        memcpy(publicKeyBuffer.data, publicKeyArrayData, 32));

    /* create a vccrypt buffer for the long term shared secret. */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_shared_secret(
                    &native_inst->crypto_suite, &keyBuffer))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "public key buffer creation failure.");
        goto publicKeyBuffer_dispose;
    }

    /* create the key agreement instance. */
    if (0 != vccrypt_suite_cipher_key_agreement_init(
                    &native_inst->crypto_suite, &ka))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "key agreement context creation failure.");
        goto keyBuffer_dispose;
    }

    /* create the long-term shared secret. */
    if (0 != vccrypt_key_agreement_long_term_secret_create(
                &ka, &privateKeyBuffer, &publicKeyBuffer, &keyBuffer))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "key agreement failure.");
        goto ka_dispose;
    }

    /* create a prng instance for generating the nonce. */
    if (0 != vccrypt_suite_prng_init(&native_inst->crypto_suite, &prng))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "prng instance creation failure.");
        goto ka_dispose;
    }

    /* create a buffer for the iv. */
    if (0 != vccrypt_buffer_init(&ivBuffer, &native_inst->alloc_opts, IV_SIZE))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "iv buffer creation failure.");
        goto prng_dispose;
    }

    /* read data into the iv buffer. */
    if (0 != vccrypt_prng_read(&prng, &ivBuffer, IV_SIZE))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "prng read failure.");
        goto ivBuffer_dispose;
    }

    /* the total size of the output array */
    size_t input_size = (*env)->GetArrayLength(env, key);
    size_t output_size =
        IV_SIZE +
        input_size;

    /* create the output byte array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, output_size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "outputArray creation failure.");
        goto ivBuffer_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
        (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(env, native_inst->NullPointerException.classid,
                         "outputArray data read failure.");
        goto ivBuffer_dispose;
    }

    /* create the block cipher instance. */
    if (0 != vccrypt_suite_block_init(
                    &native_inst->crypto_suite, &block, &keyBuffer, true))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "block cipher creation failure.");
        goto outputArrayData_dispose;
    }

    const uint8_t* keyData = (const uint8_t*)keyArrayData;
    uint8_t* out = (uint8_t*)outputArrayData;

    /* write the iv */
    MODEL_EXEMPT(
        memcpy(out, ivBuffer.data, 16));

    /* encrypt the first block of the key. */
    if (0 != vccrypt_block_encrypt(&block, out, keyData, out + 16))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "block cipher block 1 encrypt failure.");
        goto block_dispose;
    }

    /* encrypt the second block of the key. */
    if (0 != vccrypt_block_encrypt(&block, out + 16, keyData + 16, out + 32))
    {
        (*env)->ThrowNew(env, native_inst->IllegalStateException.classid,
                         "block cipher block 2 encrypt failure.");
        goto block_dispose;
    }

    /* success - set the output array to the return value. */
    retval = outputArray;

block_dispose:
    dispose((disposable_t*)&block);

outputArrayData_dispose:
    (*env)->ReleaseByteArrayElements(
                env, outputArray, outputArrayData, 0);
    
ivBuffer_dispose:
    dispose((disposable_t*)&ivBuffer);

prng_dispose:
    dispose((disposable_t*)&prng);

ka_dispose:
    dispose((disposable_t*)&ka);

keyBuffer_dispose:
    dispose((disposable_t*)&keyBuffer);

publicKeyBuffer_dispose:
    dispose((disposable_t*)&publicKeyBuffer);

publicKeyArrayData_dispose:
    (*env)->ReleaseByteArrayElements(
                env, publicKeyArray, publicKeyArrayData, JNI_ABORT);

publicKeyArray_dispose:
    (*env)->DeleteLocalRef(env, publicKeyArray);

privateKeyBuffer_dispose:
    dispose((disposable_t*)&privateKeyBuffer);

privateKeyArrayData_dispose:
    (*env)->ReleaseByteArrayElements(
                env, privateKeyArray, privateKeyArrayData, JNI_ABORT);

privateKeyArray_dispose:
    (*env)->DeleteLocalRef(env, privateKeyArray);

keyArrayData_dispose:
    (*env)->ReleaseByteArrayElements(env, key, keyArrayData, JNI_ABORT);

    return retval;
}
