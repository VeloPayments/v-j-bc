/**
 * \file CertificateParserDelegate.c
 *
 * Class and method exports for CertificateParserDelegate.  This source file
 * contains a registration method that registers the class and method references
 * just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "CertificateParserDelegate.h"

jclass CertificateParserDelegate = NULL;
jmethodID CertificateParserDelegate_resolveEntity = NULL;
jmethodID CertificateParserDelegate_resolveArtifactState = NULL;

static volatile bool CertificateParserDelegate_registered = false;

/**
 * Property: CertificateParserDelegate globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != CertificateParserDelegate \
     && NULL != CertificateParserDelegate_resolveEntity \
     && NULL != CertificateParserDelegate_resolveArtifactState)

/**
 * Register the following CertificateParserDelegate references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int CertificateParserDelegate_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register ParserDelegate once. */
    if (CertificateParserDelegate_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateParserDelegate");
    if (NULL == tempClassID)
        return 1;

    /* create global reference for this class */
    CertificateParserDelegate = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == CertificateParserDelegate)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register resolveEntity method */
    CertificateParserDelegate_resolveEntity =
        (*env)->GetMethodID(
            env, CertificateParserDelegate, "resolveEntity",
                "(Ljava/util/UUID;J)"
                "Lcom/velopayments/blockchain/cert/EntityReference;");
    if (NULL == CertificateParserDelegate_resolveEntity)
        return 1;

    /* register resolveArtifactState method */
    CertificateParserDelegate_resolveArtifactState =
        (*env)->GetMethodID(
            env, CertificateParserDelegate, "resolveArtifactState",
            "(Ljava/util/UUID;J)"
            "Lcom/velopayments/blockchain/cert/ArtifactState;");
    if (NULL == CertificateParserDelegate_resolveArtifactState)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    CertificateParserDelegate_registered = true;
    return 0;
}
