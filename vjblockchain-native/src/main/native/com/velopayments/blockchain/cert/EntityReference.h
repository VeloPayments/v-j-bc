/**
 * \file EntityReference.h
 *
 * Class and method exports for EntityReference.  This header includes a static
 * registration mechanism for creating global references to the EntityReference
 * class, so that EntityReference instances can be created from C and methods
 * for these instances can be called from C.
 */

#ifndef  PRIVATE_ENTITYREFERENCE_HEADER_GUARD
# define PRIVATE_ENTITYREFERENCE_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following EntityReference references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int EntityReference_register(JNIEnv* env);

/* public class com.velopayments.blockchain.cert.EntityReference {
 */
extern jclass EntityReference;

/* public com.velopayments.blockchain.cert.EntityReference(
 *                                              byte[], byte[], UUID);
 * descriptor: ([B[BLjava/util/UUID;)V
 */
extern jmethodID EntityReference_init;

/* public byte[] getPublicEncryptionKey();
 * descriptor: ()[B
 */
extern jmethodID EntityReference_getPublicEncryptionKey;

/* public byte[] getPublicSigningKey();
 * descriptor: ()[B
 */
extern jmethodID EntityReference_getPublicSigningKey;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ENTITYREFERENCE_HEADER_GUARD*/
