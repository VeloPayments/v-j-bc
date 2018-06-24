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

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following HashMap references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int HashMap_register(JNIEnv* env);

/* public class java.util.HashMap<K, V>
 *          extends java.util.AbstractMap<K, V>
 *          implements java.util.Map<K, V>,
 *                     java.lang.Cloneable,
 *                     java.io.Serializable {
 */
extern jclass HashMap;

/* public java.util.HashMap(int, float);
 * descriptor: (IF)V
 */
extern jmethodID HashMap_init_int_float;

/* public java.util.HashMap(int);
 * descriptor: (I)V
 */
extern jmethodID HashMap_init_int;

/* public java.util.HashMap();
 * descriptor: ()V
 */
extern jmethodID HashMap_init;

/* public java.util.HashMap(java.util.Map<? extends K, ? extends V>);
 * descriptor: (Ljava/util/Map;)V
 */
extern jmethodID HashMap_init_Map;

/* public int size();
 * descriptor: ()I
 */
extern jmethodID HashMap_size;

/* public boolean isEmpty();
 * descriptor: ()Z
 */
extern jmethodID HashMap_isEmpty;

/* public V get(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID HashMap_get;

/* public boolean containsKey(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID HashMap_containsKey;

/* public V put(K, V);
 * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID HashMap_put;

/* public void putAll(java.util.Map<? extends K, ? extends V>);
 * descriptor: (Ljava/util/Map;)V
 */
extern jmethodID HashMap_putAll;

/* public V remove(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID HashMap_remove;

/* public void clear();
 * descriptor: ()V
 */
extern jmethodID HashMap_clear;

/* public boolean containsValue(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID HashMap_containsValue;

/* public java.util.Set<K> keySet();
 * descriptor: ()Ljava/util/Set;
 */
extern jmethodID HashMap_keySet;

/* public java.util.Collection<V> values();
 * descriptor: ()Ljava/util/Collection;
 */
extern jmethodID HashMap_values;

/* public java.util.Set<java.util.Map$Entry<K, V>> entrySet();
 * descriptor: ()Ljava/util/Set;
 */
extern jmethodID HashMap_entrySet;

/* public V getOrDefault(java.lang.Object, V);
 * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID HashMap_getOrDefault;

/* public V putIfAbsent(K, V);
 * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID HashMap_putIfAbsent;

/* public boolean remove(java.lang.Object, java.lang.Object);
 * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Z
 */
extern jmethodID HashMap_remove_Object_Object;

/* public boolean replace(K, V, V);
 * descriptor: (Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Z
 */
extern jmethodID HashMap_replace_if;

/* public V replace(K, V);
 * descriptor: (Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID HashMap_replace;

/* public V computeIfAbsent(
 *              K, java.util.function.Function<? super K, ? extends V>);
 * descriptor:
 *      (Ljava/lang/Object;Ljava/util/function/Function;)Ljava/lang/Object;
 */
extern jmethodID HashMap_computeIfAbsent;

/* public V computeIfPresent(
 *              K,
 *              java.util.function.BiFunction<
 *                  ? super K, ? super V, ? extends V>);
 * descriptor:
 *      (Ljava/lang/Object;Ljava/util/function/BiFunction;)Ljava/lang/Object;
 */
extern jmethodID HashMap_computeIfPresent;

/* public V compute(
 *              K,
 *              java.util.function.BiFunction<
 *                  ? super K, ? super V, ? extends V>);
 * descriptor:
 *      (Ljava/lang/Object;Ljava/util/function/BiFunction;)Ljava/lang/Object;
 */
extern jmethodID HashMap_compute;

/* public V merge(
 *              K,
 *              V,
 *              java.util.function.BiFunction<
 *                  ? super V, ? super V, ? extends V>);
 * descriptor:
 *      (Ljava/lang/Object;Ljava/lang/Object;Ljava/util/function/BiFunction;)
 *      Ljava/lang/Object;
 */
extern jmethodID HashMap_merge;

/* public void forEach(java.util.function.BiConsumer<? super K, ? super V>);
 * descriptor: (Ljava/util/function/BiConsumer;)V
 */
extern jmethodID HashMap_forEach;

/* public void replaceAll(
 *      java.util.function.BiFunction<? super K, ? super V, ? extends V>);
 * descriptor: (Ljava/util/function/BiFunction;)V
 */
extern jmethodID HashMap_replaceAll;

/* public java.lang.Object clone();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID HashMap_clone;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_HASHMAP_HEADER_GUARD*/
