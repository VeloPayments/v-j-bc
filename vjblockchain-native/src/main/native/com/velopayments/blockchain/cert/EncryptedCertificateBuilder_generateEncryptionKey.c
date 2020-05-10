/**
 * \file EncryptedCertificateBuilder_generateEncryptionKey.c
 *
 * Generate an encryption key using the Velo crypto suite.
 *
 * \copyright 2018-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_cert_EncryptedCertificateBuilder
 * Method:    generateEncryptionKey
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL
Java_com_velopayments_blockchain_cert_EncryptedCertificateBuilder_generateEncryptionKey(
    JNIEnv* env, jclass UNUSED(clazz), jlong nativeInst)
{
    jbyteArray retval = NULL;

    vccrypt_prng_context_t prng;
    vccrypt_buffer_t randomKey;

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

    /* initialize key buffer. */
    if (0 != vccrypt_buffer_init(&randomKey, &native_inst->alloc_opts, 32))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "bad vccrypt buffer.");
        return NULL;
    }

    /* initialize prng. */
    if (0 != vccrypt_suite_prng_init(&native_inst->crypto_suite, &prng))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "bad vccrypt prng.");
        goto dispose_randomKey;
    }

    /* read cryptographically random key. */
    if (0 != vccrypt_prng_read(&prng, &randomKey, 32))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "bad vccrypt prng read.");
        goto dispose_prng;
    }

    /* create a byte array for saving the key. */
    jbyteArray keyArray = (*env)->NewByteArray(env, 32);
    if (NULL == keyArray)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "bad keyArray alloc.");
        goto dispose_prng;
    }

    /* get the buffer from this array. */
    jbyte* keyArrayData = (*env)->GetByteArrayElements(env, keyArray, NULL);
    if (NULL == keyArrayData)
    {
        (*env)->ThrowNew(
            env, native_inst->NullPointerException.classid,
            "keyArray data could not be dereferenced.");
        goto dispose_prng;
    }

    /* copy the key data to the key array buffer. */
    MODEL_EXEMPT(
        memcpy(keyArrayData, randomKey.data, 32));

    /* commit data to the byte array. */
    (*env)->ReleaseByteArrayElements(env, keyArray, keyArrayData, 0);

    /* success.  Update retval. */
    retval = keyArray;

dispose_prng:
    dispose((disposable_t*)&prng);

dispose_randomKey:
    dispose((disposable_t*)&randomKey);

    return retval;
}
