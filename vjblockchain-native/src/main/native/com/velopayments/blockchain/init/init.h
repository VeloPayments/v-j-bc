/**
 * \file init.h
 *
 * Reference to init flag and Velo C related control structures.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  PRIVATE_INIT_HEADER_GUARD
# define PRIVATE_INIT_HEADER_GUARD

#include <stdbool.h>
#include <vccert/builder.h>
#include <vccert/parser.h>
#include <vccrypt/stream_cipher.h>
#include <vccrypt/suite.h>
#include <vpr/allocator.h>

#include "../../../../com/velopayments/blockchain/agentd/MessageVerificationException.h"
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
#include "../../../../com/velopayments/blockchain/client/TransactionStatus.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionKeyPair.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionPrivateKey.h"
#include "../../../../com/velopayments/blockchain/crypt/EncryptionPublicKey.h"
#include "../../../../com/velopayments/blockchain/crypt/InvalidKeySizeException.h"
#include "../../../../com/velopayments/blockchain/crypt/Key.h"
#include "../../../../com/velopayments/blockchain/crypt/Message.h"
#include "../../../../com/velopayments/blockchain/crypt/MessageAuthenticationException.h"
#include "../../../../com/velopayments/blockchain/crypt/Signature.h"
#include "../../../../com/velopayments/blockchain/crypt/SigningKeyPair.h"
#include "../../../../com/velopayments/blockchain/crypt/SigningPrivateKey.h"
#include "../../../../com/velopayments/blockchain/crypt/SigningPublicKey.h"
#include "../../../../com/velopayments/blockchain/crypt/SimpleStreamCipher.h"
#include "../../../../java/lang/IllegalArgumentException.h"
#include "../../../../java/lang/IllegalStateException.h"
#include "../../../../java/lang/Integer.h"
#include "../../../../java/lang/NullPointerException.h"
#include "../../../../java/util/AbstractMap_SimpleEntry.h"
#include "../../../../java/util/HashMap.h"
#include "../../../../java/util/LinkedList.h"
#include "../../../../java/util/Optional.h"
#include "../../../../java/util/UUID.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

struct vjblockchain_native_instance;
typedef struct vjblockchain_native_instance vjblockchain_native_instance;

struct vjblockchain_native_instance
{
    bool initialized;
    allocator_options_t alloc_opts;
    vccrypt_suite_options_t crypto_suite;
    vccert_builder_options_t builder_opts;

    ATTESTATION_EXCEPTION_JAVA_VARS();
    CERTIFICATE_JAVA_VARS();
    CERTIFICATE_BUILDER_JAVA_VARS();
    CERTIFICATE_CONTRACT_JAVA_VARS();
    CERTIFICATE_PARSER_JAVA_VARS();
    CERTIFICATE_PARSER_DELEGATE_JAVA_VARS();
    ENCRYPTION_KEY_PAIR_JAVA_VARS();
    ENCRYPTION_PRIVATE_KEY_JAVA_VARS();
    ENCRYPTION_PUBLIC_KEY_JAVA_VARS();
    INVALID_KEY_SIZE_EXCEPTION_JAVA_VARS();
    MESSAGE_JAVA_VARS();
    MESSAGE_VERIFICATION_EXCEPTION_JAVA_VARS();
    ENTITY_REFERENCE_JAVA_VARS();
    HASH_MAP_JAVA_VARS();
    ILLEGAL_ARGUMENT_EXCEPTION_JAVA_VARS();
    ILLEGAL_STATE_EXCEPTION_JAVA_VARS();
    INTEGER_JAVA_VARS();
    KEY_JAVA_VARS();
    LINKED_LIST_JAVA_VARS();
    MESSAGE_AUTHENTICATION_EXCEPTION_JAVA_VARS();
    NULL_POINTER_EXCEPTION_JAVA_VARS();
    OPTIONAL_JAVA_VARS();
    SIMPLE_ENTRY_JAVA_VARS();
    SIGNATURE_JAVA_VARS();
    SIGNING_KEY_PAIR_JAVA_VARS();
    SIGNING_PRIVATE_KEY_JAVA_VARS();
    SIGNING_PUBLIC_KEY_JAVA_VARS();
    SIMPLE_STREAM_CIPHER_JAVA_VARS();
    TRANSACTION_STATUS_JAVA_VARS();
    UNKNOWN_ARTIFACT_EXCEPTION_JAVA_VARS();
    UNKNOWN_ARTIFACT_TYPE_EXCEPTION_JAVA_VARS();
    UNKNOWN_ENTITY_EXCEPTION_JAVA_VARS();
    UUID_JAVA_VARS();
};

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_INIT_HEADER_GUARD*/
