/**
 * \file com/velopayments/blockchain/cert/AttestationException.h
 *
 * Class and method exports for AttestationException.  This header includes a
 * static registration mechanism for creating global references to the
 * AttestationException class, so that AttestationException instances can be
 * created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_ATTESTATIONEXCEPTION_HEADER_GUARD
# define PRIVATE_ATTESTATIONEXCEPTION_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct AttestationException_JavaVars
AttestationException_JavaVars;

/**
 * Register the following AttestationException references and make them global.
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
AttestationException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java Variables for AttestationException.
 */
struct AttestationException_JavaVars
{
    /* public class com.velopayments.blockchain.cert.AttestationException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public com.velopayments.blockchain.cert.AttestationException(
     *      java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* public com.velopayments.blockchain.cert.AttestationException(
     *      java.lang.String, java.lang.Throwable);
     * descriptor: (Ljava/lang/String;Ljava/lang/Throwable;)V
     */
    jmethodID init_String_Throwable;
};

/* helper macro. */
#define ATTESTATION_EXCEPTION_JAVA_VARS() \
    AttestationException_JavaVars AttestationException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ATTESTATIONEXCEPTION_HEADER_GUARD*/
