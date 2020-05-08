/**
 * \file CertificateParserDelegate.h
 *
 * Class and method exports for CertificateParserDelegate.  This header includes
 * a static registration mechanism for creating global references to the
 * CertificateParserDelegate class, so that CertificateParserDelegate instances
 * can be created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_CERTIFICATE_PARSER_DELEGATE_HEADER_GUARD
# define PRIVATE_CERTIFICATE_PARSER_DELEGATE_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct CertificateParserDelegate_JavaVars
CertificateParserDelegate_JavaVars;

/**
 * Register the following CertificateParserDelegate references and make them
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
CertificateParserDelegate_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for CertificateParserDelegate.
 */
struct CertificateParserDelegate_JavaVars
{
    /* public interface
     * com.velopayments.blockchain.cert.CertificateParserDelegate {
     */
    jclass classid;

    /* public abstract com.velopayments.blockchain.cert.EntityReference
     *      resolveEntity(java.util.UUID, long)
     *      throws com.velopayments.blockchain.cert.UnknownEntityException;
     * descriptor:
     *   (Ljava/util/UUID;J)Lcom/velopayments/blockchain/cert/EntityReference;
     */
    jmethodID resolveEntity;

    /* public abstract com.velopayments.blockchain.cert.ArtifactState
     *      resolveArtifactState(java.util.UUID)
     *      throws com.velopayments.blockchain.cert.UnknownEntityException;
     * descriptor:
     *      (Ljava/util/UUID;)Lcom/velopayments/blockchain/cert/ArtifactState;
     */
    jmethodID resolveArtifactState;
};

/* helper macro. */
#define CERTIFICATE_PARSER_DELEGATE_JAVA_VARS() \
    CertificateParserDelegate_JavaVars CertificateParserDelegate

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_CERTIFICATE_PARSER_DELEGATE_HEADER_GUARD*/
