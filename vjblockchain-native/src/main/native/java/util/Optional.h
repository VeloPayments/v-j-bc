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

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct Optional_JavaVars
Optional_JavaVars;

/**
 * Register the following Optional references and make them global.
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
Optional_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for Optional.
 */
struct Optional_JavaVars
{
    /* public final class java.util.Optional<T> {
     */
    jclass classid;

    /* public static <T> java.util.Optional<T> empty();
     * descriptor: ()Ljava/util/Optional;
     */
    jmethodID empty;

    /* public static <T> java.util.Optional<T> of(T);
     * descriptor: (Ljava/lang/Object;)Ljava/util/Optional;
     */
    jmethodID of;

    /* public static <T> java.util.Optional<T> ofNullable(T);
     * descriptor: (Ljava/lang/Object;)Ljava/util/Optional;
     */
    jmethodID ofNullable;

    /* public T get();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID get;

    /* public boolean isPresent();
     * descriptor: ()Z
     */
    jmethodID isPresent;

    /* public void ifPresent(java.util.function.Consumer<? super T>);
     * descriptor: (Ljava/util/function/Consumer;)V
     */
    jmethodID ifPresent;

    /* public java.util.Optional<T>
     * filter(java.util.function.Predicate<? super T>);
     * descriptor: (Ljava/util/function/Predicate;)Ljava/util/Optional;
     */
    jmethodID filter;

    /* public <U> java.util.Optional<U>
     * map(java.util.function.Function<? super T, ? extends U>);
     * descriptor: (Ljava/util/function/Function;)Ljava/util/Optional;
     */
    jmethodID map;

    /* public <U> java.util.Optional<U>
     * flatMap(java.util.function.Function<? super T, java.util.Optional<U>>);
     * descriptor: (Ljava/util/function/Function;)Ljava/util/Optional;
     */
    jmethodID flatMap;

    /* public T orElse(T);
     * descriptor: (Ljava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID orElse;

    /* public T orElseGet(java.util.function.Supplier<? extends T>);
     * descriptor: (Ljava/util/function/Supplier;)Ljava/lang/Object;
     */
    jmethodID orElseGet;

    /* public <X extends java.lang.Throwable> T
     * orElseThrow(java.util.function.Supplier<? extends X>) throws X;
     * descriptor: (Ljava/util/function/Supplier;)Ljava/lang/Object;
     */
    jmethodID orElseThrow;

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
#define OPTIONAL_JAVA_VARS() \
    Optional_JavaVars Optional

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_OPTIONAL_HEADER_GUARD*/
