/**
 * \file EncryptionPublicKey.h
 *
 * Class and method exports for EncryptionPublicKey.  This header includes a
 * static registration mechanism for creating global references to the
 * EncryptionPublicKey class, so that EncryptionPublicKey instances can be
 * created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_ENCRYPTIONPUBLICKEY_HEADER_GUARD
# define PRIVATE_ENCRYPTIONPUBLICKEY_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following EncryptionPublicKey references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EncryptionPublicKey_register(JNIEnv* env);

/* public class com.velopayments.blockchain.crypt.EncryptionPublicKey {
 */
extern jclass EncryptionPublicKey;

/* public com.velopayments.blockchain.crypt.EncryptionPublicKey(byte[]);
 * descriptor: ([B)V
 */
extern jmethodID EncryptionPublicKey_init;

/* public byte[] getRawBytes();
 * descriptor: ()[B
 */
extern jmethodID EncryptionPublicKey_getRawBytes;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ENCRYPTIONPUBLICKEY_HEADER_GUARD*/
