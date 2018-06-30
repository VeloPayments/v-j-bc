/**
 * \file AgentConnection_closeNative.c
 *
 * Close a local database connection.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/parameters.h>

#include "AgentConnection.h"

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    closeNative
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_closeNative(
    JNIEnv* UNUSED(env), jobject UNUSED(that), jlong UNUSED(handle))
{
    /* TODO - implement. */
}
