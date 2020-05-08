/**
 * \file EncryptionPrivateKey.h
 *
 * Class and method exports for EncryptionPrivateKey.  This header includes a
 * static registration mechanism for creating global references to the
 * EncryptionPrivateKey class, so that EncryptionPrivateKey instances can be
 * created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_ENCRYPTIONPRIVATEKEY_HEADER_GUARD
# define PRIVATE_ENCRYPTIONPRIVATEKEY_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct EncryptionPrivateKey_JavaVars
EncryptionPrivateKey_JavaVars;

/**
 * Register the following EncryptionPrivateKey references and make them global.
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
EncryptionPrivateKey_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for EncryptionPrivateKey.
 */
struct EncryptionPrivateKey_JavaVars
{
    /* public class com.velopayments.blockchain.crypt.EncryptionPrivateKey {
     */
    jclass classid;

    /* public com.velopayments.blockchain.crypt.EncryptionPrivateKey(byte[]);
     * descriptor: ([B)V
     */
    jmethodID init;

    /* public byte[] getRawBytes();
     * descriptor: ()[B
     */
    jmethodID getRawBytes;
};

/* helper macro. */
#define ENCRYPTION_PRIVATE_KEY_JAVA_VARS() \
    EncryptionPrivateKey_JavaVars EncryptionPrivateKey

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ENCRYPTIONPRIVATEKEY_HEADER_GUARD*/
