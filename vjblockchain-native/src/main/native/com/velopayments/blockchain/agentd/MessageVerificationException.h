/**
 * \file MessageVerificationException.h
 *
 * Class and method exports for
 * com.velopayments.blockchain.agentd.MessageVerificationException.  This header
 * includes a static registration mechanism for creating global references to
 * the MessageVerificationException class, so that MessageVerificationException
 * instances can be created from C and methods for these instances can be
 * called from C.
 */

#ifndef  PRIVATE_MESSAGEVERIFICATIONEXCEPTION_HEADER_GUARD
# define PRIVATE_MESSAGEVERIFICATIONEXCEPTION_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following MessageVerificationException references and make them
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
int MessageVerificationException_register(JNIEnv* env);

/* public class com.velopayments.blockchain.agentd.MessageVerificationException
 *          extends java.lang.RuntimeException {
 */
extern jclass MessageVerificationException;

/* public com.velopayments.blockchain.agentd.
 * MessageVerificationException(
 *      java.lang.String);
 * descriptor: (Ljava/lang/String;)V
 */
extern jmethodID MessageVerificationException_init_String;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_MESSAGEVERIFICATIONEXCEPTION_HEADER_GUARD*/
