/**
 * \file AgentConnection_makeBlockNative.c
 *
 * Make a block using the currently pending transactions.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/parameters.h>

#include "AgentConnection.h"

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    makeBlockNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_makeBlockNative(
    JNIEnv* UNUSED(env), jobject UNUSED(that))
{
    /* TODO - implement. */
}
