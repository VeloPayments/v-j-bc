/**
 * \file com/velopayments/blockchain/crypt/MessageAuthenticationException.h
 *
 * Class and method exports for MessageAuthenticationException.  This header
 * includes a static registration mechanism for creating global references to
 * the MessageAuthenticationException class, so that
 * MessageAuthenticationException instances can be created from C and methods
 * for these instances can be called from C.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  PRIVATE_MESSAGEAUTHENTICATIONEXCEPTION_HEADER_GUARD
# define PRIVATE_MESSAGEAUTHENTICATIONEXCEPTION_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct MessageAuthenticationException_JavaVars
MessageAuthenticationException_JavaVars;

/**
 * Register the following MessageAuthenticationException references and make
 * them global.
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
MessageAuthenticationException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for MessageAuthenticationException.
 */
struct MessageAuthenticationException_JavaVars
{
    /* public class
     * com.velopayments.blockchain.crypt.MessageAuthenticationException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public
     * com.velopayments.blockchain.crypt.MessageAuthenticationException(
     *      java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* public com.velopayments.blockchain.crypt.MessageAuthenticationException(
     *      java.lang.String, java.lang.Throwable);
     * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
     */
    jmethodID init_String_Throwable;
};

/* helper macro. */
#define MESSAGE_AUTHENTICATION_EXCEPTION_JAVA_VARS() \
    MessageAuthenticationException_JavaVars MessageAuthenticationException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_MESSAGEAUTHENTICATIONEXCEPTION_HEADER_GUARD*/
