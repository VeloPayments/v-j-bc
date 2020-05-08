/**
 * \file AbstractMap_SimpleEntry.h
 *
 * Class and method exports for java.util.AbstractMap.SimpleEntry.  This header
 * includes a static registration mechanism for creating global references to
 * the java.util.AbstractMap.SimpleEntry class, so that SimpleEntry instances
 * can be created from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_ABSTRACTMAP_SIMPLEENTRY_HEADER_GUARD
# define PRIVATE_ABSTRACTMAP_SIMPLEENTRY_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct SimpleEntry_JavaVars
SimpleEntry_JavaVars;

/**
 * Register the following SimpleEntry references and make them global.
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
SimpleEntry_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for SimpleEntry.
 */
struct SimpleEntry_JavaVars
{
    /* public class java.util.AbstractMap$SimpleEntry<K, V>
     *          implements java.util.Map$Entry<K, V>,
     *                     java.io.Serializable {
     */
    jclass classid;

    /* public java.util.AbstractMap$SimpleEntry(K, V);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)V
     */
    jmethodID init;

    /* public java.util.AbstractMap$SimpleEntry(
     *      java.util.Map$Entry<? extends K, ? extends V>);
     * descriptor: (Ljava/util/Map$Entry;)V
     */
    jmethodID init_Entry;

    /* public K getKey();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID getKey;

    /* public V getValue();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID getValue;

    /* public V setValue(V);
     * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID setValue;

    /* public boolean equals(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID equals;

    /* public int hashCode();
     * descriptor: ()I
     */
    jmethodID hashCode;

    /* public java.lang.String toString();
     * descriptor: ()Ljava/lang/String;
     */
    jmethodID toString;
};

/* helper macro. */
#define SIMPLE_ENTRY_JAVA_VARS() \
    SimpleEntry_JavaVars SimpleEntry

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_ABSTRACTMAP_SIMPLEENTRY_HEADER_GUARD*/
