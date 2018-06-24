/**
 * \file EntityReference.c
 *
 * Class and method exports for EntityReference.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "EntityReference.h"

jclass EntityReference = NULL;
jmethodID EntityReference_init = NULL;
jmethodID EntityReference_getPublicEncryptionKey = NULL;
jmethodID EntityReference_getPublicSigningKey = NULL;

static volatile bool EntityReference_registered = false;

/**
 * Property: EntityReference globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != EntityReference \
     && NULL != EntityReference_init \
     && NULL != EntityReference_getPublicEncryptionKey \
     && NULL != EntityReference_getPublicSigningKey)

/**
 * Register the following EntityReference references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EntityReference_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register UnknownEntityException once. */
    if (EntityReference_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register EntityReference class */
    tempClassID = (*env)->FindClass(env,
        "com/velopayments/blockchain/cert/EntityReference");
    if (NULL == tempClassID)
        return 1;

    /* create global class reference */
    EntityReference = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == EntityReference)
        return 1;

    /* we're done with this local reference */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    EntityReference_init =
        (*env)->GetMethodID(
            env, EntityReference, "<init>", "([B[BLjava/util/UUID;)V");
    if (NULL == EntityReference_init)
        return 1;

    /* register init method */
    EntityReference_getPublicEncryptionKey =
        (*env)->GetMethodID(
            env, EntityReference, "getPublicEncryptionKey", "()[B");
    if (NULL == EntityReference_getPublicEncryptionKey)
        return 1;

    /* register init method */
    EntityReference_getPublicSigningKey =
        (*env)->GetMethodID(
            env, EntityReference, "getPublicSigningKey", "()[B");
    if (NULL == EntityReference_getPublicSigningKey)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    EntityReference_registered = true;
    return 0;
}
