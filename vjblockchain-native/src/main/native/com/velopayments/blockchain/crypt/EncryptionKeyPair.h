/**
 * \file EncryptionKeyPair.h
 *
 * Class and method exports for EncryptionKeyPair.  This header includes a
 * static registration mechanism for creating global references to the
 * EncryptionKeyPair class, so that EncryptionKeyPair instances can be created
 * from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_ENCRYPTIONKEYPAIR_HEADER_GUARD
# define PRIVATE_ENCRYPTIONKEYPAIR_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following EncryptionKeyPair references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EncryptionKeyPair_register(JNIEnv* env);

/* public class com.velopayments.blockchain.crypt.EncryptionKeyPair {
 */
extern jclass EncryptionKeyPair;

/* public com.velopayments.blockchain.crypt.EncryptionKeyPair(
 *      com.velopayments.blockchain.crypt.EncryptionPublicKey,
 *      com.velopayments.blockchain.crypt.EncryptionPrivateKey);
 * descriptor: (Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;
 *              Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;)V
 */
extern jmethodID EncryptionKeyPair_init;

/* public com.velopayments.blockchain.crypt.EncryptionPublicKey
 * getPublicKey();
 * descriptor: ()Lcom/velopayments/blockchain/crypt/EncryptionPublicKey;
 */
extern jmethodID EncryptionKeyPair_getPublicKey;

/* public com.velopayments.blockchain.crypt.EncryptionPrivateKey
 * getPrivateKey();
 * descriptor: ()Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;
 */
extern jmethodID EncryptionKeyPair_getPrivateKey;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ENCRYPTIONKEYPAIR_HEADER_GUARD*/
