/**
 * \file AgentConnection_getLatestBlockIdNative.c
 *
 * Get the latest block's UUID.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/parameters.h>

#include "AgentConnection.h"
#include "java/util/UUID.h"

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    getLatestBlockIdNative
 * Signature: ()Ljava/util/UUID;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_getLatestBlockIdNative(
    JNIEnv* env, jobject UNUSED(that))
{
    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);

    jobject ret =
        (*env)->NewObject(
            env, UUID, UUID_init, 0, 0);

    return ret;
}
