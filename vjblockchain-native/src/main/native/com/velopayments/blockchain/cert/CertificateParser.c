/**
 * \file CertificateParser.c
 *
 * Class and method exports for CertificateParser.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: CertificateParser globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->CertificateParser.classid \
     && NULL != inst->CertificateParser.field_certificate \
     && NULL != inst->CertificateParser.field_rawSize \
     && NULL != inst->CertificateParser.field_size \
     && NULL != inst->CertificateParser.init \
     && NULL != inst->CertificateParser.parse \
     && NULL != inst->CertificateParser.attest)

/**
 * Register the following CertificateParser references and make them global.
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
CertificateParser_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateParser");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    inst->CertificateParser.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->CertificateParser.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register certificate field */
    inst->CertificateParser.field_certificate =
        (*env)->GetFieldID(
            env, inst->CertificateParser.classid, "certificate", "[B");
    if (NULL == inst->CertificateParser.field_certificate)
        return 1;

    /* register rawSize field */
    inst->CertificateParser.field_rawSize =
        (*env)->GetFieldID(
            env, inst->CertificateParser.classid, "rawSize", "I");
    if (NULL == inst->CertificateParser.field_rawSize)
        return 1;

    /* register size field */
    inst->CertificateParser.field_size =
        (*env)->GetFieldID(
            env, inst->CertificateParser.classid, "size", "I");
    if (NULL == inst->CertificateParser.field_size)
        return 1;

    /* register init method */
    inst->CertificateParser.init =
        (*env)->GetMethodID(
            env, inst->CertificateParser.classid, "<init>",
            "(Lcom/velopayments/blockchain/cert/Certificate;)V");
    if (NULL == inst->CertificateParser.init)
        return 1;

    /* register parse method */
    inst->CertificateParser.parse =
        (*env)->GetMethodID(
            env, inst->CertificateParser.classid, "parse", "()Ljava/util/Map;");
    if (NULL == inst->CertificateParser.parse)
        return 1;

    /* register attest method */
    inst->CertificateParser.attest =
        (*env)->GetMethodID(
            env, inst->CertificateParser.classid, "attest",
            "(Lcom/velopayments/blockchain/cert/CertificateParserDelegate;"
            "JZ)Z");
    if (NULL == inst->CertificateParser.attest)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
