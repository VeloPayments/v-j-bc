/**
 * \file UUID.c
 *
 * Class and method exports for UUID.  This source file contains a registration
 * method that registers the class and method references just once, making each
 * of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: UUID globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->UUID.classid \
     && NULL != inst->UUID.init \
     && NULL != inst->UUID.randomUUID \
     && NULL != inst->UUID.nameUUIDFromBytes \
     && NULL != inst->UUID.fromString \
     && NULL != inst->UUID.getLeastSignificantBits \
     && NULL != inst->UUID.getMostSignificantBits \
     && NULL != inst->UUID.version \
     && NULL != inst->UUID.variant \
     && NULL != inst->UUID.timestamp \
     && NULL != inst->UUID.clockSequence \
     && NULL != inst->UUID.node \
     && NULL != inst->UUID.toString \
     && NULL != inst->UUID.hashCode \
     && NULL != inst->UUID.equals \
     && NULL != inst->UUID.compareTo \
     && NULL != inst->UUID_compareTo_Object)

/**
 * Register the following UUID references and make them global.
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
UUID_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "java/util/UUID");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    inst->UUID.classid = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->UUID.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init method */
    inst->UUID.init =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "<init>", "(JJ)V");
    if (NULL == inst->UUID.init)
        return 1;

    /* register randomUUID method */
    inst->UUID.randomUUID =
        (*env)->GetStaticMethodID(
            env, inst->UUID.classid, "randomUUID", "()Ljava/util/UUID;");
    if (NULL == inst->UUID.randomUUID)
        return 1;

    /* register nameUUIDFromBytes method */
    inst->UUID.nameUUIDFromBytes =
        (*env)->GetStaticMethodID(
            env, inst->UUID.classid,
            "nameUUIDFromBytes", "([B)Ljava/util/UUID;");
    if (NULL == inst->UUID.nameUUIDFromBytes)
        return 1;

    /* register fromString method */
    inst->UUID.fromString =
        (*env)->GetStaticMethodID(
            env, inst->UUID.classid,
            "fromString", "(Ljava/lang/String;)Ljava/util/UUID;");
    if (NULL == inst->UUID.fromString)
        return 1;

    /* register getLeastSignificantBits method */
    inst->UUID.getLeastSignificantBits =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "getLeastSignificantBits", "()J");
    if (NULL == inst->UUID.getLeastSignificantBits)
        return 1;

    /* register getMostSignificantBits method */
    inst->UUID.getMostSignificantBits =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "getMostSignificantBits", "()J");
    if (NULL == inst->UUID.getMostSignificantBits)
        return 1;

    /* register version method */
    inst->UUID.version =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "version", "()I");
    if (NULL == inst->UUID.version)
        return 1;

    /* register variant method */
    inst->UUID.variant =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "variant", "()I");
    if (NULL == inst->UUID.variant)
        return 1;

    /* register timestamp method */
    inst->UUID.timestamp =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "timestamp", "()J");
    if (NULL == inst->UUID.timestamp)
        return 1;

    /* register clockSequence method */
    inst->UUID.clockSequence =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "clockSequence", "()I");
    if (NULL == inst->UUID.clockSequence)
        return 1;

    /* register node method */
    inst->UUID.node =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "node", "()J");
    if (NULL == inst->UUID.node)
        return 1;

    /* register toString method */
    inst->UUID.toString =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "toString", "()Ljava/lang/String;");
    if (NULL == inst->UUID.toString)
        return 1;

    /* register hashCode method */
    inst->UUID.hashCode =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "hashCode", "()I");
    if (NULL == inst->UUID.hashCode)
        return 1;

    /* register equals method */
    inst->UUID.equals =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "equals", "(Ljava/lang/Object;)Z");
    if (NULL == inst->UUID.equals)
        return 1;

    /* register compareTo method */
    inst->UUID.compareTo =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "compareTo", "(Ljava/util/UUID;)I");
    if (NULL == inst->UUID.compareTo)
        return 1;

    /* register compareTo(Object) method */
    inst->UUID.compareTo_Object =
        (*env)->GetMethodID(
            env, inst->UUID.classid, "compareTo", "(Ljava/lang/Object;)I");
    if (NULL == inst->UUID.compareTo_Object)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
