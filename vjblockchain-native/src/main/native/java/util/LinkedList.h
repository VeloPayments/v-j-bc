/**
 * \file LinkedList.h
 *
 * Class and method exports for java.util.LinkedList.  This header includes a
 * static registration mechanism for creating global references to the
 * java.util.LinkedList class, so that LinkedList instances can be created from
 * C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_LINKEDLIST_HEADER_GUARD
# define PRIVATE_LINKEDLIST_HEADER_GUARD

#include <jni.h>

#include "../../com/velopayments/blockchain/init/init_fwd.h"

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decls. */
typedef struct LinkedList_JavaVars
LinkedList_JavaVars;

/**
 * Register the following LinkedList references and make them global.
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
LinkedList_register(
    JNIEnv* env,
    vjblockchain_native_instance* inst);

/**
 * \brief Java variables for LinkedList.
 */
struct LinkedList_JavaVars
{
    /* public class java.util.LinkedList<E>
     *          extends java.util.AbstractSequentialList<E>
     *          implements java.util.List<E>,
     *                     java.util.Deque<E>,
     *                     java.lang.Cloneable,
     *                     java.io.Serializable {
     */
    jclass classid;

    /* public java.util.LinkedList();
     * descriptor: ()V
     */
    jmethodID init;

    /* public java.util.LinkedList(java.util.Collection<? extends E>);
     * descriptor: (Ljava/util/Collection;)V
     */
    jmethodID init_Collection;

    /* public E getFirst();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID getFirst;

    /* public E getLast();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID getLast;

    /* public E removeFirst();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID removeFirst;

    /* public E removeLast();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID removeLast;

    /* public void addFirst(E);
     * descriptor: (Ljava/lang/Object;)V
     */
    jmethodID addFirst;

    /* public void addLast(E);
     * descriptor: (Ljava/lang/Object;)V
     */
    jmethodID addLast;

    /* public boolean contains(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID contains;

    /* public int size();
     * descriptor: ()I
     */
    jmethodID size;

    /* public boolean add(E);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID add;

    /* public boolean remove(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID remove_Object;

    /* public boolean addAll(java.util.Collection<? extends E>);
     * descriptor: (Ljava/util/Collection;)Z
     */
    jmethodID addAll_Collection;

    /* public boolean addAll(int, java.util.Collection<? extends E>);
     * descriptor: (ILjava/util/Collection;)Z
     */
    jmethodID addAll_int_Collection;

    /* public void clear();
     * descriptor: ()V
     */
    jmethodID clear;

    /* public E get(int);
     * descriptor: (I)Ljava/lang/Object;
     */
    jmethodID get;

    /* public E set(int, E);
     * descriptor: (ILjava/lang/Object;)Ljava/lang/Object;
     */
    jmethodID set;

    /* public void add(int, E);
     * descriptor: (ILjava/lang/Object;)V
     */
    jmethodID add_int_Object;

    /* public E remove(int);
     * descriptor: (I)Ljava/lang/Object;
     */
    jmethodID remove_int;

    /* public int indexOf(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)I
     */
    jmethodID indexOf;

    /* public int lastIndexOf(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)I
     */
    jmethodID lastIndexOf;

    /* public E peek();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID peek;

    /* public E element();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID element;

    /* public E poll();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID poll;

    /* public E remove();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID remove;

    /* public boolean offer(E);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID offer;

    /* public boolean offerFirst(E);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID offerFirst;

    /* public boolean offerLast(E);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID offerLast;

    /* public E peekFirst();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID peekFirst;

    /* public E peekLast();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID peekLast;

    /* public E pollFirst();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID pollFirst;

    /* public E pollLast();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID pollLast;

    /* public void push(E);
     * descriptor: (Ljava/lang/Object;)V
     */
    jmethodID push;

    /* public E pop();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID pop;

    /* public boolean removeFirstOccurrence(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID removeFirstOccurrence;

    /* public boolean removeLastOccurrence(java.lang.Object);
     * descriptor: (Ljava/lang/Object;)Z
     */
    jmethodID removeLastOccurrence;

    /* public java.util.ListIterator<E> listIterator(int);
     * descriptor: (I)Ljava/util/ListIterator;
     */
    jmethodID listIterator;

    /* public java.util.Iterator<E> descendingIterator();
     * descriptor: ()Ljava/util/Iterator;
     */
    jmethodID descendingIterator;

    /* public java.lang.Object clone();
     * descriptor: ()Ljava/lang/Object;
     */
    jmethodID clone;

    /* public java.lang.Object[] toArray();
     * descriptor: ()[Ljava/lang/Object;
     */
    jmethodID toArray;

    /* public <T> T[] toArray(T[]);
     * descriptor: ([Ljava/lang/Object;)[Ljava/lang/Object;
     */
    jmethodID toArray_Object;

    /* public java.util.Spliterator<E> spliterator();
     * descriptor: ()Ljava/util/Spliterator;
     */
    jmethodID spliterator;
};

/* helper macro. */
#define LINKED_LIST_JAVA_VARS() \
    LinkedList_JavaVars LinkedList

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_LINKEDLIST_HEADER_GUARD*/
