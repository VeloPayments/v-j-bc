/**
 * \file CertificateContract.h
 *
 * Class and method exports for CertificateContract.  This header includes a
 * static registration mechanism for creating global references to the
 * CertificateContract class, so that CertificateContract instances can be
 * created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_CERTIFICATECONTRACT_HEADER_GUARD
# define PRIVATE_CERTIFICATECONTRACT_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following CertificateContract references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int CertificateContract_register(JNIEnv* env);

/* public interface com.velopayments.blockchain.cert.CertificateContract {
 */
extern jclass CertificateContract;

/* public abstract boolean
 * verify(com.velopayments.blockchain.cert.CertificateParser);
 * descriptor: (Lcom/velopayments/blockchain/cert/CertificateParser;)Z
 */
extern jmethodID CertificateContract_verify;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_CERTIFICATECONTRACT_HEADER_GUARD*/
