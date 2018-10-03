/**
 * \file Initializer_blockchainInit.c
 *
 * Initialize the vcblockchain subsystem.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vccrypt/block_cipher.h>
#include <vccrypt/stream_cipher.h>
#include <vccrypt/suite.h>
#include <vjblockchain.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

#include "init.h"
#include "../../../../com/velopayments/blockchain/cert/AttestationException.h"
#include "../../../../com/velopayments/blockchain/cert/Certificate.h"
#include "../../../../com/velopayments/blockchain/cert/CertificateBuilder.h"
#include "../../../../com/velopayments/blockchain/cert/CertificateContract.h"
#include "../../../../com/velopayments/blockchain/cert/EntityReference.h"
#include "../../../../com/velopayments/blockchain/cert/CertificateParser.h"
#include "../../../../com/velopayments/blockchain/cert/CertificateParserDelegate.h"
#include "../../../../com/velopayments/blockchain/cert/UnknownArtifactException.h"
#include "../../../../com/velopayments/blockchain/cert/UnknownArtifactTypeException.h"
#include "../../../../com/velopayments/blockchain/cert/UnknownEntityException.h"
#include "../../../../com/velopayments/blockchain/client/AgentConnection.h"
#include "../../../../com/velopayments/blockchain/client/TransactionStatus.h"
#include "../../../../com/velopayments/blockchain/client/TransactionSubmissionRequest.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionKeyPair.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionPrivateKey.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionPublicKey.h"
#include "../../../../com/velopayments/blockchain/crypt/Message.h"
#include "../../../../com/velopayments/blockchain/crypt/Signature.h"
#include "../../../../com/velopayments/blockchain/crypt/SigningKeyPair.h"
#include "../../../../com/velopayments/blockchain/crypt/SigningPrivateKey.h"
#include "../../../../com/velopayments/blockchain/crypt/SigningPublicKey.h"
#include "../../../../java/lang/IllegalArgumentException.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/Integer.h"
#include "../../../../java/lang/NullPointerException.h"
#include "../../../../java/util/AbstractMap_SimpleEntry.h"
#include "../../../../java/util/HashMap.h"
#include "../../../../java/util/LinkedList.h"
#include "../../../../java/util/Optional.h"
#include "../../../../java/util/UUID.h"

volatile bool vjblockchain_initialized = false;
allocator_options_t alloc_opts;
vccrypt_suite_options_t crypto_suite;
vccert_builder_options_t builder_opts;

#define INIT_OR_FAIL(text, invocation) \
    if (0 != (invocation)) \
    { \
        (*env)->ThrowNew(env, IllegalStateException, \
                         text " not initialized."); \
        return false; \
    } \
    do { } while (0)

/*
 * Class:     com_velopayments_blockchain_init_Initializer
 * Method:    blockchainInit
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_com_velopayments_blockchain_init_Initializer_blockchainInit(
    JNIEnv* env, jclass UNUSED(that))
{
    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* we need IllegalStateException further on.  If this registration fails,
     * we're dead in the water.
     */
    if (0 != IllegalStateException_register(env))
    {
        return false;
    }

    /* register crypto suite */
    vccrypt_suite_register_velo_v1();

    /* register stream and block ciphers. */
    vccrypt_stream_register_AES_256_2X_CTR();
    vccrypt_block_register_AES_256_2X_CBC();

    /* create allocator used by vjblockchain C methods. */
    malloc_allocator_options_init(&alloc_opts);
    MODEL_ASSERT(MODEL_PROP_VALID_VPR_ALLOCATOR(&alloc_opts));

    /* create crypto suite used by Velo certificates. */
    INIT_OR_FAIL("vccrypt suite options",
                 (vccrypt_suite_options_init(
                        &crypto_suite, &alloc_opts, VCCRYPT_SUITE_VELO_V1)));
    MODEL_ASSERT(MODEL_PROP_VALID_VCCRYPT_CRYPTO_SUITE(&crypto_suite));

    /* create a builder options instance. */
    INIT_OR_FAIL("vccert builder options",
                 (vccert_builder_options_init(
                        &builder_opts, &alloc_opts, &crypto_suite)));
    MODEL_ASSERT(MODEL_PROP_VALID_VCCERT_BUILDER_OPTIONS(&builder_opts));

    /* register Java classes used by vjblockchain. */
    INIT_OR_FAIL("AgentConnection",
                 AgentConnection_register(env));
    INIT_OR_FAIL("AttestationException",
                 AttestationException_register(env));
    INIT_OR_FAIL("Certificate",
                 Certificate_register(env));
    INIT_OR_FAIL("CertificateBuilder",
                 CertificateBuilder_register(env));
    INIT_OR_FAIL("CertificateContract",
                 CertificateContract_register(env));
    INIT_OR_FAIL("CertificateParser",
                 CertificateParser_register(env));
    INIT_OR_FAIL("CertificateParserDelegate",
                 CertificateParserDelegate_register(env));
    INIT_OR_FAIL("EncryptionKeyPair",
                 EncryptionKeyPair_register(env));
    INIT_OR_FAIL("EncryptionPrivateKey",
                 EncryptionPrivateKey_register(env));
    INIT_OR_FAIL("EncryptionPublicKey",
                 EncryptionPublicKey_register(env));
    INIT_OR_FAIL("Message",
                 Message_register(env));
    INIT_OR_FAIL("EntityReference",
                 EntityReference_register(env));
    INIT_OR_FAIL("HashMap",
                 HashMap_register(env));
    INIT_OR_FAIL("IllegalArgumentException",
                 IllegalArgumentException_register(env));
    INIT_OR_FAIL("Integer",
                 Integer_register(env));
    INIT_OR_FAIL("NullPointerException",
                 NullPointerException_register(env));
    INIT_OR_FAIL("LinkedList",
                 LinkedList_register(env));
    INIT_OR_FAIL("Optional",
                 Optional_register(env));
    INIT_OR_FAIL("SimpleEntry",
                 SimpleEntry_register(env));
    INIT_OR_FAIL("Signature",
                 Signature_register(env));
    INIT_OR_FAIL("SigningKeyPair",
                 SigningKeyPair_register(env));
    INIT_OR_FAIL("SigningPrivateKey",
                 SigningPrivateKey_register(env));
    INIT_OR_FAIL("SigningPublicKey",
                 SigningPublicKey_register(env));
    INIT_OR_FAIL("TransactionStatus",
                 TransactionStatus_register(env));
    INIT_OR_FAIL("TransactionSubmissionRequest",
                 TransactionSubmissionRequest_register(env));
    INIT_OR_FAIL("UnknownArtifactException",
                 UnknownArtifactException_register(env));
    INIT_OR_FAIL("UnknownArtifactTypeException",
                 UnknownArtifactTypeException_register(env));
    INIT_OR_FAIL("UnknownEntityException",
                 UnknownEntityException_register(env));
    INIT_OR_FAIL("UUID",
                 UUID_register(env));

    /* we are now initialized */
    vjblockchain_initialized = true;

    return true;
}
