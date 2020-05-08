/**
 * \file CertificateBuilder.c
 *
 * Class and method exports for CertificateBuilder.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: CertificateBuilder globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->CertificateBuilder.classid \
     && NULL != inst->CertificateBuilder.field_fields \
     && NULL != inst->CertificateBuilder.addByte \
     && NULL != inst->CertificateBuilder.addShort \
     && NULL != inst->CertificateBuilder.addInt \
     && NULL != inst->CertificateBuilder.addLong \
     && NULL != inst->CertificateBuilder.addUUID \
     && NULL != inst->CertificateBuilder.addString \
     && NULL != inst->CertificateBuilder.addDate \
     && NULL != inst->CertificateBuilder.sign)

/**
 * Register the following CertificateBuilder references and make them global.
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
CertificateBuilder_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register CertificateBuilder class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/CertificateBuilder");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to CertificateBuilder. */
    inst->CertificateBuilder.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->CertificateBuilder.classid)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register fields field */
    inst->CertificateBuilder.field_fields =
        (*env)->GetFieldID(
            env, inst->CertificateBuilder.classid,
            "fields", "Ljava/util/LinkedList;");
    if (NULL == inst->CertificateBuilder.field_fields)
        return 1;

    /* register addByte method. */
    inst->CertificateBuilder.addByte =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addByte",
            "(IB)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addByte)
        return 1;

    /* register addShort method. */
    inst->CertificateBuilder.addShort =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addShort",
            "(II)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addShort)
        return 1;

    /* register addInt method. */
    inst->CertificateBuilder.addInt =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addInt",
            "(II)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addInt)
        return 1;

    /* register addLong method. */
    inst->CertificateBuilder.addLong =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addLong",
            "(IJ)Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addLong)
        return 1;

    /* register addUUID method. */
    inst->CertificateBuilder.addUUID =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addUUID",
            "(ILjava/util/UUID;)"
                "Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addUUID)
        return 1;

    /* register addString method. */
    inst->CertificateBuilder.addString =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addString",
            "(ILjava/lang/String;)"
                "Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addString)
        return 1;

    /* register addDate method. */
    inst->CertificateBuilder.addDate =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "addDate",
            "(ILjava/util/Date;)"
                "Lcom/velopayments/blockchain/cert/CertificateBuilder;");
    if (NULL == inst->CertificateBuilder.addDate)
        return 1;

    /* register sign method. */
    inst->CertificateBuilder.sign =
        (*env)->GetMethodID(
            env, inst->CertificateBuilder.classid, "sign",
            "(Ljava/util/UUID;"
            "Lcom/velopayments/blockchain/crypt/SigningPrivateKey;)"
            "Lcom/velopayments/blockchain/cert/Certificate;");
    if (NULL == inst->CertificateBuilder.sign)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
