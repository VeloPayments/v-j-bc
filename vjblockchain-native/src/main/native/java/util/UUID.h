/**
 * \file java_util_uuid.h
 *
 * Class and method exports for UUID.  This header includes a static
 * registration mechanism for creating global references to the UUID class, so
 * that UUID instances can be created from C and methods for these instances can
 * be called from C.
 */

#ifndef  PRIVATE_UUID_HEADER_GUARD
# define PRIVATE_UUID_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct UUID_JavaVars
UUID_JavaVars;

/**
 * Register the following UUID references and make them global.
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
UUID_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for UUID.
 */
struct UUID_JavaVars
{
    /* public final class java.util.UUID
     *      implements java.io.Serializable,
     *                 java.lang.Comparable<java.util.UUID> {
     */
    jclass classid;

    /* public java.util.UUID(long, long);
     * descriptor: (JJ)V
     */
    jmethodID init;

    /* public static java.util.UUID randomUUID();
     * descriptor: ()Ljava/util/UUID;
     */
    jmethodID randomUUID;

    /* public static java.util.UUID nameUUIDFromBytes(byte[]);
     * descriptor: ([B)Ljava/util/UUID;
     */
    jmethodID nameUUIDFromBytes;

    /* public static java.util.UUID fromString(java.lang.String);
     * descriptor: (Ljava/lang/String;)Ljava/util/UUID;
     */
    jmethodID fromString;

    /* public long getLeastSignificantBits();
     * descriptor: ()J
     */
    jmethodID getLeastSignificantBits;

    /* public long getMostSignificantBits();
     * descriptor: ()J
     */
    jmethodID getMostSignificantBits;

    /* public int version();
     * descriptor: ()I
     */
    jmethodID version;

    /* public int variant();
     * descriptor: ()I
     */
    jmethodID variant;

    /* public long timestamp();
     * descriptor: ()J
     */
    jmethodID timestamp;

    /* public int clockSequence();
     * descriptor: ()I
     */
    jmethodID clockSequence;

    /* public long node();
     * descriptor: ()J
     */
    jmethodID node;

    /* public java.lang.String toString();
     * descriptor: ()Ljava/lang/String;
     */
    jmethodID toString;

    /* public int hashCode();
     * descriptor: ()I
     */
    jmethodID hashCode;

    /* public boolean equals(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID equals;

    /* public int compareTo(java.util.UUID);
     * descriptor: (Ljava/util/UUID;)I
     */
    jmethodID compareTo;

    /* public int compareTo(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)I
     */
    jmethodID compareTo_Object;
};

/* helper macro. */
#define UUID_JAVA_VARS() \
    UUID_JavaVars UUID

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_UUID_HEADER_GUARD*/
