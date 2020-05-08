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

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct UnknownArtifactTypeException_JavaVars
UnknownArtifactTypeException_JavaVars;

/**
 * Register the following UnknownArtifactTypeException references and make them
 * global.
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
UnknownArtifactTypeException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for UnknownArtifactTypeException.
 */
struct UnknownArtifactTypeException_JavaVars
{
    /* public class
     * com.velopayments.blockchain.cert.UnknownArtifactTypeException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public com.velopayments.blockchain.cert.UnknownArtifactTypeException(
     *      java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* com.velopayments.blockchain.cert.UnknownArtifactTypeException(
     *      java.util.UUID);
     * descriptor: (Ljava/util/UUID;)V
     */
    jmethodID init_UUID;
};

/* helper macro. */
#define UNKNOWN_ARTIFACT_TYPE_EXCEPTION_JAVA_VARS() \
    UnknownArtifactTypeException_JavaVars UnknownArtifactTypeException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_UNKNOWN_ARTIFACT_TYPE_EXCEPTION_HEADER_GUARD*/
