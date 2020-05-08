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

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct EntityReference_JavaVars
EntityReference_JavaVars;

/**
 * Register the following EntityReference references and make them global.
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
EntityReference_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for EntityReference.
 */
struct EntityReference_JavaVars
{
    /* public class com.velopayments.blockchain.cert.EntityReference {
     */
    jclass classid;

    /* public com.velopayments.blockchain.cert.EntityReference(
     *                                              byte[], byte[], UUID);
     * descriptor: ([B[BLjava/util/UUID;)V
     */
    jmethodID init;

    /* public byte[] getPublicEncryptionKey();
     * descriptor: ()[B
     */
    jmethodID getPublicEncryptionKey;

    /* public byte[] getPublicSigningKey();
     * descriptor: ()[B
     */
    jmethodID getPublicSigningKey;
};

/* helper macro. */
#define ENTITY_REFERENCE_JAVA_VARS() \
    EntityReference_JavaVars EntityReference

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ENTITYREFERENCE_HEADER_GUARD*/
