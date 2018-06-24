/**
 * \file HashMap.c
 *
 * Class and method exports for java.util.HashMap.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "HashMap.h"

jclass HashMap = NULL;
jmethodID HashMap_init_int_float = NULL;
jmethodID HashMap_init_int = NULL;
jmethodID HashMap_init = NULL;
jmethodID HashMap_init_Map = NULL;
jmethodID HashMap_size = NULL;
jmethodID HashMap_isEmpty = NULL;
jmethodID HashMap_get = NULL;
jmethodID HashMap_containsKey = NULL;
jmethodID HashMap_put = NULL;
jmethodID HashMap_putAll = NULL;
jmethodID HashMap_clear = NULL;
jmethodID HashMap_containsValue = NULL;
jmethodID HashMap_keySet = NULL;
jmethodID HashMap_values = NULL;
jmethodID HashMap_entrySet = NULL;
jmethodID HashMap_getOrDefault = NULL;
jmethodID HashMap_putIfAbsent = NULL;
jmethodID HashMap_remove_Object_Object = NULL;
jmethodID HashMap_remove = NULL;
jmethodID HashMap_replace_if = NULL;
jmethodID HashMap_replace = NULL;
jmethodID HashMap_computeIfAbsent = NULL;
jmethodID HashMap_computeIfPresent = NULL;
jmethodID HashMap_compute = NULL;
jmethodID HashMap_merge = NULL;
jmethodID HashMap_forEach = NULL;
jmethodID HashMap_replaceAll = NULL;
jmethodID HashMap_clone = NULL;

static volatile bool HashMap_registered = false;

/**
 * Property: HashMap globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != HashMap \
     && NULL != HashMap_init_int_float \
     && NULL != HashMap_init_int \
     && NULL != HashMap_init \
     && NULL != HashMap_init_Map \
     && NULL != HashMap_size \
     && NULL != HashMap_isEmpty \
     && NULL != HashMap_get \
     && NULL != HashMap_containsKey \
     && NULL != HashMap_put \
     && NULL != HashMap_putAll \
     && NULL != HashMap_remove \
     && NULL != HashMap_clear \
     && NULL != HashMap_containsValue \
     && NULL != HashMap_keySet \
     && NULL != HashMap_values \
     && NULL != HashMap_entrySet \
     && NULL != HashMap_getOrDefault \
     && NULL != HashMap_putIfAbsent \
     && NULL != HashMap_remove_Object_Object \
     && NULL != HashMap_replace_if \
     && NULL != HashMap_replace \
     && NULL != HashMap_computeIfAbsent \
     && NULL != HashMap_computeIfPresent \
     && NULL != HashMap_compute \
     && NULL != HashMap_merge \
     && NULL != HashMap_forEach \
     && NULL != HashMap_replaceAll \
     && NULL != HashMap_clone)

/**
 * Register the following HashMap references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \param env           Pointer to the Java Environment structure.
 */
int HashMap_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.util.HashMap once. */
    if (HashMap_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register HashMap class */
    tempClassID = (*env)->FindClass(env, "java/util/HashMap");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    HashMap = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == HashMap)
        return 1;

    /* we're done with this reference. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register <init>(int,float) method */
    HashMap_init_int_float =
        (*env)->GetMethodID(env, HashMap, "<init>", "(IF)V");
    if (NULL == HashMap_init_int_float)
        return 1;

    /* register <init>(int) method */
    HashMap_init_int = (*env)->GetMethodID(env, HashMap, "<init>", "(I)V");
    if (NULL == HashMap_init_int)
        return 1;
 
    /* register <init> method */
    HashMap_init = (*env)->GetMethodID(env, HashMap, "<init>", "()V");
    if (NULL == HashMap_init)
        return 1;

    /* register <init>(Map) method */
    HashMap_init_Map =
        (*env)->GetMethodID(env, HashMap, "<init>", "(Ljava/util/Map;)V");
    if (NULL == HashMap_init_Map)
        return 1;

    /* register size method */
    HashMap_size = (*env)->GetMethodID(env, HashMap, "size", "()I");
    if (NULL == HashMap_size)
        return 1;

    /* register isEmpty method */
    HashMap_isEmpty = (*env)->GetMethodID(env, HashMap, "isEmpty", "()Z");
    if (NULL == HashMap_isEmpty)
        return 1;

    /* register get method */
    HashMap_get =
        (*env)->GetMethodID(
            env, HashMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == HashMap_get)
        return 1;

    /* register containsKey method */
    HashMap_containsKey =
        (*env)->GetMethodID(
            env, HashMap, "containsKey", "(Ljava/lang/Object;)Z");
    if (NULL == HashMap_containsKey)
        return 1;

    /* register put method */
    HashMap_put =
        (*env)->GetMethodID(
            env, HashMap, "put",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == HashMap_put)
        return 1;

    /* register putAll method */
    HashMap_putAll =
        (*env)->GetMethodID(env, HashMap, "putAll", "(Ljava/util/Map;)V");
    if (NULL == HashMap_putAll)
        return 1;

    /* register remove method */
    HashMap_remove =
        (*env)->GetMethodID(
            env, HashMap, "remove", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == HashMap_remove)
        return 1;

    /* register clear method */
    HashMap_clear =
        (*env)->GetMethodID(env, HashMap, "clear", "()V");
    if (NULL == HashMap_clear)
        return 1;
 
    /* register containsValue method */
    HashMap_containsValue =
        (*env)->GetMethodID(
            env, HashMap, "containsValue", "(Ljava/lang/Object;)Z");
    if (NULL == HashMap_containsValue)
        return 1;

    /* register keySet method */
    HashMap_keySet =
        (*env)->GetMethodID(env, HashMap, "keySet", "()Ljava/util/Set;");
    if (NULL == HashMap_keySet)
        return 1;

    /* register values method */
    HashMap_values =
        (*env)->GetMethodID(env, HashMap, "values", "()Ljava/util/Collection;");
    if (NULL == HashMap_values)
        return 1;

    /* register entrySet method */
    HashMap_entrySet =
        (*env)->GetMethodID(env, HashMap, "entrySet", "()Ljava/util/Set;");
    if (NULL == HashMap_entrySet)
        return 1;

    /* register getOrDefault method */
    HashMap_getOrDefault =
        (*env)->GetMethodID(
            env, HashMap, "getOrDefault",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == HashMap_getOrDefault)
        return 1;

    /* register putIfAbsent method */
    HashMap_putIfAbsent =
        (*env)->GetMethodID(
            env, HashMap, "putIfAbsent",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == HashMap_putIfAbsent)
        return 1;

    /* register remove method */
    HashMap_remove_Object_Object =
        (*env)->GetMethodID(
            env, HashMap, "remove", "(Ljava/lang/Object;Ljava/lang/Object;)Z");
    if (NULL == HashMap_remove_Object_Object)
        return 1;

    /* register replace_if method */
    HashMap_replace_if =
        (*env)->GetMethodID(
            env, HashMap, "replace",
            "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Z");
    if (NULL == HashMap_replace_if)
        return 1;

    /* register replace method */
    HashMap_replace =
        (*env)->GetMethodID(
            env, HashMap, "replace",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == HashMap_replace)
        return 1;

    /* register computeIfAbsent method */
    HashMap_computeIfAbsent =
        (*env)->GetMethodID(
            env, HashMap, "computeIfAbsent",
            "(Ljava/lang/Object;Ljava/util/function/Function;)"
                "Ljava/lang/Object;");
    if (NULL == HashMap_computeIfAbsent)
        return 1;

    /* register computeIfPresent method */
    HashMap_computeIfPresent =
        (*env)->GetMethodID(
            env, HashMap, "computeIfPresent",
            "(Ljava/lang/Object;Ljava/util/function/BiFunction;)"
                "Ljava/lang/Object;");
    if (NULL == HashMap_computeIfPresent)
        return 1;

    /* register compute method */
    HashMap_compute =
        (*env)->GetMethodID(
            env, HashMap, "compute",
            "(Ljava/lang/Object;Ljava/util/function/BiFunction;)"
                "Ljava/lang/Object;");
    if (NULL == HashMap_compute)
        return 1;

    /* register merge method */
    HashMap_merge =
        (*env)->GetMethodID(
            env, HashMap, "merge",
            "(Ljava/lang/Object;Ljava/lang/Object;"
                "Ljava/util/function/BiFunction;)Ljava/lang/Object;");
    if (NULL == HashMap_merge)
        return 1;

    /* register forEach method */
    HashMap_forEach =
        (*env)->GetMethodID(
            env, HashMap, "forEach", "(Ljava/util/function/BiConsumer;)V");
    if (NULL == HashMap_forEach)
        return 1;

    /* register replaceAll method */
    HashMap_replaceAll =
        (*env)->GetMethodID(
            env, HashMap, "replaceAll", "(Ljava/util/function/BiFunction;)V");
    if (NULL == HashMap_replaceAll)
        return 1;

    /* register clone method */
    HashMap_clone =
        (*env)->GetMethodID(env, HashMap, "clone", "()Ljava/lang/Object;");
    if (NULL == HashMap_clone)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    HashMap_registered = true;
    return 0;
}
