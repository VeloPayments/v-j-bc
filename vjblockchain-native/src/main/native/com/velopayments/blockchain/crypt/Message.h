/**
 * \file Message.h
 *
 * Class and method exports for Message.  This header includes a static
 * registration mechanism for creating global references to the Message class,
 * so that Message instances can be created from C and methods for these
 * instances can be called from C.
 */

#ifndef  PRIVATE_MESSAGE_HEADER_GUARD
# define PRIVATE_MESSAGE_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct Message_JavaVars
Message_JavaVars;

/**
 * Register the following Message references and make them global.
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
Message_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for Message.
 */
struct Message_JavaVars
{
    /* public class com.velopayments.blockchain.crypt.Message {
     */
    jclass classid;

    /* public com.velopayments.blockchain.crypt.Message(byte[]);
     * descriptor: ([B)V
     */
    jmethodID init;

    /* public byte[] getRawBytes();
     * descriptor: ()[B
     */
    jmethodID getRawBytes;
};

/* helper macro. */
#define MESSAGE_JAVA_VARS() \
    Message_JavaVars Message

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_MESSAGE_HEADER_GUARD*/
