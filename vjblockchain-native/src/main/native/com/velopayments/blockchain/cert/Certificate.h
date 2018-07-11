/**
 * \file Certificate.h
 *
 * Class and method exports for Certificate.  This header includes a
 * static registration mechanism for creating global references to the
 * Certificate class, so that Certificate instances can be created
 * from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_CERTIFICATE_HEADER_GUARD
# define PRIVATE_CERTIFICATE_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following Certificate references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Certificate_register(JNIEnv* env);

/* public class com.velopayments.blockchain.cert.Certificate {
 */
extern jclass Certificate;

/* public static com.velopayments.blockchain.cert.Certificate
 * fromByteArray(byte[]);
 * descriptor: ([B)Lcom/velopayments/blockchain/cert/Certificate;
 */
extern jmethodID Certificate_fromByteArray;

/* public byte[] toByteArray();
 * descriptor: ()[B
 */
extern jmethodID Certificate_toByteArray;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_CERTIFICATE_HEADER_GUARD*/
