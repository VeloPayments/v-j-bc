/**
 * \file CertificateParser_parseNative.c
 *
 * Parse the certificate and build a map to return to Java.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>
#include <com_velopayments_blockchain_cert_CertificateParser.h>

#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalArgumentException.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/NullPointerException.h"
#include "../../../../java/lang/Integer.h"
#include "../../../../java/util/AbstractMap_SimpleEntry.h"
#include "../../../../java/util/LinkedList.h"

//forward declarations for dummy certificate delegate methods
static bool dummy_txn_resolver(
                    void*, void*, const uint8_t*, const uint8_t*,
                    vccrypt_buffer_t*, bool*);
static int32_t dummy_artifact_state_resolver(
                    void*, void*, const uint8_t*, vccrypt_buffer_t*);
static bool dummy_entity_key_resolver(
                void*, void*, uint64_t, const uint8_t*, vccrypt_buffer_t*,
                vccrypt_buffer_t*);
static vccert_contract_fn_t dummy_contract_resolver(
                void*, void*, const uint8_t*, const uint8_t*);

/*
 * Class:     com_velopayments_blockchain_cert_CertificateParser
 * Method:    parseNative
 * Signature: ([BI)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_cert_CertificateParser_parseNative(
    JNIEnv* env, jobject UNUSED(that), jbyteArray cert, jint size)
{
    vccert_parser_options_t parser_options;
    uint16_t field_id;
    const uint8_t* value;
    size_t field_size;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != cert);
    MODEL_ASSERT(0 <= size);

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the cert parameter is not null. */
    if (NULL == cert)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "cert");
        return NULL;
    }

    /* create the parser options structure for this parse. */
    if (0 != vccert_parser_options_init(
                    &parser_options, &native_inst->alloc_opts,
                    &native_inst->crypto_suite, &dummy_txn_resolver,
                    &dummy_artifact_state_resolver, &dummy_contract_resolver,
                    &dummy_entity_key_resolver, NULL))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vccert could not be initialized.");
        return NULL;
    }

    /* create the return object */
    jobject result = (*env)->NewObject(env, LinkedList, LinkedList_init);

    /* copy the C bytes for this byte array. */
    jbyte* bufferPtr = (*env)->GetByteArrayElements(env, cert, NULL);

    /* create a parser to parse this buffer */
    vccert_parser_context_t parser;
    if (0 != vccert_parser_init(&parser_options, &parser, bufferPtr, size))
    {
        (*env)->ThrowNew(
            env, IllegalArgumentException, "certificate invalid.");
        result = NULL;
        goto releaseJBuffer;
    }

    /* get the first field for this certificate */
    if (0 != vccert_parser_field_first(&parser, &field_id, &value, &field_size))
    {
        (*env)->ThrowNew(
            env, IllegalArgumentException, "certificate has no valid fields.");
        result = NULL;
        goto disposeParser;
    }

    /* copy this field to the output list and get the next field. */
    do
    {
        /* create byte[] value for SimpleEntry */
        jbyteArray byteArray = (*env)->NewByteArray(env, field_size);
        (*env)->SetByteArrayRegion(
            env, byteArray, 0, field_size, (int8_t*)value);

        /* create Integer key for SimpleEntry */
        jobject key =
            (*env)->CallStaticObjectMethod(
                env, Integer, Integer_valueOf, (jint)field_id);

        /* create the SimpleEntry of (key, value) */
        jobject entry =
            (*env)->NewObject(
                env, SimpleEntry, SimpleEntry_init, key, byteArray);

        /* add this entry to our result list */
        (*env)->CallBooleanMethod(env, result, LinkedList_add, entry);

        /* let the collector know these references are falling out of scope. */
        (*env)->DeleteLocalRef(env, key);
        (*env)->DeleteLocalRef(env, entry);
        (*env)->DeleteLocalRef(env, byteArray);

    } while (
        0 == vccert_parser_field_next(&parser, &field_id, &value, &field_size));

disposeParser:
    dispose((disposable_t*)&parser);

releaseJBuffer:
    (*env)->ReleaseByteArrayElements(env, cert, bufferPtr, JNI_ABORT);

    dispose((disposable_t*)&parser_options);

    return result;
}

/**
 * Dummy transaction resolver.
 */
static bool dummy_txn_resolver(
                    void* UNUSED(options), void* UNUSED(parser),
                    const uint8_t* UNUSED(artifact_id),
                    const uint8_t* UNUSED(txn_id),
                    vccrypt_buffer_t* UNUSED(output_buffer),
                    bool* UNUSED(trusted))
{
    return false;
}

/**
 * Dummy artifact state resolver.
 */
static int32_t dummy_artifact_state_resolver(
                    void* UNUSED(options), void* UNUSED(parser),
                    const uint8_t* UNUSED(artifact_id),
                    vccrypt_buffer_t* UNUSED(txn_id))
{
    return -1;
}

/**
 * Dummy entity key resolver.
 */
static bool dummy_entity_key_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                uint64_t UNUSED(height), const uint8_t* UNUSED(entity_id),
                vccrypt_buffer_t* UNUSED(pubenckey_buffer),
                vccrypt_buffer_t* UNUSED(pubsignkey_buffer))
{
    return false;
}

/**
 * Dummy contract resolver.
 */
static vccert_contract_fn_t dummy_contract_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                const uint8_t* UNUSED(type_id),
                const uint8_t* UNUSED(artifact_id))
{
    return NULL;
}
