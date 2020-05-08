/**
 * \file IllegalStateException.h
 *
 * Class and method exports for java.lang.IllegalStateException.  This header
 * includes a static registration mechanism for creating global references to
 * the java.lang.IllegalStateException class, so that IllegalStateException
 * instances can be created from C and methods for these instances can be
 * called from C.
 */

#ifndef  PRIVATE_ILLEGALSTATEEXCEPTION_HEADER_GUARD
# define PRIVATE_ILLEGALSTATEEXCEPTION_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct IllegalStateException_JavaVars
IllegalStateException_JavaVars;

/**
 * Register the following IllegalStateException references and make them global.
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
IllegalStateException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for IllegalStateException.
 */
struct IllegalStateException_JavaVars
{
    /* public class java.lang.IllegalStateException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public java.lang.IllegalStateException();
     * descriptor: ()V
     */
    jmethodID init;

    /* public java.lang.IllegalStateException(java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* public java.lang.IllegalStateException(
     *      java.lang.String, java.lang.Throwable);
     * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
     */
    jmethodID init_String_Throwable;

    /* public java.lang.IllegalStateException(java.lang.Throwable);
     * descriptor: (Ljava/lang/Throwable;)V
     */
    jmethodID init_Throwable;
};

/* helper macro. */
#define ILLEGAL_STATE_EXCEPTION_JAVA_VARS() \
    IllegalStateException_JavaVars IllegalStateException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ILLEGALSTATEEXCEPTION_HEADER_GUARD*/
