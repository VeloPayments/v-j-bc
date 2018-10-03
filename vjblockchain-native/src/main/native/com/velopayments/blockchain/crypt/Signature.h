/**
 * \file Signature.h
 *
 * Class and method exports for Signature.  This header includes a static
 * registration mechanism for creating global references to the Signature class,
 * so that Signature instances can be created from C and methods for these
 * instances can be called from C.
 */

#ifndef  PRIVATE_SIGNATURE_HEADER_GUARD
# define PRIVATE_SIGNATURE_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following Signature references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Signature_register(JNIEnv* env);

/* public class com.velopayments.blockchain.crypt.Signature {
 */
extern jclass Signature;

/* public com.velopayments.blockchain.crypt.Signature(byte[]);
 * descriptor: ([B)V
 */
extern jmethodID Signature_init;

/* public byte[] getSignatureBytes();
 * descriptor: ()[B
 */
extern jmethodID Signature_getSignatureBytes;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_SIGNATURE_HEADER_GUARD*/
