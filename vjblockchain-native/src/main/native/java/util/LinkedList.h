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

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following LinkedList references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int LinkedList_register(JNIEnv* env);

/* public class java.util.LinkedList<E>
 *          extends java.util.AbstractSequentialList<E>
 *          implements java.util.List<E>,
 *                     java.util.Deque<E>,
 *                     java.lang.Cloneable,
 *                     java.io.Serializable {
 */
extern jclass LinkedList;

/* public java.util.LinkedList();
 * descriptor: ()V
 */
extern jmethodID LinkedList_init;

/* public java.util.LinkedList(java.util.Collection<? extends E>);
 * descriptor: (Ljava/util/Collection;)V
 */
extern jmethodID LinkedList_init_Collection;

/* public E getFirst();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_getFirst;

/* public E getLast();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_getLast;

/* public E removeFirst();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_removeFirst;

/* public E removeLast();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_removeLast;

/* public void addFirst(E);
 * descriptor: (Ljava/lang/Object;)V
 */
extern jmethodID LinkedList_addFirst;

/* public void addLast(E);
 * descriptor: (Ljava/lang/Object;)V
 */
extern jmethodID LinkedList_addLast;

/* public boolean contains(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_contains;

/* public int size();
 * descriptor: ()I
 */
extern jmethodID LinkedList_size;

/* public boolean add(E);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_add;

/* public boolean remove(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_remove_Object;

/* public boolean addAll(java.util.Collection<? extends E>);
 * descriptor: (Ljava/util/Collection;)Z
 */
extern jmethodID LinkedList_addAll_Collection;

/* public boolean addAll(int, java.util.Collection<? extends E>);
 * descriptor: (ILjava/util/Collection;)Z
 */
extern jmethodID LinkedList_addAll_int_Collection;

/* public void clear();
 * descriptor: ()V
 */
extern jmethodID LinkedList_clear;

/* public E get(int);
 * descriptor: (I)Ljava/lang/Object;
 */
extern jmethodID LinkedList_get;

/* public E set(int, E);
 * descriptor: (ILjava/lang/Object;)Ljava/lang/Object;
 */
extern jmethodID LinkedList_set;

/* public void add(int, E);
 * descriptor: (ILjava/lang/Object;)V
 */
extern jmethodID LinkedList_add_int_Object;

/* public E remove(int);
 * descriptor: (I)Ljava/lang/Object;
 */
extern jmethodID LinkedList_remove_int;

/* public int indexOf(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)I
 */
extern jmethodID LinkedList_indexOf;

/* public int lastIndexOf(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)I
 */
extern jmethodID LinkedList_lastIndexOf;

/* public E peek();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_peek;

/* public E element();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_element;

/* public E poll();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_poll;

/* public E remove();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_remove;

/* public boolean offer(E);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_offer;

/* public boolean offerFirst(E);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_offerFirst;

/* public boolean offerLast(E);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_offerLast;

/* public E peekFirst();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_peekFirst;

/* public E peekLast();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_peekLast;

/* public E pollFirst();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_pollFirst;

/* public E pollLast();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_pollLast;

/* public void push(E);
 * descriptor: (Ljava/lang/Object;)V
 */
extern jmethodID LinkedList_push;

/* public E pop();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_pop;

/* public boolean removeFirstOccurrence(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_removeFirstOccurrence;

/* public boolean removeLastOccurrence(java.lang.Object);
 * descriptor: (Ljava/lang/Object;)Z
 */
extern jmethodID LinkedList_removeLastOccurrence;

/* public java.util.ListIterator<E> listIterator(int);
 * descriptor: (I)Ljava/util/ListIterator;
 */
extern jmethodID LinkedList_listIterator;

/* public java.util.Iterator<E> descendingIterator();
 * descriptor: ()Ljava/util/Iterator;
 */
extern jmethodID LinkedList_descendingIterator;

/* public java.lang.Object clone();
 * descriptor: ()Ljava/lang/Object;
 */
extern jmethodID LinkedList_clone;

/* public java.lang.Object[] toArray();
 * descriptor: ()[Ljava/lang/Object;
 */
extern jmethodID LinkedList_toArray;

/* public <T> T[] toArray(T[]);
 * descriptor: ([Ljava/lang/Object;)[Ljava/lang/Object;
 */
extern jmethodID LinkedList_toArray_Object;

/* public java.util.Spliterator<E> spliterator();
 * descriptor: ()Ljava/util/Spliterator;
 */
extern jmethodID LinkedList_spliterator;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_LINKEDLIST_HEADER_GUARD*/
