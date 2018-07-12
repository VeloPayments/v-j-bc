/**
 * \file CertificateBuilder.c
 *
 * Class and method exports for CertificateBuilder.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "CertificateBuilder.h"

jclass CertificateBuilder = NULL;
jfieldID CertificateBuilder_field_fields = NULL;
jmethodID CertificateBuilder_addByte = NULL;
jmethodID CertificateBuilder_addShort = NULL;
jmethodID CertificateBuilder_addInt = NULL;
jmethodID CertificateBuilder_addLong = NULL;
jmethodID CertificateBuilder_addUUID = NULL;
jmethodID CertificateBuilder_addString = NULL;
jmethodID CertificateBuilder_addDate = NULL;
jmethodID CertificateBuilder_sign = NULL;

static volatile bool CertificateBuilder_registered = false;

/**
 * Property: CertificateBuilder globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != CertificateBuilder \
     && NULL != CertificateBuilder_field_fields \
     && NULL != CertificateBuilder_addByte \
     && NULL != CertificateBuilder_addShort \
     && NULL != CertificateBuilder_addInt \
     && NULL != CertificateBuilder_addLong \
     && NULL != CertificateBuilder_addUUID \
     && NULL != CertificateBuilder_addString \
     && NULL != CertificateBuilder_addDate \
     && NULL != CertificateBuilder_sign)

/**
 * Register the following CertificateBuilder references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int CertificateBuilder_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register Builder once. */
    if (CertificateBuilder_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register CertificateBuilder class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateBuilder");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to CertificateBuilder. */
    CertificateBuilder = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == CertificateBuilder)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register fields field */
    CertificateBuilder_field_fields =
        (*env)->GetFieldID(
            env, CertificateBuilder, "fields", "Ljava/util/LinkedList;");
    if (NULL == CertificateBuilder_field_fields)
        return 1;

    /* register addByte method. */
    CertificateBuilder_addByte =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addByte",
            "(IB)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addByte)
        return 1;

    /* register addShort method. */
    CertificateBuilder_addShort =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addShort",
            "(II)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addShort)
        return 1;

    /* register addInt method. */
    CertificateBuilder_addInt =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addInt",
            "(II)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addInt)
        return 1;

    /* register addLong method. */
    CertificateBuilder_addLong =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addLong",
            "(IJ)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addLong)
        return 1;

    /* register addUUID method. */
    CertificateBuilder_addUUID =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addUUID",
            "(ILjava/util/UUID;)"
                "Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addUUID)
        return 1;

    /* register addString method. */
    CertificateBuilder_addString =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addString",
            "(ILjava/lang/String;)"
                "Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addString)
        return 1;

    /* register addDate method. */
    CertificateBuilder_addDate =
        (*env)->GetMethodID(
            env, CertificateBuilder, "addDate",
            "(ILjava/util/Date;)"
                "Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == CertificateBuilder_addDate)
        return 1;

    /* register sign method. */
    CertificateBuilder_sign =
        (*env)->GetMethodID(
            env, CertificateBuilder, "sign",
            "(Ljava/util/UUID;"
            "Lcom/velopayments/blockchain/crypt/SigningPrivateKey;)"
            "Lcom/velopayments/blockchain/cert/Certificate;");
    if (NULL == CertificateBuilder_sign)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    CertificateBuilder_registered = true;
    return 0;
}
