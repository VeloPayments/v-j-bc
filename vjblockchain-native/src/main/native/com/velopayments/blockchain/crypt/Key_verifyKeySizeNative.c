/**
 * \file Key_verifyKeySizeNative.c
 *
 * Verify the size of the key, throwing an exception if the key size is invalid.
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "../init/init.h"

/*
 * Class:     com_velopayments_blockchain_crypt_Key
 * Method:    verifyKeySizeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_crypt_Key_verifyKeySizeNative(
    JNIEnv *env, jobject that, jlong nativeInst)
{
    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);
    MODEL_ASSERT(NULL != that);

    /* get a pointer to the native instance. */
    vjblockchain_native_instance* native_inst =
        (vjblockchain_native_instance*)nativeInst;

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vjblockchain not initialized.");
        return;
    }

    /* get the length of the key. */
    jbyteArray key = (*env)->GetObjectField(env, that, native_inst->Key.key);
    jint keyLen = (*env)->GetArrayLength(env, key);

    if ((size_t)keyLen != native_inst->crypto_suite.stream_cipher_opts.key_size)
    {
        (*env)->ThrowNew(
            env, native_inst->InvalidKeySizeException.classid,
            "Invalid Key Size.");
    }
}
