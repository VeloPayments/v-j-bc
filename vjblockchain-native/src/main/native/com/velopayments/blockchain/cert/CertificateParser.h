/**
 * \file CertificateParser.h
 *
 * Class and method exports for CertificateParser.  This header includes a
 * static registration mechanism for creating global references to the
 * CertificateParser class, so that CertificateParser instances can be created
 * from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_CERTIFICATE_PARSER_HEADER_GUARD
# define PRIVATE_CERTIFICATE_PARSER_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct CertificateParser_JavaVars
CertificateParser_JavaVars;

/**
 * Register the following CertificateParser references and make them global.
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
CertificateParser_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for CertificateParser.
 */
struct CertificateParser_JavaVars
{
    /* public class com.velopayments.blockchain.cert.CertificateParser {
     */
    jclass classid;

    /* private byte[] certificate;
     * descriptor: [B
     */
    jfieldID field_certificate;

    /* private int rawSize;
     * descriptor: I
     */
    jfieldID field_rawSize;

    /* private int size;
     * descriptor: I
     */
    jfieldID field_size;

    /* public com.velopayments.blockchain.cert.CertificateParser(byte[]);
     * descriptor: (Lcom/velopayments/blockchain/cert/Certificate;)V
     */
    jmethodID init;

    /* public java.util.Map<java.lang.Integer, java.util.List<byte[]>> parse();
     * descriptor: ()Ljava/util/Map;
     */
    jmethodID parse;

    /* public boolean
     *      attest(com.velopayments.blockchain.cert.CertificateParserDelegate,
                   long, boolean)
     *      throws com.velopayments.blockchain.cert.AttestationException,
     *             com.velopayments.blockchain.cert.UnknownEntityTypeException,
     *             com.velopayments.blockchain.cert.UnknownEntityException;
     * descriptor:
     * (Lcom/velopayments/blockchain/cert/CertificateParserDelegate;JZ)Z
     */
    jmethodID attest;
};

/* helper macro. */
#define CERTIFICATE_PARSER_JAVA_VARS() \
    CertificateParser_JavaVars CertificateParser

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_CERTIFICATE_PARSER_HEADER_GUARD*/
