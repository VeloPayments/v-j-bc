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

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following IllegalStateException references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int IllegalStateException_register(JNIEnv* env);

/* public class java.lang.IllegalStateException
 *          extends java.lang.RuntimeException {
 */
extern jclass IllegalStateException;

/* public java.lang.IllegalStateException();
 * descriptor: ()V
 */
extern jmethodID IllegalStateException_init;

/* public java.lang.IllegalStateException(java.lang.String);
 * descriptor: (Ljava/lang/String;)V
 */
extern jmethodID IllegalStateException_init_String;

/* public java.lang.IllegalStateException(
 *      java.lang.String, java.lang.Throwable);
 * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
 */
extern jmethodID IllegalStateException_String_Throwable;

/* public java.lang.IllegalStateException(java.lang.Throwable);
 * descriptor: (Ljava/lang/Throwable;)V
 */
extern jmethodID IllegalStateException_Throwable;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ILLEGALSTATEEXCEPTION_HEADER_GUARD*/
