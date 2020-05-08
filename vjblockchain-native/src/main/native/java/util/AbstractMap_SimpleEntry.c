/**
 * \file AbstractMap_SimpleEntry.c
 *
 * Class and method exports for java.util.AbstractMap.SimpleEntry.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: SimpleEntry globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->SimpleEntry.classid \
     && NULL != inst->SimpleEntry.init \
     && NULL != inst->SimpleEntry.init_Entry \
     && NULL != inst->SimpleEntry.getKey \
     && NULL != inst->SimpleEntry.getValue \
     && NULL != inst->SimpleEntry.setValue \
     && NULL != inst->SimpleEntry.equals \
     && NULL != inst->SimpleEntry.hashCode \
     && NULL != inst->SimpleEntry.toString)

/**
 * Register the following SimpleEntry references and make them global.
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
SimpleEntry_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register SimpleEntry class */
    tempClassID = (*env)->FindClass(env, "java/util/AbstractMap$SimpleEntry");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class. */
    inst->SimpleEntry.classid = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->SimpleEntry.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(Object,Object) method */
    inst->SimpleEntry.init =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid,
            "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    if (NULL == inst->SimpleEntry.init)
        return 1;

    /* register init(Map.Entry) method */
    inst->SimpleEntry.init_Entry =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid,
            "<init>", "(Ljava/util/Map$Entry;)V");
    if (NULL == inst->SimpleEntry.init_Entry)
        return 1;

    /* register getKey method */
    inst->SimpleEntry.getKey =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid, "getKey", "()Ljava/lang/Object;");
    if (NULL == inst->SimpleEntry.getKey)
        return 1;

    /* register getValue method */
    inst->SimpleEntry.getValue =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid,
            "getValue", "()Ljava/lang/Object;");
    if (NULL == inst->SimpleEntry.getValue)
        return 1;

    /* register setValue method */
    inst->SimpleEntry.setValue =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid,
            "setValue", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->SimpleEntry.setValue)
        return 1;

    /* register equals method */
    inst->SimpleEntry.equals =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid, "equals", "(Ljava/lang/Object;)Z");
    if (NULL == inst->SimpleEntry.equals)
        return 1;

    /* register hashCode method */
    inst->SimpleEntry.hashCode =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid, "hashCode", "()I");
    if (NULL == inst->SimpleEntry.hashCode)
        return 1;

    /* register toString method */
    inst->SimpleEntry.toString =
        (*env)->GetMethodID(
            env, inst->SimpleEntry.classid, "toString", "()Ljava/lang/String;");
    if (NULL == inst->SimpleEntry.toString)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
