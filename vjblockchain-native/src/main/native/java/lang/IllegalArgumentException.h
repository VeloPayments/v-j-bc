/**
 * \file IllegalArgumentException.h
 *
 * Class and method exports for java.lang.IllegalArgumentException.  This header
 * includes a static registration mechanism for creating global references to
 * the java.lang.IllegalArgumentException class, so that
 * IllegalArgumentException instances can be created from C and methods for
 * these instances can be called from C.
 */

#ifndef  PRIVATE_ILLEGALARGUMENTEXCEPTION_HEADER_GUARD
# define PRIVATE_ILLEGALARGUMENTEXCEPTION_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct IllegalArgumentException_JavaVars
IllegalArgumentException_JavaVars;

/**
 * Register the following IllegalArgumentException references and make them
 * global.
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
IllegalArgumentException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for IllegalArgumentException.
 */
struct IllegalArgumentException_JavaVars
{
    /* public class java.lang.IllegalArgumentException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public java.lang.IllegalArgumentException();
     * descriptor: ()V
     */
    jmethodID init;

    /* public java.lang.IllegalArgumentException(java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* public java.lang.IllegalArgumentException(
     *          java.lang.String, java.lang.Throwable);
     * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
     */
    jmethodID init_String_Throwable;

    /* public java.lang.IllegalArgumentException(java.lang.Throwable);
     * descriptor: (Ljava/lang/Throwable;)V
     */
    jmethodID init_Throwable;
};

/* helper macro. */
#define ILLEGAL_ARGUMENT_EXCEPTION_JAVA_VARS() \
    IllegalArgumentException_JavaVars IllegalArgumentException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ILLEGALARGUMENTEXCEPTION_HEADER_GUARD*/
