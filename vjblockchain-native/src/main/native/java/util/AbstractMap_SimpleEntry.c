/**
 * \file AbstractMap_SimpleEntry.c
 *
 * Class and method exports for java.util.AbstractMap.SimpleEntry.  This source
 * file contains a registration method that registers the class and method
 * references just once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "AbstractMap_SimpleEntry.h"

jclass SimpleEntry = NULL;
jmethodID SimpleEntry_init = NULL;
jmethodID SimpleEntry_init_Entry = NULL;
jmethodID SimpleEntry_getKey = NULL;
jmethodID SimpleEntry_getValue = NULL;
jmethodID SimpleEntry_setValue = NULL;
jmethodID SimpleEntry_equals = NULL;
jmethodID SimpleEntry_hashCode = NULL;
jmethodID SimpleEntry_toString = NULL;

static volatile bool SimpleEntry_registered = false;

/**
 * Property: SimpleEntry globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != SimpleEntry \
     && NULL != SimpleEntry_init \
     && NULL != SimpleEntry_init_Entry \
     && NULL != SimpleEntry_getKey \
     && NULL != SimpleEntry_getValue \
     && NULL != SimpleEntry_setValue \
     && NULL != SimpleEntry_equals \
     && NULL != SimpleEntry_hashCode \
     && NULL != SimpleEntry_toString)

/**
 * Register the following SimpleEntry references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int SimpleEntry_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.util.AbstractMap.SimpleEntry once. */
    if (SimpleEntry_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register SimpleEntry class */
    tempClassID = (*env)->FindClass(env, "java/util/AbstractMap$SimpleEntry");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class. */
    SimpleEntry = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == SimpleEntry)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init(Object,Object) method */
    SimpleEntry_init =
        (*env)->GetMethodID(
            env, SimpleEntry, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    if (NULL == SimpleEntry_init)
        return 1;

    /* register init(Map.Entry) method */
    SimpleEntry_init_Entry =
        (*env)->GetMethodID(
            env, SimpleEntry, "<init>", "(Ljava/util/Map$Entry;)V");
    if (NULL == SimpleEntry_init_Entry)
        return 1;

    /* register getKey method */
    SimpleEntry_getKey =
        (*env)->GetMethodID(
            env, SimpleEntry, "getKey", "()Ljava/lang/Object;");
    if (NULL == SimpleEntry_getKey)
        return 1;

    /* register getValue method */
    SimpleEntry_getValue =
        (*env)->GetMethodID(
            env, SimpleEntry, "getValue", "()Ljava/lang/Object;");
    if (NULL == SimpleEntry_getValue)
        return 1;

    /* register setValue method */
    SimpleEntry_setValue =
        (*env)->GetMethodID(
            env, SimpleEntry, "setValue", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == SimpleEntry_setValue)
        return 1;

    /* register equals method */
    SimpleEntry_equals =
        (*env)->GetMethodID(
            env, SimpleEntry, "equals", "(Ljava/lang/Object;)Z");
    if (NULL == SimpleEntry_equals)
        return 1;

    /* register hashCode method */
    SimpleEntry_hashCode =
        (*env)->GetMethodID(
            env, SimpleEntry, "hashCode", "()I");
    if (NULL == SimpleEntry_hashCode)
        return 1;

    /* register toString method */
    SimpleEntry_toString =
        (*env)->GetMethodID(
            env, SimpleEntry, "toString", "()Ljava/lang/String;");
    if (NULL == SimpleEntry_toString)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    SimpleEntry_registered = true;
    return 0;
}
