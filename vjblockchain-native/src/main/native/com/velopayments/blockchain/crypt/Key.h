/**
 * \file Key.h
 *
 * Class and method exports for Key.  This header includes a static registration
 * mechanism for creating global references to the Key class, so that Key
 * instances can be created from C and methods for these instances can be called
 * from C.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  PRIVATE_KEY_HEADER_GUARD
# define PRIVATE_KEY_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/**
 * Register the following Key references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Key_register(JNIEnv* env);

/* public class com.velopayments.blockchain.crypt.Key {
 */
extern jclass Key;

/* public com.velopayments.blockchain.crypt.Key(byte[]);
 * descriptor: ([B)V
 */
extern jmethodID Key_init;

/* public byte[] getRawBytes();
 * descriptor: ()[B
 */
extern jmethodID Key_getRawBytes;

/*
 * private byte[] key;
 * descriptor: [B
 */
extern jfieldID Key_key;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //PRIVATE_KEY_HEADER_GUARD
