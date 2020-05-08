/**
 * \file LinkedList.c
 *
 * Class and method exports for java.util.LinkedList.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>

#include "../../com/velopayments/blockchain/init/init.h"

/**
 * Property: ParserDelegate globals are set.
 */
#define MODEL_PROP_GLOBALS_SET(inst) \
    (   NULL != inst->LinkedList.classid \
     && NULL != inst->LinkedList.init \
     && NULL != inst->LinkedList.init_Collection \
     && NULL != inst->LinkedList.getFirst \
     && NULL != inst->LinkedList.getLast \
     && NULL != inst->LinkedList.removeFirst \
     && NULL != inst->LinkedList.removeLast \
     && NULL != inst->LinkedList.addFirst \
     && NULL != inst->LinkedList.addLast \
     && NULL != inst->LinkedList.contains \
     && NULL != inst->LinkedList.size \
     && NULL != inst->LinkedList.add \
     && NULL != inst->LinkedList.remove_Object \
     && NULL != inst->LinkedList.addAll_Collection \
     && NULL != inst->LinkedList.addAll_int_Collection \
     && NULL != inst->LinkedList.clear \
     && NULL != inst->LinkedList.get \
     && NULL != inst->LinkedList.set \
     && NULL != inst->LinkedList.add_int_Object \
     && NULL != inst->LinkedList.remove_int \
     && NULL != inst->LinkedList.indexOf \
     && NULL != inst->LinkedList.lastIndexOf \
     && NULL != inst->LinkedList.peek \
     && NULL != inst->LinkedList.element \
     && NULL != inst->LinkedList.poll \
     && NULL != inst->LinkedList.remove \
     && NULL != inst->LinkedList.offer \
     && NULL != inst->LinkedList.offerFirst \
     && NULL != inst->LinkedList.offerLast \
     && NULL != inst->LinkedList.peekFirst \
     && NULL != inst->LinkedList.peekLast \
     && NULL != inst->LinkedList.pollFirst \
     && NULL != inst->LinkedList.pollLast \
     && NULL != inst->LinkedList.push \
     && NULL != inst->LinkedList.pop \
     && NULL != inst->LinkedList.removeFirstOccurrence \
     && NULL != inst->LinkedList.removeLastOccurrence \
     && NULL != inst->LinkedList.listIterator \
     && NULL != inst->LinkedList.descendingIterator \
     && NULL != inst->LinkedList.clone \
     && NULL != inst->LinkedList.toArray \
     && NULL != inst->LinkedList.toArray_Object \
     && NULL != inst->LinkedList.spliterator)

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
    vjblockchain_native_instance* inst)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* register LinkedList class */
    tempClassID = (*env)->FindClass(env, "java/util/LinkedList");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    inst->LinkedList.classid = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == inst->LinkedList.classid)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init() method */
    inst->LinkedList.init =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "<init>", "()V");
    if (NULL == inst->LinkedList.init)
        return 1;

    /* register init(Collection) method */
    inst->LinkedList.init_Collection =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "<init>", "(Ljava/util/Collection;)V");
    if (NULL == inst->LinkedList.init_Collection)
        return 1;

    /* register getFirst method */
    inst->LinkedList.getFirst =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "getFirst", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.getFirst)
        return 1;

    /* register getLast method */
    inst->LinkedList.getLast =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "getLast", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.getLast)
        return 1;

    /* register removeFirst method */
    inst->LinkedList.removeFirst =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "removeFirst", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.removeFirst)
        return 1;

    /* register removeLast method */
    inst->LinkedList.removeLast =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "removeLast", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.removeLast)
        return 1;

    /* register addFirst method */
    inst->LinkedList.addFirst =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "addFirst", "(Ljava/lang/Object;)V");
    if (NULL == inst->LinkedList.addFirst)
        return 1;

    /* register addLast method */
    inst->LinkedList.addLast =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "addLast", "(Ljava/lang/Object;)V");
    if (NULL == inst->LinkedList.addLast)
        return 1;

    /* register contains method */
    inst->LinkedList.contains =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "contains", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.contains)
        return 1;

    /* register size method */
    inst->LinkedList.size =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "size", "()I");
    if (NULL == inst->LinkedList.size)
        return 1;

    /* register add method */
    inst->LinkedList.add =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "add", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.add)
        return 1;

    /* register remove(Object) method */
    inst->LinkedList.remove_Object =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "remove", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.remove_Object)
        return 1;

    /* register addAll(Collection) method */
    inst->LinkedList.addAll_Collection =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "addAll", "(Ljava/util/Collection;)Z");
    if (NULL == inst->LinkedList.addAll_Collection)
        return 1;

    /* register addAll(int, Collection) method */
    inst->LinkedList.addAll_int_Collection =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "addAll", "(ILjava/util/Collection;)Z");
    if (NULL == inst->LinkedList.addAll_int_Collection)
        return 1;

    /* register clear method */
    inst->LinkedList.clear =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "clear", "()V");
    if (NULL == inst->LinkedList.clear)
        return 1;

    /* register get method */
    inst->LinkedList.get =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "get", "(I)Ljava/lang/Object;");
    if (NULL == inst->LinkedList.get)
        return 1;

    /* register set method */
    inst->LinkedList.set =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "set", "(ILjava/lang/Object;)Ljava/lang/Object;");
    if (NULL == inst->LinkedList.set)
        return 1;

    /* register add(int, Object) method */
    inst->LinkedList.add_int_Object =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "add", "(ILjava/lang/Object;)V");
    if (NULL == inst->LinkedList.add_int_Object)
        return 1;

    /* register remove(int) method */
    inst->LinkedList.remove_int =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "remove", "(I)Ljava/lang/Object;");
    if (NULL == inst->LinkedList.remove_int)
        return 1;

    /* register indexOf method */
    inst->LinkedList.indexOf =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "indexOf", "(Ljava/lang/Object;)I");
    if (NULL == inst->LinkedList.indexOf)
        return 1;

    /* register lastIndexOf method */
    inst->LinkedList.lastIndexOf =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "lastIndexOf", "(Ljava/lang/Object;)I");
    if (NULL == inst->LinkedList.lastIndexOf)
        return 1;

    /* register peek method */
    inst->LinkedList.peek =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "peek", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.peek)
        return 1;

    /* register element method */
    inst->LinkedList.element =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "element", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.element)
        return 1;

    /* register poll method */
    inst->LinkedList.poll =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "poll", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.poll)
        return 1;

    /* register remove method */
    inst->LinkedList.remove =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "remove", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.remove)
        return 1;

    /* register offer method */
    inst->LinkedList.offer =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "offer", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.offer)
        return 1;

    /* register offerFirst method */
    inst->LinkedList.offerFirst =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "offerFirst", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.offerFirst)
        return 1;

    /* register offerLast method */
    inst->LinkedList.offerLast =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "offerLast", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.offerLast)
        return 1;

    /* register peekFirst method */
    inst->LinkedList.peekFirst =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "peekFirst", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.peekFirst)
        return 1;

    /* register peekLast method */
    inst->LinkedList.peekLast =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "peekLast", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.peekLast)
        return 1;

    /* register pollFirst method */
    inst->LinkedList.pollFirst =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "pollFirst", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.pollFirst)
        return 1;

    /* register pollLast method */
    inst->LinkedList.pollLast =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "pollLast", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.pollLast)
        return 1;

    /* register push method */
    inst->LinkedList.push =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "push", "(Ljava/lang/Object;)V");
    if (NULL == inst->LinkedList.push)
        return 1;

    /* register pop method */
    inst->LinkedList.pop =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "pop", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.pop)
        return 1;

    /* register removeFirstOccurrence method */
    inst->LinkedList.removeFirstOccurrence =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "removeFirstOccurrence", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.removeFirstOccurrence)
        return 1;

    /* register removeLastOccurrence method */
    inst->LinkedList.removeLastOccurrence =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "removeLastOccurrence", "(Ljava/lang/Object;)Z");
    if (NULL == inst->LinkedList.removeLastOccurrence)
        return 1;

    /* register listIterator method */
    inst->LinkedList.listIterator =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "listIterator", "(I)Ljava/util/ListIterator;");
    if (NULL == inst->LinkedList.listIterator)
        return 1;

    /* register descendingIterator method */
    inst->LinkedList.descendingIterator =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "descendingIterator", "()Ljava/util/Iterator;");
    if (NULL == inst->LinkedList.descendingIterator)
        return 1;

    /* register clone method */
    inst->LinkedList.clone =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "clone", "()Ljava/lang/Object;");
    if (NULL == inst->LinkedList.clone)
        return 1;

    /* register toArray method */
    inst->LinkedList.toArray =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "toArray", "()[Ljava/lang/Object;");
    if (NULL == inst->LinkedList.toArray)
        return 1;

    /* register toArray(Object) method */
    inst->LinkedList.toArray_Object =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid, "toArray",
            "([Ljava/lang/Object;)[Ljava/lang/Object;");
    if (NULL == inst->LinkedList.toArray_Object)
        return 1;

    /* register spliterator method */
    inst->LinkedList.spliterator =
        (*env)->GetMethodID(
            env, inst->LinkedList.classid,
            "spliterator", "()Ljava/util/Spliterator;");
    if (NULL == inst->LinkedList.spliterator)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET(inst));

    /* success */
    return 0;
}
