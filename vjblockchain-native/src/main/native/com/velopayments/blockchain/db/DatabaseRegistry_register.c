/**
 * \file DatabaseRegistry_register.c
 *
 * Register a database adapter engine with vcdb's engine registry.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/engine.h>
#include <vpr/parameters.h>
#include <com_velopayments_blockchain_db_DatabaseRegistry.h>

#include "../../../../java/lang/NullPointerException.h"

/*
 * Class:     com_velopayments_blockchain_db_DatabaseRegistry
 * Method:    register
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_com_velopayments_blockchain_db_DatabaseRegistry_register(
    JNIEnv* env, jclass UNUSED(that), jlong handle, jstring engine)
{

    /* get the database engine details structure. */
    vcdb_database_engine_t* engine_ptr = (vcdb_database_engine_t*)handle;
    if (NULL == engine_ptr)
    {
        (*env)->ThrowNew(env, NullPointerException, "handle");
        return;
    }

    /* verify that the engine parameter is not null. */
    if (NULL == engine)
    {
        (*env)->ThrowNew(env, NullPointerException, "engine");
        return;
    }

    const char* engine_str = (*env)->GetStringUTFChars(env, engine, NULL);

    vcdb_database_engine_register(engine_ptr, engine_str);

    (*env)->ReleaseStringUTFChars(env, engine, engine_str);
}
