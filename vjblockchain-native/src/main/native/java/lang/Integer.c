/**
 * \file Integer.c
 *
 * Class and method exports for java.lang.Integer.  This source file contains a
 * registration method that registers the class and method references just once,
 * making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "Integer.h"

jclass Integer = NULL;
jmethodID Integer_valueOf = NULL;
jmethodID Integer_intValue = NULL;

static volatile bool Integer_registered = false;

/**
 * Property: Integer globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != Integer \
     && NULL != Integer_valueOf \
     && NULL != Integer_intValue)

/**
 * Register the following Integer references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Integer_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.lang.Integer once. */
    if (Integer_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register Integer class */
    tempClassID = (*env)->FindClass(env, "java/lang/Integer");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference for this class */
    Integer = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == Integer)
        return 1;

    /* we don't need this local reference anymore. */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register valueOf method */
    Integer_valueOf =
        (*env)->GetStaticMethodID(
            env, Integer, "valueOf", "(I)Ljava/lang/Integer;");
    if (NULL == Integer_valueOf)
        return 1;

    /* register intValue method */
    Integer_intValue =
        (*env)->GetMethodID(
            env, Integer, "intValue", "()I");
    if (NULL == Integer_intValue)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    Integer_registered = true;
    return 0;
}
