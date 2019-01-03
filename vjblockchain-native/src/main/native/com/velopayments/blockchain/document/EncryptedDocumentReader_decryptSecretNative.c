/**
 * \file EncryptedCertificateReader_decryptSecretNative.c
 *
 * Encrypt and MAC the key for the encrypted data.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/block_cipher.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../../../../com/velopayments/blockchain/crypt/EncryptionPrivateKey.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionPublicKey.h"
#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"

/*
 * Class:     com_velopayments_blockchain_document_EncryptedDocumentReader
 * Method:    decryptSecretNative
 * Signature: (Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;[B)[B
 */
 JNIEXPORT jbyteArray JNICALL
 Java_com_velopayments_blockchain_document_EncryptedDocumentReader_decryptSecretNative
   (JNIEnv *env, jclass UNUSED(clazz), jobject localPrivateKey,
   jobject peerPublicKey, jbyteArray encryptedKey)
{
    jbyteArray retval = NULL;

    const size_t IV_SIZE = 16;

    vccrypt_buffer_t keyBuffer;
    vccrypt_buffer_t privateKeyBuffer;
    vccrypt_buffer_t publicKeyBuffer;
    vccrypt_block_options_t block_opts;
    vccrypt_block_context_t block;
    vccrypt_key_agreement_context_t ka;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != localPrivateKey);
    MODEL_ASSERT(NULL != peerPublicKey);
    MODEL_ASSERT(NULL != encryptedKey);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the localPrivateKey parameter is not null. */
    if (NULL == localPrivateKey)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "localPrivateKey");
        return NULL;
    }

    /* verify that the peerPublicKey parameter is not null. */
    if (NULL == peerPublicKey)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "peerPublicKey");
        return NULL;
    }

    /* verify that the encryptedKey parameter is not null. */
    if (NULL == encryptedKey)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "encryptedKey");
        return NULL;
    }

    /* get the buffer for the key array. */
    jbyte* keyArrayData = (*env)->GetByteArrayElements(env, encryptedKey, NULL);
    if (NULL == keyArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "key data read failure.");
        return NULL;
    }

    /* get the byte array for the private key. */
    jbyteArray privateKeyArray = (*env)->CallObjectMethod(
            env, localPrivateKey, EncryptionPrivateKey_getRawBytes);
    if (NULL == privateKeyArray)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "private key read failure.");
        goto keyArrayData_dispose ;
    }

    /* get the buffer for the key array. */
    jbyte* privateKeyArrayData =
        (*env)->GetByteArrayElements(env, privateKeyArray, NULL);
    if (NULL == privateKeyArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "private key data read failure.");
        goto privateKeyArray_dispose;
    }

    /* create a vccrypt buffer for this private key data. */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_private_key(
                    &crypto_suite, &privateKeyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "private key buffer creation failure.");
        goto privateKeyArrayData_dispose;
    }

    /* copy the private key data to the private key buffer. */
    MODEL_EXEMPT(
        memcpy(privateKeyBuffer.data, privateKeyArrayData, 32));

    /* get the byte array for the public key. */
    jbyteArray publicKeyArray = (*env)->CallObjectMethod(
            env, peerPublicKey, EncryptionPublicKey_getRawBytes);
    if (NULL == publicKeyArray)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "peerPublicKey.getRawBytes()");
        goto privateKeyBuffer_dispose;
    }

    /* get the buffer for the key array. */
    jbyte* publicKeyArrayData =
        (*env)->GetByteArrayElements(env, publicKeyArray, NULL);
    if (NULL == publicKeyArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "public key data read failure.");
        goto publicKeyArray_dispose;
    }

    /* create a vccrypt buffer for this public key data. */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_public_key(
                    &crypto_suite, &publicKeyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "public key buffer creation failure.");
        goto publicKeyArrayData_dispose;
    }

    /* copy the public key data to the public key buffer. */
    MODEL_EXEMPT(
        memcpy(publicKeyBuffer.data, publicKeyArrayData, 32));

    /* create a vccrypt buffer for the long term shared secret. */
    if (0 != vccrypt_suite_buffer_init_for_cipher_key_agreement_shared_secret(
                    &crypto_suite, &keyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "public key buffer creation failure.");
        goto publicKeyBuffer_dispose;
    }

    /* create the key agreement instance. */
    if (0 != vccrypt_suite_cipher_key_agreement_init(&crypto_suite, &ka))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key agreement context creation failure.");
        goto keyBuffer_dispose;
    }

    /* create the long-term shared secret. */
    if (0 != vccrypt_key_agreement_long_term_secret_create(
                &ka, &privateKeyBuffer, &publicKeyBuffer, &keyBuffer))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "key agreement failure.");
        goto ka_dispose;
    }

    /* the total size of the output array */
    size_t input_size = (*env)->GetArrayLength(env, encryptedKey);
    size_t output_size = input_size - IV_SIZE;

    /* create the output byte array. */
    jbyteArray outputArray = (*env)->NewByteArray(env, output_size);
    if (NULL == outputArray)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "outputArray creation failure.");
        goto ka_dispose;
    }

    /* get the buffer from this array. */
    jbyte* outputArrayData =
        (*env)->GetByteArrayElements(env, outputArray, NULL);
    if (NULL == outputArrayData)
    {
        (*env)->ThrowNew(env, NullPointerException,
                         "outputArray data read failure.");
        goto ka_dispose;
    }

    /* create the block cipher options. */
    if (0 != vccrypt_block_options_init(
                &block_opts, &alloc_opts,
                VCCRYPT_BLOCK_ALGORITHM_AES_256_2X_CBC))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "block cipher options creation failure.");
        goto outputArrayData_dispose;
    }

    /* create the block cipher instance. */
    if (0 != vccrypt_block_init(&block_opts, &block, &keyBuffer, false))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "block cipher creation failure.");
        goto block_opts_dispose;
    }

    const uint8_t* keyData = (const uint8_t*)keyArrayData;
    uint8_t* out = (uint8_t*)outputArrayData;

    /* decrypt the first block of the key. */
    if (0 != vccrypt_block_decrypt(
                &block, keyData, keyData + 16, out))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "block cipher block 1 decrypt failure.");
        goto block_dispose;
    }

    /* decrypt the second block of the key. */
    if (0 != vccrypt_block_decrypt(
                &block, keyData + 16, keyData + 32, out + 16))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "block cipher block 2 decrypt failure.");
        goto block_dispose;
    }

    /* success - set the output array to the return value. */
    retval = outputArray;

block_dispose:
    dispose((disposable_t*)&block);

block_opts_dispose:
    dispose((disposable_t*)&block_opts);

outputArrayData_dispose:
    (*env)->ReleaseByteArrayElements(
                env, outputArray, outputArrayData, 0);

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
    (*env)->ReleaseByteArrayElements(
                env, encryptedKey, keyArrayData, JNI_ABORT);

    return retval;
}
