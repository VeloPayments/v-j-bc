/**
 * \file SimpleStreamCipher.h
 *
 * Class and method exports for SimpleStreamCipher.  This header includes a
 * static registration mechanism for creating global references to the
 * SimpleStreamCipher class, so that SimpleStreamCipher instances can be created
 * from C and methods for these instances can be called from C.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  PRIVATE_SIMPLESTREAMCIPHER_HEADER_GUARD
# define PRIVATE_SIMPLESTREAMCIPHER_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* forward decls. */
typedef struct SimpleStreamCipher_JavaVars
SimpleStreamCipher_JavaVars;

/**
 * Register the following SimpleStreamCipher references and make them global.
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
SimpleStreamCipher_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for SimpleStreamCipher.
 */
struct SimpleStreamCipher_JavaVars
{
    /* public class com.velopayments.blockchain.crypt.SimpleStreamCipher {
     */
    jclass classid;

    /* public com.velopayments.blockchain.crypt.SimpleStreamCipher(Key);
     * descriptor: (Lcom/velopayments/blockchain/crypt/Key;)V
     */
    jmethodID init;

    /* public com.velopayments.blockchain.crypt.Key getKey();
     * descriptor: ()Lcom/velopayments/blockchain/crypt/Key;
     */
    jmethodID getKey;

    /* public byte[] encrypt(byte[]);
     * descriptor: ([B)[B
     */
    jmethodID encrypt;

    /* public byte[] decrypt(byte[]);
     * descriptor: ([B)[B
     */
    jmethodID decrypt;

    /*
     * private Key key;
     * descriptor: Lcom/velopayments/blockchain/crypt/Key;
     */
    jfieldID field_key;
};

/* helper macro. */
#define SIMPLE_STREAM_CIPHER_JAVA_VARS() \
    SimpleStreamCipher_JavaVars SimpleStreamCipher

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //PRIVATE_SIMPLESTREAMCIPHER_HEADER_GUARD
