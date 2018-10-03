/**
 * \file NullPointerException.h
 *
 * Class and method exports for java.lang.NullPointerException.  This header
 * includes a static registration mechanism for creating global references to
 * the java.lang.NullPointerException class, so that NullPointerException
 * instances can be created from C and methods for these instances can be
 * called from C.
 */

#ifndef  PRIVATE_NULLPOINTEREXCEPTION_HEADER_GUARD
# define PRIVATE_NULLPOINTEREXCEPTION_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following NullPointerException references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int NullPointerException_register(JNIEnv* env);

/* public class java.lang.NullPointerException
 *          extends java.lang.RuntimeException {
 */
extern jclass NullPointerException;

/* public java.lang.NullPointerException();
 * descriptor: ()V
 */
extern jmethodID NullPointerException_init;

/* public java.lang.NullPointerException(java.lang.String);
 * descriptor: (Ljava/lang/String;)V
 */
extern jmethodID NullPointerException_init_String;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_NULLPOINTEREXCEPTION_HEADER_GUARD*/
