/**
 * \file Optional.c
 *
 * Class and method exports for Optional.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "Optional.h"

jclass Optional = NULL;
jmethodID Optional_empty = NULL;
jmethodID Optional_of = NULL;
jmethodID Optional_ofNullable = NULL;
jmethodID Optional_get = NULL;
jmethodID Optional_isPresent = NULL;
jmethodID Optional_ifPresent = NULL;
jmethodID Optional_filter = NULL;
jmethodID Optional_map = NULL;
jmethodID Optional_flatMap = NULL;
jmethodID Optional_orElse = NULL;
jmethodID Optional_orElseGet = NULL;
jmethodID Optional_orElseThrow = NULL;
jmethodID Optional_equals = NULL;
jmethodID Optional_hashCode = NULL;
jmethodID Optional_toString = NULL;

static volatile bool Optional_registered = false;

/**
 * Property: Optional globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != Optional \
        NULL != Optional_empty \
        NULL != Optional_of \
        NULL != Optional_ofNullable \
        NULL != Optional_get \
        NULL != Optional_isPresent \
        NULL != Optional_ifPresent \
        NULL != Optional_filter \
        NULL != Optional_map \
        NULL != Optional_flatMap \
        NULL != Optional_orElse \
        NULL != Optional_orElseGet \
        NULL != Optional_orElseThrow \
        NULL != Optional_equals \
        NULL != Optional_hashCode \
        NULL != Optional_toString)

/**
 * Register the following Optional references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Optional_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register Optional once. */
    if (Optional_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register ParserDelegate class */
    tempClassID = (*env)->FindClass(env,
        "java/util/Optional");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    Optional = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == Optional)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register empty method. */
    Optional_empty =
        (*env)->GetStaticMethodID(
            env, Optional, "empty", "()Ljava/util/Optional;");
    if (NULL == Optional_empty)
        return 1;

    /* register of method. */
    Optional_of =
        (*env)->GetStaticMethodID(
            env, Optional, "of", "(Ljava/lang/Object;)Ljava/util/Optional;");
    if (NULL == Optional_of)
        return 1;

    /* register ofNullable method. */
    Optional_ofNullable =
        (*env)->GetStaticMethodID(
            env, Optional, "ofNullable",
            "(Ljava/lang/Object;)Ljava/util/Optional;");
    if (NULL == Optional_ofNullable)
        return 1;

    /* register get method. */
    Optional_get =
        (*env)->GetMethodID(
            env, Optional, "get", "()Ljava/lang/Object;");
    if (NULL == Optional_get)
        return 1;

    /* register isPresent method. */
    Optional_isPresent =
        (*env)->GetMethodID(
            env, Optional, "isPresent", "()Z");
    if (NULL == Optional_isPresent)
        return 1;

    /* register ifPresent method. */
    Optional_ifPresent =
        (*env)->GetMethodID(
            env, Optional, "ifPresent", "(Ljava/util/function/Consumer;)V");
    if (NULL == Optional_ifPresent)
        return 1;

    /* register filter method. */
    Optional_filter =
        (*env)->GetMethodID(
            env, Optional, "filter",
            "(Ljava/util/function/Predicate;)Ljava/util/Optional;");
    if (NULL == Optional_filter)
        return 1;

    /* register map method. */
    Optional_map =
        (*env)->GetMethodID(
            env, Optional, "map",
            "(Ljava/util/function/Function;)Ljava/util/Optional;");
    if (NULL == Optional_map)
        return 1;

    /* register flatMap method. */
    Optional_flatMap =
        (*env)->GetMethodID(
            env, Optional, "flatMap",
            "(Ljava/util/function/Function;)Ljava/util/Optional;");
    if (NULL == Optional_flatMap)
        return 1;

    /* register orElse method. */
    Optional_orElse =
        (*env)->GetMethodID(
            env, Optional, "orElse", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (NULL == Optional_orElse)
        return 1;

    /* register orElseGet method. */
    Optional_orElseGet =
        (*env)->GetMethodID(
            env, Optional, "orElseGet", 
            "(Ljava/util/function/Supplier;)Ljava/lang/Object;");
    if (NULL == Optional_orElseGet)
        return 1;

    /* register orElseThrow method. */
    Optional_orElseThrow =
        (*env)->GetMethodID(
            env, Optional, "orElseThrow",
            "(Ljava/util/function/Supplier;)Ljava/lang/Object;");
    if (NULL == Optional_orElseThrow)
        return 1;

    /* register equals method. */
    Optional_equals =
        (*env)->GetMethodID(
            env, Optional, "equals", "(Ljava/lang/Object;)Z");
    if (NULL == Optional_equals)
        return 1;

    /* register hashCode method. */
    Optional_hashCode =
        (*env)->GetMethodID(
            env, Optional, "hashCode", "()I");
    if (NULL == Optional_hashCode)
        return 1;

    /* register toString method. */
    Optional_toString =
        (*env)->GetMethodID(
            env, Optional, "toString", "()Ljava/lang/String;");
    if (NULL == Optional_toString)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    Optional_registered = true;
    return 0;
}
