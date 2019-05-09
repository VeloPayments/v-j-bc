/**
 * \file Key_verifyKeySizeNative.c
 *
 * Verify the size of the key, throwing an exception if the key size is invalid.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>

#include "InvalidKeySizeException.h"
#include "Key.h"
#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"

/*
 * Class:     com_velopayments_blockchain_crypt_Key
 * Method:    verifyKeySizeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_crypt_Key_verifyKeySizeNative(
    JNIEnv *env, jobject that)
{
    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return;
    }

    /* get the length of the key. */
    jbyteArray key = (*env)->GetObjectField(env, that, Key_key);
    jint keyLen = (*env)->GetArrayLength(env, key);

    if ((size_t)keyLen != stream_opts.key_size)
    {
        (*env)->ThrowNew(
            env, InvalidKeySizeException, "Invalid Key Size.");
    }
}
