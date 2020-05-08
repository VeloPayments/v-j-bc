/**
 * \file SigningPublicKey.h
 *
 * Class and method exports for SigningPublicKey.  This header includes a
 * static registration mechanism for creating global references to the
 * SigningPublicKey class, so that SigningPublicKey instances can be created
 * from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_SIGNING_PUBLIC_KEY_HEADER_GUARD
# define PRIVATE_SIGNING_PUBLIC_KEY_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct SigningPublicKey_JavaVars
SigningPublicKey_JavaVars;

/**
 * Register the following SigningPublicKey references and make them global.
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
SigningPublicKey_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for SigningPublicKey.
 */
struct SigningPublicKey_JavaVars
{
    /* public class com.velopayments.blockchain.crypt.SigningPublicKey {
     */
    jclass classid;

    /* public com.velopayments.blockchain.crypt.SigningPublicKey(byte[]);
     * descriptor: ([B)V
     */
    jmethodID init;

    /* public byte[] getRawBytes();
     * descriptor: ()[B
     */
    jmethodID getRawBytes;
};

/* helper macro. */
#define SIGNING_PUBLIC_KEY_JAVA_VARS() \
    SigningPublicKey_JavaVars SigningPublicKey

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_SIGNING_PUBLIC_KEY_HEADER_GUARD*/
