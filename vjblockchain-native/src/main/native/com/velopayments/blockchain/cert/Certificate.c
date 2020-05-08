/**
 * \file Certificate.c
 *
 * Class and method exports for Certificate.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: Certificate globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->Certificate.classid \
        NULL != inst->Certificate.fromByteArray \
        NULL != inst->Certificate.toByteArray)

/**
 * Register the following Certificate references and make them global.
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
Certificate_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register Certificate class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/Certificate");
    if (NULL == tempClassID)
        return 1;

    /* create global reference to Certificate. */
    inst->Certificate.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->Certificate.classid)
        return 1;

    /* delete local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register fromByteArray method. */
    inst->Certificate.fromByteArray =
        (*env)->GetStaticMethodID(
            env, inst->Certificate.classid, "fromByteArray",
            "([B)Lcom/velopayments/blockchain/cert/Certificate;");
    if (NULL == inst->Certificate.fromByteArray)
        return 1;

    /* register toByteArray method. */
    inst->Certificate.toByteArray =
        (*env)->GetMethodID(
            env, inst->Certificate.classid, "toByteArray",
            "()[B");
    if (NULL == inst->Certificate.toByteArray)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
