/**
 * \file java/util/Optional.h
 *
 * Class and method exports for Optional.  This header includes a static
 * registration mechanism for creating global references to the Optional class,
 * so that Optional instances can be created from C and methods for these
 * instances can be called from C.
 */

#ifndef  PRIVATE_OPTIONAL_HEADER_GUARD
# define PRIVATE_OPTIONAL_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following Optional references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int Optional_register(JNIEnv* env);

/* public final class java.util.Optional<T> {
 */
extern jclass Optional;

/* public static <T> java.util.Optional<T> empty();
 * descriptor: ()Ljava/util/Optional;
 */
extern jmethodID Optional_empty;

/* public static <T> java.util.Optional<T> of(T);
 * descriptor: (Ljava/lang/Object;)Ljava/util/Optional;
 */
extern jmethodID Optional_of;

/* public static <T> java.util.Optional<T> ofNullable(T);
 * descriptor: (Ljava/lang/Object;)Ljava/util/Optional;
 */
extern jmethodID Optional_ofNullable;

/* public T get();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID Optional_get;

/* public boolean isPresent();
 * descriptor: ()Z
 */
extern jmethodID Optional_isPresent;

/* public void ifPresent(java.util.function.Consumer<? super T>);
 * descriptor: (Ljava/util/function/Consumer;)V
 */
extern jmethodID Optional_ifPresent;

/* public java.util.Optional<T> filter(java.util.function.Predicate<? super T>);
 * descriptor: (Ljava/util/function/Predicate;)Ljava/util/Optional;
 */
extern jmethodID Optional_filter;

/* public <U> java.util.Optional<U>
 * map(java.util.function.Function<? super T, ? extends U>);
 * descriptor: (Ljava/util/function/Function;)Ljava/util/Optional;
 */
extern jmethodID Optional_map;

/* public <U> java.util.Optional<U>
 * flatMap(java.util.function.Function<? super T, java.util.Optional<U>>);
 * descriptor: (Ljava/util/function/Function;)Ljava/util/Optional;
 */
extern jmethodID Optional_flatMap;

/* public T orElse(T);
 * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID Optional_orElse;

/* public T orElseGet(java.util.function.Supplier<? extends T>);
 * descriptor: (Ljava/util/function/Supplier;)Ljava/lang/Object;
 */
extern jmethodID Optional_orElseGet;

/* public <X extends java.lang.Throwable> T
 * orElseThrow(java.util.function.Supplier<? extends X>) throws X;
 * descriptor: (Ljava/util/function/Supplier;)Ljava/lang/Object;
 */
extern jmethodID Optional_orElseThrow;

/* public boolean equals(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID Optional_equals;

/* public int hashCode();
 * descriptor: ()I
 */
extern jmethodID Optional_hashCode;

/* public java.lang.String toString();
 * descriptor: ()Ljava/lang/String;
 */
extern jmethodID Optional_toString;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_OPTIONAL_HEADER_GUARD*/
