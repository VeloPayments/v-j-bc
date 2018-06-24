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

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following UUID references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int UUID_register(JNIEnv* env);

/* public final class java.util.UUID
 *      implements java.io.Serializable,
 *                 java.lang.Comparable<java.util.UUID> {
 */
extern jclass UUID;

/* public java.util.UUID(long, long);
 * descriptor: (JJ)V
 */
extern jmethodID UUID_init;

/* public static java.util.UUID randomUUID();
 * descriptor: ()Ljava/util/UUID;
 */
extern jmethodID UUID_randomUUID;

/* public static java.util.UUID nameUUIDFromBytes(byte[]);
 * descriptor: ([B)Ljava/util/UUID;
 */
extern jmethodID UUID_nameUUIDFromBytes;

/* public static java.util.UUID fromString(java.lang.String);
 * descriptor: (Ljava/lang/String;)Ljava/util/UUID;
 */
extern jmethodID UUID_fromString;

/* public long getLeastSignificantBits();
 * descriptor: ()J
 */
extern jmethodID UUID_getLeastSignificantBits;

/* public long getMostSignificantBits();
 * descriptor: ()J
 */
extern jmethodID UUID_getMostSignificantBits;

/* public int version();
 * descriptor: ()I
 */
extern jmethodID UUID_version;

/* public int variant();
 * descriptor: ()I
 */
extern jmethodID UUID_variant;

/* public long timestamp();
 * descriptor: ()J
 */
extern jmethodID UUID_timestamp;

/* public int clockSequence();
 * descriptor: ()I
 */
extern jmethodID UUID_clockSequence;

/* public long node();
 * descriptor: ()J
 */
extern jmethodID UUID_node;

/* public java.lang.String toString();
 * descriptor: ()Ljava/lang/String;
 */
extern jmethodID UUID_toString;

/* public int hashCode();
 * descriptor: ()I
 */
extern jmethodID UUID_hashCode;

/* public boolean equals(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID UUID_equals;

/* public int compareTo(java.util.UUID);
 * descriptor: (Ljava/util/UUID;)I
 */
extern jmethodID UUID_compareTo;

/* public int compareTo(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)I
 */
extern jmethodID UUID_compareTo_Object;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_UUID_HEADER_GUARD*/
