/**
 * \file com/velopayments/blockchain/crypt/InvalidKeySizeException.h
 *
 * Class and method exports for InvalidKeySizeException.  This header includes a
 * static registration mechanism for creating global references to the
 * InvalidKeySizeException class, so that InvalidKeySizeException instances can
 * be created from C and methods for these instances can be called from C.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  PRIVATE_INVALIDKEYSIZEEXCEPTION_HEADER_GUARD
# define PRIVATE_INVALIDKEYSIZEEXCEPTION_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct InvalidKeySizeException_JavaVars
InvalidKeySizeException_JavaVars;

/**
 * Register the following InvalidKeySizeException references and make them
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
InvalidKeySizeException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for InvalidKeySizeException.
 */
struct InvalidKeySizeException_JavaVars
{
    /* public class com.velopayments.blockchain.crypt.InvalidKeySizeException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public com.velopayments.blockchain.crypt.InvalidKeySizeException(
     *      java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* public com.velopayments.blockchain.crypt.InvalidKeySizeException(
     *      java.lang.String, java.lang.Throwable);
     * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
     */
    jmethodID init_String_Throwable;
};

/* helper macro. */
#define INVALID_KEY_SIZE_EXCEPTION_JAVA_VARS() \
    InvalidKeySizeException_JavaVars InvalidKeySizeException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_INVALIDKEYSIZEEXCEPTION_HEADER_GUARD*/
