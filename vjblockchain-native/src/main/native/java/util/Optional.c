/**
 * \file Optional.c
 *
 * Class and method exports for Optional.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: Optional globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->Optional.classid \
        NULL != inst->Optional.empty \
        NULL != inst->Optional.of \
        NULL != inst->Optional.ofNullable \
        NULL != inst->Optional.get \
        NULL != inst->Optional.isPresent \
        NULL != inst->Optional.ifPresent \
        NULL != inst->Optional.filter \
        NULL != inst->Optional.map \
        NULL != inst->Optional.flatMap \
        NULL != inst->Optional.orElse \
        NULL != inst->Optional.orElseGet \
        NULL != inst->Optional.orElseThrow \
        NULL != inst->Optional.equals \
        NULL != inst->Optional.hashCode \
        NULL != inst->Optional.toString)

/**
 * Register the following Optional references and make them global.
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
Optional_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register class */
    tempClassID = (*env)->FindClass(env,
        "java/util/Optional");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    inst->Optional.classid = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->Optional.classid)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register empty method. */
    inst->Optional.empty =
        (*env)->GetStaticMethodID(
            env, inst->Optional.classid, "empty", "()Ljava/util/Optional;");
    if (NULL == inst->Optional.empty)
        return 1;

    /* register of method. */
    inst->Optional.of =
        (*env)->GetStaticMethodID(
            env, inst->Optional.classid,
            "of", "(Ljava/lang/Object;)Ljava/util/Optional;");
    if (NULL == inst->Optional.of)
        return 1;

    /* register ofNullable method. */
    inst->Optional.ofNullable =
        (*env)->GetStaticMethodID(
            env, inst->Optional.classid, "ofNullable",
            "(Ljava/lang/Object;)Ljava/util/Optional;");
    if (NULL == inst->Optional.ofNullable)
        return 1;

    /* register get method. */
    inst->Optional.get =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "get", "()Ljava/lang/Object;");
    if (NULL == inst->Optional.get)
        return 1;

    /* register isPresent method. */
    inst->Optional.isPresent =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "isPresent", "()Z");
    if (NULL == inst->Optional.isPresent)
        return 1;

    /* register ifPresent method. */
    inst->Optional.ifPresent =
        (*env)->GetMethodID(
            env, inst->Optional.classid,
            "ifPresent", "(Ljava/util/function/Consumer;)V");
    if (NULL == inst->Optional.ifPresent)
        return 1;

    /* register filter method. */
    inst->Optional.filter =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "filter",
            "(Ljava/util/function/Predicate;)Ljava/util/Optional;");
    if (NULL == inst->Optional.filter)
        return 1;

    /* register map method. */
    inst->Optional.map =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "map",
            "(Ljava/util/function/Function;)Ljava/util/Optional;");
    if (NULL == inst->Optional.map)
        return 1;

    /* register flatMap method. */
    inst->Optional.flatMap =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "flatMap",
            "(Ljava/util/function/Function;)Ljava/util/Optional;");
    if (NULL == inst->Optional.flatMap)
        return 1;

    /* register orElse method. */
    inst->Optional.orElse =
        (*env)->GetMethodID(
            env, inst->Optional.classid,
            "orElse", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->Optional.orElse)
        return 1;

    /* register orElseGet method. */
    inst->Optional.orElseGet =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "orElseGet", 
            "(Ljava/util/function/Supplier;)Ljava/lang/Object;");
    if (NULL == inst->Optional.orElseGet)
        return 1;

    /* register orElseThrow method. */
    inst->Optional.orElseThrow =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "orElseThrow",
            "(Ljava/util/function/Supplier;)Ljava/lang/Object;");
    if (NULL == inst->Optional.orElseThrow)
        return 1;

    /* register equals method. */
    inst->Optional.equals =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "equals", "(Ljava/lang/Object;)Z");
    if (NULL == inst->Optional.equals)
        return 1;

    /* register hashCode method. */
    inst->Optional.hashCode =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "hashCode", "()I");
    if (NULL == inst->Optional.hashCode)
        return 1;

    /* register toString method. */
    inst->Optional.toString =
        (*env)->GetMethodID(
            env, inst->Optional.classid, "toString", "()Ljava/lang/String;");
    if (NULL == inst->Optional.toString)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
