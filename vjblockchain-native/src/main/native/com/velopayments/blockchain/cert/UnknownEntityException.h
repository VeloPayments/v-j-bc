/**
 * \file UnknownEntityException.h
 *
 * Class and method exports for UnknownEntityException.  This header includes a
 * static registration mechanism for creating global references to the
 * UnknownEntityException class, so that UnknownEntityException instances can be
 * created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_UNKNOWNENTITYEXCEPTION_HEADER_GUARD
# define PRIVATE_UNKNOWNENTITYEXCEPTION_HEADER_GUARD

#include <jni.h>

#include "../init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* forward decls. */
typedef struct UnknownEntityException_JavaVars
UnknownEntityException_JavaVars;

/**
 * Register the following UnknownEntityException references and make them
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
UnknownEntityException_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for UnknownEntityException.
 */
struct UnknownEntityException_JavaVars
{
    /* public class com.velopayments.blockchain.cert.UnknownEntityException
     *          extends java.lang.RuntimeException {
     */
    jclass classid;

    /* public com.velopayments.blockchain.cert.UnknownEntityException(
     *      java.lang.String);
     * descriptor: (Ljava/lang/String;)V
     */
    jmethodID init_String;

    /* com.velopayments.blockchain.cert.UnknownEntityException(
     *      java.util.UUID);
     * descriptor: (Ljava/util/UUID;)V
     */
    jmethodID init_UUID;
};

/* helper macro. */
#define UNKNOWN_ENTITY_EXCEPTION_JAVA_VARS() \
    UnknownEntityException_JavaVars UnknownEntityException

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //PRIVATE_UNKNOWNENTITYEXCEPTION_HEADER_GUARD
