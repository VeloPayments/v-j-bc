/**
 * \file UnknownArtifactTypeException.h
 *
 * Class and method exports for UnknownArtifactTypeException.  This header
 * includes a * static registration mechanism for creating global references to
 * the UnknownArtifactTypeException class, so that UnknownArtifactTypeException
 * instances can be created from C and methods for these instances can be called
 * from C.
 */

#ifndef  PRIVATE_UNKNOWN_ARTIFACT_TYPE_EXCEPTION_HEADER_GUARD
# define PRIVATE_UNKNOWN_ARTIFACT_TYPE_EXCEPTION_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following UnknownArtifactTypeException references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int UnknownArtifactTypeException_register(JNIEnv* env);

/* public class com.velopayments.blockchain.cert.UnknownArtifactTypeException
 *          extends java.lang.Exception {
 */
extern jclass UnknownArtifactTypeException;

/* public com.velopayments.blockchain.cert.UnknownArtifactTypeException(
 *      java.lang.String);
 * descriptor: (Ljava/lang/String;)V
 */
extern jmethodID UnknownArtifactTypeException_init_String;

/* com.velopayments.blockchain.cert.UnknownArtifactTypeException(
 *      java.util.UUID);
 * descriptor: (Ljava/util/UUID;)V
 */
extern jmethodID UnknownArtifactTypeException_init_UUID;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_UNKNOWN_ARTIFACT_TYPE_EXCEPTION_HEADER_GUARD*/
