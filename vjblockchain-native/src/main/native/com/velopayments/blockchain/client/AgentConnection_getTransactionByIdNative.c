/**
 * \file AgentConnection_getTransactionByIdNative.c
 *
 * Get a Transaction from the database by a UUID.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/parameters.h>

#include "AgentConnection.h"
#include "java/util/Optional.h"

/*
 * Class:     com_velopayments_blockchain_client_AgentConnection
 * Method:    getTransactionByIdNative
 * Signature: (Ljava/util/UUID;)Ljava/util/Optional;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_client_AgentConnection_getTransactionByIdNative(
    JNIEnv* env, jobject UNUSED(that), jobject UNUSED(txnId))
{
    /* TODO - implement. */

    return
        (*env)->CallStaticObjectMethod(env, Optional, Optional_empty);
}
