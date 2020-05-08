/**
 * \file HashMap.h
 *
 * Class and method exports for java.util.HashMap.  This header includes a
 * static registration mechanism for creating global references to the
 * java.util.HashMap class, so that HashMap instances can be created from C and
 * methods for these instances can be called from C.
 */

#ifndef  PRIVATE_HASHMAP_HEADER_GUARD
# define PRIVATE_HASHMAP_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct HashMap_JavaVars
HashMap_JavaVars;

/**
 * Register the following HashMap references and make them global.
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
HashMap_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for HashMap.
 */
struct HashMap_JavaVars
{
    /* public class java.util.HashMap<K, V>
     *          extends java.util.AbstractMap<K, V>
     *          implements java.util.Map<K, V>,
     *                     java.lang.Cloneable,
     *                     java.io.Serializable {
     */
    jclass classid;

    /* public java.util.HashMap(int, float);
     * descriptor: (IF)V
     */
    jmethodID init_int_float;

    /* public java.util.HashMap(int);
     * descriptor: (I)V
     */
    jmethodID init_int;

    /* public java.util.HashMap();
     * descriptor: ()V
     */
    jmethodID init;

    /* public java.util.HashMap(java.util.Map<? extends K, ? extends V>);
     * descriptor: (Ljava/util/Map;)V
     */
    jmethodID init_Map;

    /* public int size();
     * descriptor: ()I
     */
    jmethodID size;

    /* public boolean isEmpty();
     * descriptor: ()Z
     */
    jmethodID isEmpty;

    /* public V get(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID get;

    /* public boolean containsKey(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID containsKey;

    /* public V put(K, V);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID put;

    /* public void putAll(java.util.Map<? extends K, ? extends V>);
     * descriptor: (Ljava/util/Map;)V
     */
    jmethodID putAll;

    /* public V remove(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID remove;

    /* public void clear();
     * descriptor: ()V
     */
    jmethodID clear;

    /* public boolean containsValue(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID containsValue;

    /* public java.util.Set<K> keySet();
     * descriptor: ()Ljava/util/Set;
     */
    jmethodID keySet;

    /* public java.util.Collection<V> values();
     * descriptor: ()Ljava/util/Collection;
     */
    jmethodID values;

    /* public java.util.Set<java.util.Map$Entry<K, V>> entrySet();
     * descriptor: ()Ljava/util/Set;
     */
    jmethodID entrySet;

    /* public V getOrDefault(java.lang.Object, V);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID getOrDefault;

    /* public V putIfAbsent(K, V);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID putIfAbsent;

    /* public boolean remove(java.lang.Object, java.lang.Object);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Z
     */
    jmethodID remove_Object_Object;

    /* public boolean replace(K, V, V);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Z
     */
    jmethodID replace_if;

    /* public V replace(K, V);
     * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID replace;

    /* public V computeIfAbsent(
     *              K, java.util.function.Function<? super K, ? extends V>);
     * descriptor:
     *      (Ljava/lang/Object;Ljava/util/function/Function;)Ljava/lang/Object;
     */
    jmethodID computeIfAbsent;

    /* public V computeIfPresent(
     *              K,
     *              java.util.function.BiFunction<
     *                  ? super K, ? super V, ? extends V>);
     * descriptor:
     *    (Ljava/lang/Object;Ljava/util/function/BiFunction;)Ljava/lang/Object;
     */
    jmethodID computeIfPresent;

    /* public V compute(
     *              K,
     *              java.util.function.BiFunction<
     *                  ? super K, ? super V, ? extends V>);
     * descriptor:
     *    (Ljava/lang/Object;Ljava/util/function/BiFunction;)Ljava/lang/Object;
     */
    jmethodID compute;

    /* public V merge(
     *              K,
     *              V,
     *              java.util.function.BiFunction<
     *                  ? super V, ? super V, ? extends V>);
     * descriptor:
     *    (Ljava/lang/Object;Ljava/lang/Object;Ljava/util/function/BiFunction;)
     *    Ljava/lang/Object;
     */
    jmethodID merge;

    /* public void forEach(java.util.function.BiConsumer<? super K, ? super V>);
     * descriptor: (Ljava/util/function/BiConsumer;)V
     */
    jmethodID forEach;

    /* public void replaceAll(
     *      java.util.function.BiFunction<? super K, ? super V, ? extends V>);
     * descriptor: (Ljava/util/function/BiFunction;)V
     */
    jmethodID replaceAll;

    /* public java.lang.Object clone();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID clone;
};

/* helper macro. */
#define HASH_MAP_JAVA_VARS() \
    HashMap_JavaVars HashMap

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_HASHMAP_HEADER_GUARD*/
