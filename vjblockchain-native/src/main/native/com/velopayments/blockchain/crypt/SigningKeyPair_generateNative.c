/**
 * \file SigningKeyPair_generateNative.c
 *
 * Generate a digital signature keypair using the Velo crypto suite.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_SigningKeyPair
 * Method:    generateNative
 * Signature: ()Lcom/velopayments/blockchain/crypt/SigningKeyPair;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_crypt_SigningKeyPair_generateNative(
    JNIEnv *env, jclass UNUSED(clazz))
{
    jobject retval = NULL;

    vccrypt_digital_signature_context_t sign;
    vccrypt_buffer_t priv;
    vccrypt_buffer_t pub;

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

    /* initialize digital signature impl. */
    if (0 !=
            vccrypt_suite_digital_signature_init(
                &native_inst->crypto_suite, &sign))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "digital signature could not be initialized.");
        return NULL;
    }

    MODEL_ASSERT(MODEL_PROP_VALID_VCCRYPT_DIGITAL_SIGNATURE_CONTEXT(&sign));

    /* initialize private key buffer. */
    if (0 !=
            vccrypt_suite_buffer_init_for_signature_private_key(
                &native_inst->crypto_suite, &priv))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "private key buffer could not be initialized.");
        goto sign_dispose;
    }

    MODEL_ASSERT(MODEL_PROP_VALID_VCCRYPT_BUFFER(&priv));

    /* initialize public key buffer. */
    if (0 !=
            vccrypt_suite_buffer_init_for_signature_public_key(
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
            vccrypt_digital_signature_keypair_create(
                &sign, &priv, &pub))
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

    /* create the SigningPrivateKey object. */
    jobject signingPrivateKey =
        (*env)->NewObject(
            env, native_inst->SigningPrivateKey.classid,
            native_inst->SigningPrivateKey.init, privArray);
    if (NULL == signingPrivateKey)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "signingPrivateKey could not be instantiated.");
        goto pub_array_dispose;
    }

    /* create the SigningPublicKey object. */
    jobject signingPublicKey =
        (*env)->NewObject(
            env, native_inst->SigningPublicKey.classid,
            native_inst->SigningPublicKey.init, pubArray);
    if (NULL == signingPublicKey)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "signingPublicKey could not be instantiated.");

        goto pub_array_dispose;
    }

    /* create the return object. */
    retval =
        (*env)->NewObject(
            env, native_inst->SigningKeyPair.classid,
            native_inst->SigningKeyPair.init, signingPublicKey,
            signingPrivateKey);

    /* fall-through cleanup and return of allocated SigningKeyPair. */

pub_array_dispose:
    (*env)->DeleteLocalRef(env, pubArray);

priv_array_dispose:
    (*env)->DeleteLocalRef(env, privArray);

pub_dispose:
    dispose((disposable_t*)&pub);

priv_dispose:
    dispose((disposable_t*)&priv);

sign_dispose:
    dispose((disposable_t*)&sign);

    return retval;
}
