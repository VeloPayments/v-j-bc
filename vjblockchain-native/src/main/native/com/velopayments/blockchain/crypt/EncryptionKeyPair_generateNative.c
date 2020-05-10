/**
 * \file EncryptionKeyPair_generateNative.c
 *
 * Generate an encryption keypair using the Velo crypto suite.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_EncryptionKeyPair
 * Method:    generateNative
 * Signature: ()Lcom/velopayments/blockchain/crypt/EncryptionKeyPair;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_crypt_EncryptionKeyPair_generateNative(
    JNIEnv *env, jclass UNUSED(clazz), jlong nativeInst)
{
    jobject retval = NULL;

    vccrypt_key_agreement_context_t ka;
    vccrypt_buffer_t priv;
    vccrypt_buffer_t pub;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);

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

    /* initialize key agreement impl. */
    if (0 !=
            vccrypt_suite_cipher_key_agreement_init(
                &native_inst->crypto_suite, &ka))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "cipher key agreement could not be initialized.");
        return NULL;
    }

    MODEL_ASSERT(MODEL_PROP_VALID_VCCRYPT_CIPHER_KEY_AGREEMENT_CONTEXT(&ka));

    /* initialize private key buffer. */
    if (0 !=
            vccrypt_suite_buffer_init_for_cipher_key_agreement_private_key(
                &native_inst->crypto_suite, &priv))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "private key buffer could not be initialized.");
        goto ka_dispose;
    }

    MODEL_ASSERT(MODEL_PROP_VALID_VCCRYPT_BUFFER(&priv));

    /* initialize public key buffer. */
    if (0 !=
            vccrypt_suite_buffer_init_for_cipher_key_agreement_public_key(
                &native_inst->crypto_suite, &pub))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "public key buffer could not be initialized.");
        goto priv_dispose;
    }

    MODEL_ASSERT(MODEL_PROP_VALID_VCCRYPT_BUFFER(&pub));

    /* create keypair. */
    if (0 !=
            vccrypt_key_agreement_keypair_create(
                &ka, &priv, &pub))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "keypair could not be generated.");
        goto pub_dispose;
    }

    /* create a byte array for the private key. */
    jbyteArray privArray = (*env)->NewByteArray(env, priv.size);
    if (NULL == privArray)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "priv key array could not be allocated.");
        goto pub_dispose;
    }

    /* get the buffer of this array. */
    jbyte* privArrayData = (*env)->GetByteArrayElements(env, privArray, NULL);
    if (NULL == privArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "priv key array data could not be dereferenced.");
        goto priv_array_dispose;
    }

    /* copy the private key data to the buffer. */
    MODEL_EXEMPT(
        memcpy(privArrayData, priv.data, priv.size));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, privArray, privArrayData, 0);

    /* create a byte array for the public key. */
    jbyteArray pubArray = (*env)->NewByteArray(env, pub.size);
    if (NULL == pubArray)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "pub key array could not be allocated.");
        goto priv_array_dispose;
    }

    /* get the buffer of this array. */
    jbyte* pubArrayData = (*env)->GetByteArrayElements(env, pubArray, NULL);
    if (NULL == pubArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "pub key array data could not be dereferenced.");
        goto pub_array_dispose;
    }

    /* copy the public key data to the buffer. */
    MODEL_EXEMPT(
        memcpy(pubArrayData, pub.data, pub.size));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, pubArray, pubArrayData, 0);

    /* create the private key object. */
    jobject privKey =
        (*env)->NewObject(
            env, native_inst->EncryptionPrivateKey.classid,
            native_inst->EncryptionPrivateKey.init, privArray);
    if (NULL == privKey)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "privateKey could not be instantiated.");
        goto pub_array_dispose;
    }

    /* create the public key object. */
    jobject pubKey =
        (*env)->NewObject(
            env, native_inst->EncryptionPublicKey.classid,
            native_inst->EncryptionPublicKey.init, pubArray);
    if (NULL == pubKey)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "publicKey could not be instantiated.");
        goto priv_key_dispose;
    }

    /* create the return object. */
    retval =
        (*env)->NewObject(
            env, native_inst->EncryptionKeyPair.classid,
            native_inst->EncryptionKeyPair.init, pubKey, privKey);
    if (NULL == retval)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "EncryptionKeyPair could not be instantiated.");
        goto pub_key_dispose;
    }

    /* fall-through cleanup and return of allocated EncryptionKeyPair. */

pub_key_dispose:
    (*env)->DeleteLocalRef(env, pubKey);

priv_key_dispose:
    (*env)->DeleteLocalRef(env, privKey);

pub_array_dispose:
    (*env)->DeleteLocalRef(env, pubArray);

priv_array_dispose:
    (*env)->DeleteLocalRef(env, privArray);

pub_dispose:
    dispose((disposable_t*)&pub);

priv_dispose:
    dispose((disposable_t*)&priv);

ka_dispose:
    dispose((disposable_t*)&ka);

    return retval;
}
