/**
 * \file CertificateParser.c
 *
 * Class and method exports for CertificateParser.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "CertificateParser.h"

jclass CertificateParser = NULL;
jfieldID CertificateParser_field_certificate = NULL;
jfieldID CertificateParser_field_rawSize = NULL;
jfieldID CertificateParser_field_size = NULL;
jmethodID CertificateParser_init = NULL;
jmethodID CertificateParser_parse = NULL;
jmethodID CertificateParser_attest = NULL;

static volatile bool CertificateParser_registered = false;

/**
 * Property: CertificateParser globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != CertificateParser \
     && NULL != CertificateParser_field_certificate \
     && NULL != CertificateParser_field_rawSize \
     && NULL != CertificateParser_field_size \
     && NULL != CertificateParser_init \
     && NULL != CertificateParser_parse \
     && NULL != CertificateParser_attest)

/**
 * Register the following CertificateParser references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int CertificateParser_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register ParserDelegate once. */
    if (CertificateParser_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateParser");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    CertificateParser = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == CertificateParser)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register certificate field */
    CertificateParser_field_certificate =
        (*env)->GetFieldID(
            env, CertificateParser, "certificate", "[B");
    if (NULL == CertificateParser_field_certificate)
        return 1;

    /* register rawSize field */
    CertificateParser_field_rawSize =
        (*env)->GetFieldID(
            env, CertificateParser, "rawSize", "I");
    if (NULL == CertificateParser_field_rawSize)
        return 1;

    /* register size field */
    CertificateParser_field_size =
        (*env)->GetFieldID(
            env, CertificateParser, "size", "I");
    if (NULL == CertificateParser_field_size)
        return 1;

    /* register init method */
    CertificateParser_init =
        (*env)->GetMethodID(
            env, CertificateParser, "<init>", "([B)V");
    if (NULL == CertificateParser_init)
        return 1;

    /* register parse method */
    CertificateParser_parse =
        (*env)->GetMethodID(
            env, CertificateParser, "parse", "()Ljava/util/Map;");
    if (NULL == CertificateParser_parse)
        return 1;

    /* register attest method */
    CertificateParser_attest =
        (*env)->GetMethodID(
            env, CertificateParser, "attest",
            "(Lcom/velopayments/blockchain/cert/CertificateParserDelegate;"
            "JZ)Z");
    if (NULL == CertificateParser_attest)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    CertificateParser_registered = true;
    return 0;
}
