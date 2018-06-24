/**
 * \file SigningKeyPair.h
 *
 * Class and method exports for SigningKeyPair.  This header includes a static
 * registration mechanism for creating global references to the SigningKeyPair
 * class, so that SigningKeyPair instances can be created from C and methods for
 * these instances can be called from C.
 */

#ifndef  PRIVATE_SIGNING_KEYPAIR_HEADER_GUARD
# define PRIVATE_SIGNING_KEYPAIR_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following SigningKeyPair references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int SigningKeyPair_register(JNIEnv* env);

/* public class com.velopayments.blockchain.crypt.SigningKeyPair {
 */
extern jclass SigningKeyPair;

/* private com.velopayments.blockchain.crypt.SigningKeyPair(
 *      byte[], byte[]);
 * descriptor: ([B[B)V
 */
extern jmethodID SigningKeyPair_init;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_SIGNING_KEYPAIR_HEADER_GUARD*/
