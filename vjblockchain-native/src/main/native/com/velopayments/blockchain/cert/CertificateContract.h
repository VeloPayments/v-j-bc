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

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct CertificateContract_JavaVars
CertificateContract_JavaVars;

/**
 * Register the following CertificateContract references and make them global.
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
CertificateContract_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for CertificateContract.
 */
struct CertificateContract_JavaVars
{
    /* public interface com.velopayments.blockchain.cert.CertificateContract {
     */
    jclass classid;

    /* public abstract boolean
     * verify(com.velopayments.blockchain.cert.CertificateParser);
     * descriptor: (Lcom/velopayments/blockchain/cert/CertificateParser;)Z
     */
    jmethodID verify;
};

/* helper macro. */
#define CERTIFICATE_CONTRACT_JAVA_VARS() \
    CertificateContract_JavaVars CertificateContract

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_CERTIFICATECONTRACT_HEADER_GUARD*/
