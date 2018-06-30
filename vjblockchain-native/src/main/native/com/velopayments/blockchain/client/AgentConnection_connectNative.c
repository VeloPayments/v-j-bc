/**
 * \file AgentConnection_connectNative.c
 *
 * Connect to a local database.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/parameters.h>

#include "AgentConnection.h"

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    connectNative
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_connectNative(
    JNIEnv* UNUSED(env), jobject UNUSED(that), jstring UNUSED(connect))
{
    /* TODO - implement. */

    return 1;
}
