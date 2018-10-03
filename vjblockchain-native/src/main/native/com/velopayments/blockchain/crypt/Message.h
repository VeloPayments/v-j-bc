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

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following Message references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Message_register(JNIEnv* env);

/* public class com.velopayments.blockchain.crypt.Message {
 */
extern jclass Message;

/* public com.velopayments.blockchain.crypt.Message(byte[]);
 * descriptor: ([B)V
 */
extern jmethodID Message_init;

/* public byte[] getRawBytes();
 * descriptor: ()[B
 */
extern jmethodID Message_getRawBytes;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_MESSAGE_HEADER_GUARD*/
