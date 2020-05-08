/**
 * \file CertificateParserDelegate.c
 *
 * Class and method exports for CertificateParserDelegate.  This source file
 * contains a registration method that registers the class and method references
 * just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: CertificateParserDelegate globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->CertificateParserDelegate.classid \
     && NULL != inst->CertificateParserDelegate.resolveEntity \
     && NULL != inst->CertificateParserDelegate.resolveArtifactState)

/**
 * Register the following CertificateParserDelegate references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 * \param inst  native instance to initialize.
 *
 * \returns 0 on success and non-zero on failure.
 */
int
CertificateParserDelegate_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateParserDelegate");
    if (NULL == tempClassID)
        return 1;

    /* create global reference for this class */
    inst->CertificateParserDelegate.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->CertificateParserDelegate.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register resolveEntity method */
    inst->CertificateParserDelegate.resolveEntity =
        (*env)->GetMethodID(
            env, inst->CertificateParserDelegate.classid, "resolveEntity",
                "(Ljava/util/UUID;J)"
                "Lcom/velopayments/blockchain/cert/EntityReference;");
    if (NULL == inst->CertificateParserDelegate.resolveEntity)
        return 1;

    /* register resolveArtifactState method */
    inst->CertificateParserDelegate.resolveArtifactState =
        (*env)->GetMethodID(
            env, inst->CertificateParserDelegate.classid,
            "resolveArtifactState",
            "(Ljava/util/UUID;J)"
            "Lcom/velopayments/blockchain/cert/ArtifactState;");
    if (NULL == inst->CertificateParserDelegate.resolveArtifactState)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
