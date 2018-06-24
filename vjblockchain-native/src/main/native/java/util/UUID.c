/**
 * \file UUID.c
 *
 * Class and method exports for UUID.  This source file contains a registration
 * method that registers the class and method references just once, making each
 * of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "UUID.h"

jclass UUID = NULL;
jmethodID UUID_init = NULL;
jmethodID UUID_randomUUID = NULL;
jmethodID UUID_nameUUIDFromBytes = NULL;
jmethodID UUID_fromString = NULL;
jmethodID UUID_getLeastSignificantBits = NULL;
jmethodID UUID_getMostSignificantBits = NULL;
jmethodID UUID_version = NULL;
jmethodID UUID_variant = NULL;
jmethodID UUID_timestamp = NULL;
jmethodID UUID_clockSequence = NULL;
jmethodID UUID_node = NULL;
jmethodID UUID_toString = NULL;
jmethodID UUID_hashCode = NULL;
jmethodID UUID_equals = NULL;
jmethodID UUID_compareTo = NULL;
jmethodID UUID_compareTo_Object = NULL;

static volatile bool UUID_registered = false;

/**
 * Property: UUID globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != UUID \
     && NULL != UUID_init \
     && NULL != UUID_randomUUID \
     && NULL != UUID_nameUUIDFromBytes \
     && NULL != UUID_fromString \
     && NULL != UUID_getLeastSignificantBits \
     && NULL != UUID_getMostSignificantBits \
     && NULL != UUID_version \
     && NULL != UUID_variant \
     && NULL != UUID_timestamp \
     && NULL != UUID_clockSequence \
     && NULL != UUID_node \
     && NULL != UUID_toString \
     && NULL != UUID_hashCode \
     && NULL != UUID_equals \
     && NULL != UUID_compareTo \
     && NULL != UUID_compareTo_Object)

/**
 * Register the following UUID references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int UUID_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register ParserDelegate once. */
    if (UUID_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "java/util/UUID");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    UUID = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == UUID)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    UUID_init =
        (*env)->GetMethodID(
            env, UUID, "<init>", "(JJ)V");
    if (NULL == UUID_init)
        return 1;

    /* register randomUUID method */
    UUID_randomUUID =
        (*env)->GetStaticMethodID(
            env, UUID, "randomUUID", "()Ljava/util/UUID;");
    if (NULL == UUID_randomUUID)
        return 1;

    /* register nameUUIDFromBytes method */
    UUID_nameUUIDFromBytes =
        (*env)->GetStaticMethodID(
            env, UUID, "nameUUIDFromBytes", "([B)Ljava/util/UUID;");
    if (NULL == UUID_nameUUIDFromBytes)
        return 1;

    /* register fromString method */
    UUID_fromString =
        (*env)->GetStaticMethodID(
            env, UUID, "fromString", "(Ljava/lang/String;)Ljava/util/UUID;");
    if (NULL == UUID_fromString)
        return 1;

    /* register getLeastSignificantBits method */
    UUID_getLeastSignificantBits =
        (*env)->GetMethodID(
            env, UUID, "getLeastSignificantBits", "()J");
    if (NULL == UUID_getLeastSignificantBits)
        return 1;

    /* register getMostSignificantBits method */
    UUID_getMostSignificantBits =
        (*env)->GetMethodID(
            env, UUID, "getMostSignificantBits", "()J");
    if (NULL == UUID_getMostSignificantBits)
        return 1;

    /* register version method */
    UUID_version =
        (*env)->GetMethodID(
            env, UUID, "version", "()I");
    if (NULL == UUID_version)
        return 1;

    /* register variant method */
    UUID_variant =
        (*env)->GetMethodID(
            env, UUID, "variant", "()I");
    if (NULL == UUID_variant)
        return 1;

    /* register timestamp method */
    UUID_timestamp =
        (*env)->GetMethodID(
            env, UUID, "timestamp", "()J");
    if (NULL == UUID_timestamp)
        return 1;

    /* register clockSequence method */
    UUID_clockSequence =
        (*env)->GetMethodID(
            env, UUID, "clockSequence", "()I");
    if (NULL == UUID_clockSequence)
        return 1;

    /* register node method */
    UUID_node =
        (*env)->GetMethodID(
            env, UUID, "node", "()J");
    if (NULL == UUID_node)
        return 1;

    /* register toString method */
    UUID_toString =
        (*env)->GetMethodID(
            env, UUID, "toString", "()Ljava/lang/String;");
    if (NULL == UUID_toString)
        return 1;

    /* register hashCode method */
    UUID_hashCode =
        (*env)->GetMethodID(
            env, UUID, "hashCode", "()I");
    if (NULL == UUID_hashCode)
        return 1;

    /* register equals method */
    UUID_equals =
        (*env)->GetMethodID(
            env, UUID, "equals", "(Ljava/lang/Object;)Z");
    if (NULL == UUID_equals)
        return 1;

    /* register compareTo method */
    UUID_compareTo =
        (*env)->GetMethodID(
            env, UUID, "compareTo", "(Ljava/util/UUID;)I");
    if (NULL == UUID_compareTo)
        return 1;

    /* register compareTo(Object) method */
    UUID_compareTo_Object =
        (*env)->GetMethodID(
            env, UUID, "compareTo", "(Ljava/lang/Object;)I");
    if (NULL == UUID_compareTo_Object)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    UUID_registered = true;
    return 0;
}
