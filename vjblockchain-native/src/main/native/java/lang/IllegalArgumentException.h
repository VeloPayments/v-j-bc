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

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following IllegalArgumentException references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int IllegalArgumentException_register(JNIEnv* env);

/* public class java.lang.IllegalArgumentException
 *          extends java.lang.RuntimeException {
 */
extern jclass IllegalArgumentException;

/* public java.lang.IllegalArgumentException();
 * descriptor: ()V
 */
extern jmethodID IllegalArgumentException_init;

/* public java.lang.IllegalArgumentException(java.lang.String);
 * descriptor: (Ljava/lang/String;)V
 */
extern jmethodID IllegalArgumentException_String;

/* public java.lang.IllegalArgumentException(
 *          java.lang.String, java.lang.Throwable);
 * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
 */
extern jmethodID IllegalArgumentException_String_Throwable;

/* public java.lang.IllegalArgumentException(java.lang.Throwable);
 * descriptor: (Ljava/lang/Throwable;)V
 */
extern jmethodID IllegalArgumentException_Throwable;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ILLEGALARGUMENTEXCEPTION_HEADER_GUARD*/
