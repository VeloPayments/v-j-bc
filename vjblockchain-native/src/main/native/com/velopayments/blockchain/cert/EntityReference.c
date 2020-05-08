/**
 * \file EntityReference.c
 *
 * Class and method exports for EntityReference.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../init/init.h"

/**
 * Property: EntityReference globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->EntityReference.classid \
     && NULL != inst->EntityReference.init \
     && NULL != inst->EntityReference.getPublicEncryptionKey \
     && NULL != inst->EntityReference.getPublicSigningKey)

/**
 * Register the following EntityReference references and make them global.
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
EntityReference_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register EntityReference class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/EntityReference");
    if (NULL == tempClassID)
        return 1;

    /* create global class reference */
    inst->EntityReference.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->EntityReference.classid)
        return 1;

    /* we're done with this local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->EntityReference.init =
        (*env)->GetMethodID(
            env, inst->EntityReference.classid,
            "<init>", "([B[BLjava/util/UUID;)V");
    if (NULL == inst->EntityReference.init)
        return 1;

    /* register init method */
    inst->EntityReference.getPublicEncryptionKey =
        (*env)->GetMethodID(
            env, inst->EntityReference.classid,
            "getPublicEncryptionKey", "()[B");
    if (NULL == inst->EntityReference.getPublicEncryptionKey)
        return 1;

    /* register init method */
    inst->EntityReference.getPublicSigningKey =
        (*env)->GetMethodID(
            env, inst->EntityReference.classid,
            "getPublicSigningKey", "()[B");
    if (NULL == inst->EntityReference.getPublicSigningKey)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
