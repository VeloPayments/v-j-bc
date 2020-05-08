/**
 * \file Integer.h
 *
 * Class and method exports for java.lang.Integer.  This header includes a
 * static registration mechanism for creating global references to the
 * java.lang.Integer class, so that Integer instances can be created from C and
 * methods for these instances can be called from C.
 */

#ifndef  PRIVATE_INTEGER_HEADER_GUARD
# define PRIVATE_INTEGER_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct Integer_JavaVars
Integer_JavaVars;

/**
 * Register the following Integer references and make them global.
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
Integer_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for Integer.
 */
struct Integer_JavaVars
{
    /* public final class java.lang.Integer
     *          extends java.lang.Number
     *          implements java.lang.Comparable<java.lang.Integer> {
     */
    jclass classid;

    /* public static java.lang.Integer valueOf(int);
     * descriptor: (I)Ljava/lang/Integer;
     */
    jmethodID valueOf;

    /* public int intValue();
     * descriptor: ()I
     */
    jmethodID intValue;
};

/* helper macro. */
#define INTEGER_JAVA_VARS() \
    Integer_JavaVars Integer

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_INTEGER_HEADER_GUARD*/
