/**
 * \file CertificateParser_attestNative.c
 *
 * Perform certificate attestation.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vccert/fields.h>
#include <vccrypt/suite.h>
#include <vjblockchain.h>
#include <vpr/parameters.h>

#include "AttestationException.h"
#include "CertificateParser.h"
#include "CertificateParserDelegate.h"
#include "EntityReference.h"
#include "UnknownEntityException.h"
#include "../../../../com/velopayments/blockchain/init/init.h"
#include "../../../../java/lang/IllegalArgumentException.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../util/uuid_conv.h"

//forward declarations for certificate delegate methods
static jboolean attest(
    JNIEnv* env, jobject, vccert_parser_context_t*, jlong height,
    jboolean validateContract);
static bool attest_txn_resolver(
                void*, void*, const uint8_t*, const uint8_t*,
                vccrypt_buffer_t*, bool*);
static int32_t attest_artifact_state_resolver(
                void*, void*, const uint8_t*, vccrypt_buffer_t*);
static bool attest_entity_key_resolver(
                void*, void*, uint64_t, const uint8_t*, vccrypt_buffer_t*,
                vccrypt_buffer_t*);
static vccert_contract_fn_t attest_contract_resolver(
                void*, void*, const uint8_t*, const uint8_t*);

/**
 * User context structure for parser options.  This structure holds the JNI
 * Environment and the CertificateParserDelegate instance for this
 * CertificateParser.  This context structure provides the context necessary to
 * re-enter Java CertificateParserDelegate callbacks from the vccert C library.
 */
typedef struct parser_user_context
{
    JNIEnv* env;
    jobject parser;
    jobject delegate;
} parser_user_context_t;

/*
 * Class:     com_velopayments_blockchain_cert_CertificateParser
 * Method:    attestNative
 * Signature: (Lcom/velopayments/blockchain/cert/CertificateParserDelegate;JZ)Z
 */
JNIEXPORT jboolean JNICALL
Java_com_velopayments_blockchain_cert_CertificateParser_attestNative(
    JNIEnv* env, jobject that, jobject delegate, jlong height,
    jboolean validateContract)
{
    jboolean result = JNI_FALSE;
    vccert_parser_options_t parser_options;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != delegate);
    MODEL_ASSERT(0 <= height);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return JNI_FALSE;
    }

    /* set up the user context */
    parser_user_context_t user_context;
    user_context.env = env;
    user_context.parser = that;
    user_context.delegate = delegate;

    /* create the parser options structure for attestation. */
    if (0 != vccert_parser_options_init(
                &parser_options, &alloc_opts, &crypto_suite,
                &attest_txn_resolver, &attest_artifact_state_resolver,
                &attest_contract_resolver, &attest_entity_key_resolver,
                &user_context))
    {
        (*env)->ThrowNew(env, IllegalStateException,
                         "vccert could not be initialized.");
        return JNI_FALSE;
    }

    /* get the certificate and rawSize fields */
    jint raw_size =
        (*env)->GetIntField(env, that, CertificateParser_field_rawSize);
    jbyteArray cert = (jbyteArray)
        (*env)->GetObjectField(env, that, CertificateParser_field_certificate);

    /* copy the C bytes for the certificate byte array */
    jbyte* bufferPtr = (*env)->GetByteArrayElements(env, cert, NULL);

    /* create a parser to attest this buffer */
    vccert_parser_context_t parser;
    if (0 != vccert_parser_init(&parser_options, &parser, bufferPtr, raw_size))
    {
        (*env)->ThrowNew(
            env, IllegalArgumentException, "certificate invalid.");
        result = JNI_FALSE;
        goto releaseJBuffer;
    }

    /* perform attestation of the certificate */
    result = attest(env, that, &parser, height, validateContract);

    /* dispose of the parser */
    dispose((disposable_t*)&parser);

releaseJBuffer:
    (*env)->ReleaseByteArrayElements(env, cert, bufferPtr, JNI_ABORT);

    /* dispose of the parser options */
    dispose((disposable_t*)&parser_options);

    return result;
}

/**
 * Perform the attestation proper and decode the result.
 *
 * \param env           The Java Environment for this call.
 * \param that          The Java CertificateParser object.
 * \param parser        The certificate parser.
 *
 * \returns JNI_TRUE on success and JNI_FALSE on error.  Will throw an
 * appropriate exception on error.
 */
static jboolean attest(
    JNIEnv* env, jobject that, vccert_parser_context_t* parser, jlong height,
    jboolean validateContract)
{
    jthrowable nestedException;

    switch (
        vccert_parser_attest(parser, (uint64_t)height, (bool)validateContract))
    {
        case VCCERT_STATUS_SUCCESS:
            /* update the certificate size so we will only parse data that has
             * been signed. */
            (*env)->SetIntField(
                env, that, CertificateParser_field_size, parser->size);
            return JNI_TRUE;

        case VCCERT_ERROR_PARSER_ATTEST_GENERAL:
            (*env)->ThrowNew(
                env, AttestationException, "general error.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_MISSING_SIGNER_UUID:
            (*env)->ThrowNew(
                env, AttestationException, "certificate missing signer uuid.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_MISSING_SIGNATURE:
            (*env)->ThrowNew(
                env, AttestationException, "certificate missing signature.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_MISSING_SIGNING_CERT:
            /* the entity resolver should have thrown an UnknownEntityException.
             * We will return false here, which will propagate the exception
             * back to the caller.
             */
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_CHAIN_ATTESTATION:
            nestedException = (*env)->ExceptionOccurred(env);
            if (nestedException != NULL)
            {
                jthrowable ex = (jthrowable)
                    (*env)->NewObject(
                        env, AttestationException,
                        AttestationException_init_String_Throwable,
                        "certificate chain attestation failed.",
                        nestedException);
                (*env)->Throw(env, ex);
            }
            else
            {
                (*env)->ThrowNew(
                    env, AttestationException,
                    "certificate chain attestation failed.");
            }
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_SIGNER_UUID_MISMATCH:
            (*env)->ThrowNew(
                env, AttestationException, "signer UUID mismatch.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_SIGNER_MISSING_SIGNING_KEY:
            (*env)->ThrowNew(
                env, AttestationException, "Signing key missing.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_SIGNATURE_MISMATCH:
            (*env)->ThrowNew(
                env, AttestationException, "Signature mismatch.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_MISSING_CONTRACT:
            (*env)->ThrowNew(
                env, AttestationException, "Missing contract.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_CONTRACT_VERIFICATION:
            (*env)->ThrowNew(
                env, AttestationException, "Contract verification failure.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_MISSING_TRANSACTION_TYPE:
            (*env)->ThrowNew(
                env, AttestationException, "Missing transaction type field.");
            return JNI_FALSE;

        case VCCERT_ERROR_PARSER_ATTEST_MISSING_ARTIFACT_ID:
            (*env)->ThrowNew(
                env, AttestationException, "Missing artifact id field.");
            return JNI_FALSE;

        default:
            (*env)->ThrowNew(
                env, AttestationException, "Unknown error.");
            return JNI_FALSE;
    }
}

static bool attest_txn_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                const uint8_t* UNUSED(artifact_id),
                const uint8_t* UNUSED(txn_id),
                vccrypt_buffer_t* UNUSED(output_buffer), bool* UNUSED(trusted))
{
    return false;
}

static int32_t attest_artifact_state_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                const uint8_t* UNUSED(artifact_id),
                vccrypt_buffer_t* UNUSED(txn_id))
{
    return -1;
}

static bool attest_entity_key_resolver(
                void* options, void* UNUSED(parser), uint64_t height,
                const uint8_t* entity_id, vccrypt_buffer_t* pubenckey_buffer,
                vccrypt_buffer_t* pubsignkey_buffer)
{
    parser_user_context_t* ctx = (parser_user_context_t*)
        ((vccert_parser_options_t*)options)->context;
    jthrowable nestedException;

    /* first, we need to convert the entity ID to a valid Java UUID. */
    jobject jEntityId = uuidFromBytes(ctx->env, entity_id);
    nestedException = (*ctx->env)->ExceptionOccurred(ctx->env);
    if (NULL != nestedException)
    {
        /* bubble this up as an attestation exception */
        jthrowable ex = (jthrowable)
            (*ctx->env)->NewObject(
                ctx->env, AttestationException,
                AttestationException_init_String_Throwable,
                "invalid entity uuid",
                nestedException);
        (*ctx->env)->Throw(ctx->env, ex);
        return false;
    }

    /* call the parser delegate resolve method. */
    jobject entref =
        (*ctx->env)->CallObjectMethod(
            ctx->env, ctx->delegate, CertificateParserDelegate_resolveEntity,
            jEntityId, (jlong)height);
    nestedException = (*ctx->env)->ExceptionOccurred(ctx->env);
    if (NULL != nestedException)
    {
        /* This will be an UnknownEntityException, which can be bubbled up
         * naked. */
        return false;
    }
    else if (NULL == entref)
    {
        /* bubble this up as an UnknownEntityException */
        jthrowable ex = (jthrowable)
            (*ctx->env)->NewObject(
                ctx->env, UnknownEntityException,
                UnknownEntityException_init_UUID, jEntityId);
        (*ctx->env)->Throw(ctx->env, ex);
        return false;
    }

    /* get the java byte array from the entity public encryption key. */
    jbyteArray array = (jbyteArray)
        (*ctx->env)->CallObjectMethod(
            ctx->env, entref, EntityReference_getPublicEncryptionKey);
    jsize arrayLen = (*ctx->env)->GetArrayLength(ctx->env, array);

    /* create an output buffer large enough to hold the entity encryption key */
    if (0 != vccrypt_buffer_init(pubenckey_buffer, &alloc_opts, arrayLen))
    {
        (*ctx->env)->ThrowNew(ctx->env, AttestationException, "general error.");
        return false;
    }

    /* copy the certificate data into this buffer */
    jbyte* bufferData =
        (*ctx->env)->GetByteArrayElements(ctx->env, array, NULL);
    memcpy(pubenckey_buffer->data, bufferData, arrayLen);
    (*ctx->env)->ReleaseByteArrayElements(
        ctx->env, array, bufferData, JNI_ABORT);

    /* release local reference to array */
    (*ctx->env)->DeleteLocalRef(ctx->env, array);

    /* get the java byte array from the entity public signing key. */
    array = (jbyteArray)
        (*ctx->env)->CallObjectMethod(
            ctx->env, entref, EntityReference_getPublicSigningKey);
    arrayLen = (*ctx->env)->GetArrayLength(ctx->env, array);

    /* create an output buffer large enough to hold the entity signing key */
    if (0 != vccrypt_buffer_init(pubsignkey_buffer, &alloc_opts, arrayLen))
    {
        (*ctx->env)->ThrowNew(ctx->env, AttestationException, "general error.");
        return false;
    }

    /* copy the certificate data into this buffer */
    bufferData =
        (*ctx->env)->GetByteArrayElements(ctx->env, array, NULL);
    memcpy(pubsignkey_buffer->data, bufferData, arrayLen);
    (*ctx->env)->ReleaseByteArrayElements(
        ctx->env, array, bufferData, JNI_ABORT);

    /* release local references no longer needed */
    (*ctx->env)->DeleteLocalRef(ctx->env, array);
    (*ctx->env)->DeleteLocalRef(ctx->env, entref);

    return true;
}

static vccert_contract_fn_t attest_contract_resolver(
                void* UNUSED(options), void* UNUSED(parser),
                const uint8_t* UNUSED(type_id),
                const uint8_t* UNUSED(artifact_id))
{
    /* TODO - enable contract retrieval */
    return NULL;
}
