/**
 * \file HashMap.c
 *
 * Class and method exports for java.util.HashMap.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: HashMap globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->HashMap.classid \
     && NULL != inst->HashMap.init_int_float \
     && NULL != inst->HashMap.init_int \
     && NULL != inst->HashMap.init \
     && NULL != inst->HashMap.init_Map \
     && NULL != inst->HashMap.size \
     && NULL != inst->HashMap.isEmpty \
     && NULL != inst->HashMap.get \
     && NULL != inst->HashMap.containsKey \
     && NULL != inst->HashMap.put \
     && NULL != inst->HashMap.putAll \
     && NULL != inst->HashMap.remove \
     && NULL != inst->HashMap.clear \
     && NULL != inst->HashMap.containsValue \
     && NULL != inst->HashMap.keySet \
     && NULL != inst->HashMap.values \
     && NULL != inst->HashMap.entrySet \
     && NULL != inst->HashMap.getOrDefault \
     && NULL != inst->HashMap.putIfAbsent \
     && NULL != inst->HashMap.remove_Object_Object \
     && NULL != inst->HashMap.replace_if \
     && NULL != inst->HashMap.replace \
     && NULL != inst->HashMap.computeIfAbsent \
     && NULL != inst->HashMap.computeIfPresent \
     && NULL != inst->HashMap.compute \
     && NULL != inst->HashMap.merge \
     && NULL != inst->HashMap.forEach \
     && NULL != inst->HashMap.replaceAll \
     && NULL != inst->HashMap.clone)

/**
 * Register the following HashMap references and make them global.
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
HashMap_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register HashMap class */
    tempClassID = (*env)->FindClass(env, "java/util/HashMap");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class. */
    inst->HashMap.classid =
        (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->HashMap.classid)
        return 1;

    /* we're done with this reference. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register <init>(int,float) method */
    inst->HashMap.init_int_float =
        (*env)->GetMethodID(env, inst->HashMap.classid, "<init>", "(IF)V");
    if (NULL == inst->HashMap.init_int_float)
        return 1;

    /* register <init>(int) method */
    inst->HashMap.init_int =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "<init>", "(I)V");
    if (NULL == inst->HashMap.init_int)
        return 1;
 
    /* register <init> method */
    inst->HashMap.init =
        (*env)->GetMethodID(env, inst->HashMap.classid, "<init>", "()V");
    if (NULL == inst->HashMap.init)
        return 1;

    /* register <init>(Map) method */
    inst->HashMap.init_Map =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "<init>", "(Ljava/util/Map;)V");
    if (NULL == inst->HashMap.init_Map)
        return 1;

    /* register size method */
    inst->HashMap.size =
        (*env)->GetMethodID(env, inst->HashMap.classid, "size", "()I");
    if (NULL == inst->HashMap.size)
        return 1;

    /* register isEmpty method */
    inst->HashMap.isEmpty =
        (*env)->GetMethodID(env, inst->HashMap.classid, "isEmpty", "()Z");
    if (NULL == inst->HashMap.isEmpty)
        return 1;

    /* register get method */
    inst->HashMap.get =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.get)
        return 1;

    /* register containsKey method */
    inst->HashMap.containsKey =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "containsKey", "(Ljava/lang/Object;)Z");
    if (NULL == inst->HashMap.containsKey)
        return 1;

    /* register put method */
    inst->HashMap.put =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "put",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.put)
        return 1;

    /* register putAll method */
    inst->HashMap.putAll =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "putAll", "(Ljava/util/Map;)V");
    if (NULL == inst->HashMap.putAll)
        return 1;

    /* register remove method */
    inst->HashMap.remove =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "remove", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.remove)
        return 1;

    /* register clear method */
    inst->HashMap.clear =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "clear", "()V");
    if (NULL == inst->HashMap.clear)
        return 1;
 
    /* register containsValue method */
    inst->HashMap.containsValue =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "containsValue", "(Ljava/lang/Object;)Z");
    if (NULL == inst->HashMap.containsValue)
        return 1;

    /* register keySet method */
    inst->HashMap.keySet =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "keySet", "()Ljava/util/Set;");
    if (NULL == inst->HashMap.keySet)
        return 1;

    /* register values method */
    inst->HashMap.values =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "values", "()Ljava/util/Collection;");
    if (NULL == inst->HashMap.values)
        return 1;

    /* register entrySet method */
    inst->HashMap.entrySet =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "entrySet", "()Ljava/util/Set;");
    if (NULL == inst->HashMap.entrySet)
        return 1;

    /* register getOrDefault method */
    inst->HashMap.getOrDefault =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "getOrDefault",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.getOrDefault)
        return 1;

    /* register putIfAbsent method */
    inst->HashMap.putIfAbsent =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "putIfAbsent",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.putIfAbsent)
        return 1;

    /* register remove method */
    inst->HashMap.remove_Object_Object =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "remove", "(Ljava/lang/Object;Ljava/lang/Object;)Z");
    if (NULL == inst->HashMap.remove_Object_Object)
        return 1;

    /* register replace_if method */
    inst->HashMap.replace_if =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "replace",
            "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Z");
    if (NULL == inst->HashMap.replace_if)
        return 1;

    /* register replace method */
    inst->HashMap.replace =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "replace",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.replace)
        return 1;

    /* register computeIfAbsent method */
    inst->HashMap.computeIfAbsent =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "computeIfAbsent",
            "(Ljava/lang/Object;Ljava/util/function/Function;)"
                "Ljava/lang/Object;");
    if (NULL == inst->HashMap.computeIfAbsent)
        return 1;

    /* register computeIfPresent method */
    inst->HashMap.computeIfPresent =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "computeIfPresent",
            "(Ljava/lang/Object;Ljava/util/function/BiFunction;)"
                "Ljava/lang/Object;");
    if (NULL == inst->HashMap.computeIfPresent)
        return 1;

    /* register compute method */
    inst->HashMap.compute =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "compute",
            "(Ljava/lang/Object;Ljava/util/function/BiFunction;)"
                "Ljava/lang/Object;");
    if (NULL == inst->HashMap.compute)
        return 1;

    /* register merge method */
    inst->HashMap.merge =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "merge",
            "(Ljava/lang/Object;Ljava/lang/Object;"
                "Ljava/util/function/BiFunction;)Ljava/lang/Object;");
    if (NULL == inst->HashMap.merge)
        return 1;

    /* register forEach method */
    inst->HashMap.forEach =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "forEach", "(Ljava/util/function/BiConsumer;)V");
    if (NULL == inst->HashMap.forEach)
        return 1;

    /* register replaceAll method */
    inst->HashMap.replaceAll =
        (*env)->GetMethodID(
            env, inst->HashMap.classid,
            "replaceAll", "(Ljava/util/function/BiFunction;)V");
    if (NULL == inst->HashMap.replaceAll)
        return 1;

    /* register clone method */
    inst->HashMap.clone =
        (*env)->GetMethodID(
            env, inst->HashMap.classid, "clone", "()Ljava/lang/Object;");
    if (NULL == inst->HashMap.clone)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
