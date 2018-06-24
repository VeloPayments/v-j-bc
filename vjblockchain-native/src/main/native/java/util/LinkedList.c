/**
 * \file LinkedList.c
 *
 * Class and method exports for java.util.LinkedList.  This source file contains
 * a registration method that registers the class and method references just
 * once, making each of these a global reference.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include "LinkedList.h"

jclass LinkedList = NULL;
jmethodID LinkedList_init = NULL;
jmethodID LinkedList_init_Collection = NULL;
jmethodID LinkedList_getFirst = NULL;
jmethodID LinkedList_getLast = NULL;
jmethodID LinkedList_removeFirst = NULL;
jmethodID LinkedList_removeLast = NULL;
jmethodID LinkedList_addFirst = NULL;
jmethodID LinkedList_addLast = NULL;
jmethodID LinkedList_contains = NULL;
jmethodID LinkedList_size = NULL;
jmethodID LinkedList_add = NULL;
jmethodID LinkedList_remove_Object = NULL;
jmethodID LinkedList_addAll_Collection = NULL;
jmethodID LinkedList_addAll_int_Collection = NULL;
jmethodID LinkedList_clear = NULL;
jmethodID LinkedList_get = NULL;
jmethodID LinkedList_set = NULL;
jmethodID LinkedList_add_int_Object = NULL;
jmethodID LinkedList_remove_int = NULL;
jmethodID LinkedList_indexOf = NULL;
jmethodID LinkedList_lastIndexOf = NULL;
jmethodID LinkedList_peek = NULL;
jmethodID LinkedList_element = NULL;
jmethodID LinkedList_poll = NULL;
jmethodID LinkedList_remove = NULL;
jmethodID LinkedList_offer = NULL;
jmethodID LinkedList_offerFirst = NULL;
jmethodID LinkedList_offerLast = NULL;
jmethodID LinkedList_peekFirst = NULL;
jmethodID LinkedList_peekLast = NULL;
jmethodID LinkedList_pollFirst = NULL;
jmethodID LinkedList_pollLast = NULL;
jmethodID LinkedList_push = NULL;
jmethodID LinkedList_pop = NULL;
jmethodID LinkedList_removeFirstOccurrence = NULL;
jmethodID LinkedList_removeLastOccurrence = NULL;
jmethodID LinkedList_listIterator = NULL;
jmethodID LinkedList_descendingIterator = NULL;
jmethodID LinkedList_clone = NULL;
jmethodID LinkedList_toArray = NULL;
jmethodID LinkedList_toArray_Object = NULL;
jmethodID LinkedList_spliterator = NULL;

static volatile bool LinkedList_registered = false;

/**
 * Property: ParserDelegate globals are set.
 */
#define MODEL_PROP_GLOBALS_SET \
    (   NULL != LinkedList \
     && NULL != LinkedList_init \
     && NULL != LinkedList_init_Collection \
     && NULL != LinkedList_getFirst \
     && NULL != LinkedList_getLast \
     && NULL != LinkedList_removeFirst \
     && NULL != LinkedList_removeLast \
     && NULL != LinkedList_addFirst \
     && NULL != LinkedList_addLast \
     && NULL != LinkedList_contains \
     && NULL != LinkedList_size \
     && NULL != LinkedList_add \
     && NULL != LinkedList_remove_Object \
     && NULL != LinkedList_addAll_Collection \
     && NULL != LinkedList_addAll_int_Collection \
     && NULL != LinkedList_clear \
     && NULL != LinkedList_get \
     && NULL != LinkedList_set \
     && NULL != LinkedList_add_int_Object \
     && NULL != LinkedList_remove_int \
     && NULL != LinkedList_indexOf \
     && NULL != LinkedList_lastIndexOf \
     && NULL != LinkedList_peek \
     && NULL != LinkedList_element \
     && NULL != LinkedList_poll \
     && NULL != LinkedList_remove \
     && NULL != LinkedList_offer \
     && NULL != LinkedList_offerFirst \
     && NULL != LinkedList_offerLast \
     && NULL != LinkedList_peekFirst \
     && NULL != LinkedList_peekLast \
     && NULL != LinkedList_pollFirst \
     && NULL != LinkedList_pollLast \
     && NULL != LinkedList_push \
     && NULL != LinkedList_pop \
     && NULL != LinkedList_removeFirstOccurrence \
     && NULL != LinkedList_removeLastOccurrence \
     && NULL != LinkedList_listIterator \
     && NULL != LinkedList_descendingIterator \
     && NULL != LinkedList_clone \
     && NULL != LinkedList_toArray \
     && NULL != LinkedList_toArray_Object \
     && NULL != LinkedList_spliterator)

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
int LinkedList_register(JNIEnv* env)
{
    jclass tempClassID;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));

    /* only register java.util.LinkedList once. */
    if (LinkedList_registered)
    {
        /* enforce globals invariant */
        MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

        return 0;
    }

    /* register LinkedList class */
    tempClassID = (*env)->FindClass(env, "java/util/LinkedList");
    if (NULL == tempClassID)
        return 1;

    /* create a global reference to this class */
    LinkedList = (jclass)(*env)->NewGlobalRef(env, tempClassID);
    if (NULL == LinkedList)
        return 1;

    /* we don't need this local reference anymore */
    (*env)->DeleteLocalRef(env, tempClassID);

    /* register init() method */
    LinkedList_init =
        (*env)->GetMethodID(
            env, LinkedList, "<init>", "()V");
    if (NULL == LinkedList_init)
        return 1;

    /* register init(Collection) method */
    LinkedList_init_Collection =
        (*env)->GetMethodID(
            env, LinkedList, "<init>", "(Ljava/util/Collection;)V");
    if (NULL == LinkedList_init_Collection)
        return 1;

    /* register getFirst method */
    LinkedList_getFirst =
        (*env)->GetMethodID(
            env, LinkedList, "getFirst", "()Ljava/lang/Object;");
    if (NULL == LinkedList_getFirst)
        return 1;

    /* register getLast method */
    LinkedList_getLast =
        (*env)->GetMethodID(
            env, LinkedList, "getLast", "()Ljava/lang/Object;");
    if (NULL == LinkedList_getLast)
        return 1;

    /* register removeFirst method */
    LinkedList_removeFirst =
        (*env)->GetMethodID(
            env, LinkedList, "removeFirst", "()Ljava/lang/Object;");
    if (NULL == LinkedList_removeFirst)
        return 1;

    /* register removeLast method */
    LinkedList_removeLast =
        (*env)->GetMethodID(
            env, LinkedList, "removeLast", "()Ljava/lang/Object;");
    if (NULL == LinkedList_removeLast)
        return 1;

    /* register addFirst method */
    LinkedList_addFirst =
        (*env)->GetMethodID(
            env, LinkedList, "addFirst", "(Ljava/lang/Object;)V");
    if (NULL == LinkedList_addFirst)
        return 1;

    /* register addLast method */
    LinkedList_addLast =
        (*env)->GetMethodID(
            env, LinkedList, "addLast", "(Ljava/lang/Object;)V");
    if (NULL == LinkedList_addLast)
        return 1;

    /* register contains method */
    LinkedList_contains =
        (*env)->GetMethodID(
            env, LinkedList, "contains", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_contains)
        return 1;

    /* register size method */
    LinkedList_size =
        (*env)->GetMethodID(
            env, LinkedList, "size", "()I");
    if (NULL == LinkedList_size)
        return 1;

    /* register add method */
    LinkedList_add =
        (*env)->GetMethodID(
            env, LinkedList, "add", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_add)
        return 1;

    /* register remove(Object) method */
    LinkedList_remove_Object =
        (*env)->GetMethodID(
            env, LinkedList, "remove", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_remove_Object)
        return 1;

    /* register addAll(Collection) method */
    LinkedList_addAll_Collection =
        (*env)->GetMethodID(
            env, LinkedList, "addAll", "(Ljava/util/Collection;)Z");
    if (NULL == LinkedList_addAll_Collection)
        return 1;

    /* register addAll(int, Collection) method */
    LinkedList_addAll_int_Collection =
        (*env)->GetMethodID(
            env, LinkedList, "addAll", "(ILjava/util/Collection;)Z");
    if (NULL == LinkedList_addAll_int_Collection)
        return 1;

    /* register clear method */
    LinkedList_clear =
        (*env)->GetMethodID(
            env, LinkedList, "clear", "()V");
    if (NULL == LinkedList_clear)
        return 1;

    /* register get method */
    LinkedList_get =
        (*env)->GetMethodID(
            env, LinkedList, "get", "(I)Ljava/lang/Object;");
    if (NULL == LinkedList_get)
        return 1;

    /* register set method */
    LinkedList_set =
        (*env)->GetMethodID(
            env, LinkedList, "set", "(ILjava/lang/Object;)Ljava/lang/Object;");
    if (NULL == LinkedList_set)
        return 1;

    /* register add(int, Object) method */
    LinkedList_add_int_Object =
        (*env)->GetMethodID(
            env, LinkedList, "add", "(ILjava/lang/Object;)V");
    if (NULL == LinkedList_add_int_Object)
        return 1;

    /* register remove(int) method */
    LinkedList_remove_int =
        (*env)->GetMethodID(
            env, LinkedList, "remove", "(I)Ljava/lang/Object;");
    if (NULL == LinkedList_remove_int)
        return 1;

    /* register indexOf method */
    LinkedList_indexOf =
        (*env)->GetMethodID(
            env, LinkedList, "indexOf", "(Ljava/lang/Object;)I");
    if (NULL == LinkedList_indexOf)
        return 1;

    /* register lastIndexOf method */
    LinkedList_lastIndexOf =
        (*env)->GetMethodID(
            env, LinkedList, "lastIndexOf", "(Ljava/lang/Object;)I");
    if (NULL == LinkedList_lastIndexOf)
        return 1;

    /* register peek method */
    LinkedList_peek =
        (*env)->GetMethodID(
            env, LinkedList, "peek", "()Ljava/lang/Object;");
    if (NULL == LinkedList_peek)
        return 1;

    /* register element method */
    LinkedList_element =
        (*env)->GetMethodID(
            env, LinkedList, "element", "()Ljava/lang/Object;");
    if (NULL == LinkedList_element)
        return 1;

    /* register poll method */
    LinkedList_poll =
        (*env)->GetMethodID(
            env, LinkedList, "poll", "()Ljava/lang/Object;");
    if (NULL == LinkedList_poll)
        return 1;

    /* register remove method */
    LinkedList_remove =
        (*env)->GetMethodID(
            env, LinkedList, "remove", "()Ljava/lang/Object;");
    if (NULL == LinkedList_remove)
        return 1;

    /* register offer method */
    LinkedList_offer =
        (*env)->GetMethodID(
            env, LinkedList, "offer", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_offer)
        return 1;

    /* register offerFirst method */
    LinkedList_offerFirst =
        (*env)->GetMethodID(
            env, LinkedList, "offerFirst", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_offerFirst)
        return 1;

    /* register offerLast method */
    LinkedList_offerLast =
        (*env)->GetMethodID(
            env, LinkedList, "offerLast", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_offerLast)
        return 1;

    /* register peekFirst method */
    LinkedList_peekFirst =
        (*env)->GetMethodID(
            env, LinkedList, "peekFirst", "()Ljava/lang/Object;");
    if (NULL == LinkedList_peekFirst)
        return 1;

    /* register peekLast method */
    LinkedList_peekLast =
        (*env)->GetMethodID(
            env, LinkedList, "peekLast", "()Ljava/lang/Object;");
    if (NULL == LinkedList_peekLast)
        return 1;

    /* register pollFirst method */
    LinkedList_pollFirst =
        (*env)->GetMethodID(
            env, LinkedList, "pollFirst", "()Ljava/lang/Object;");
    if (NULL == LinkedList_pollFirst)
        return 1;

    /* register pollLast method */
    LinkedList_pollLast =
        (*env)->GetMethodID(
            env, LinkedList, "pollLast", "()Ljava/lang/Object;");
    if (NULL == LinkedList_pollLast)
        return 1;

    /* register push method */
    LinkedList_push =
        (*env)->GetMethodID(
            env, LinkedList, "push", "(Ljava/lang/Object;)V");
    if (NULL == LinkedList_push)
        return 1;

    /* register pop method */
    LinkedList_pop =
        (*env)->GetMethodID(
            env, LinkedList, "pop", "()Ljava/lang/Object;");
    if (NULL == LinkedList_pop)
        return 1;

    /* register removeFirstOccurrence method */
    LinkedList_removeFirstOccurrence =
        (*env)->GetMethodID(
            env, LinkedList, "removeFirstOccurrence", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_removeFirstOccurrence)
        return 1;

    /* register removeLastOccurrence method */
    LinkedList_removeLastOccurrence =
        (*env)->GetMethodID(
            env, LinkedList, "removeLastOccurrence", "(Ljava/lang/Object;)Z");
    if (NULL == LinkedList_removeLastOccurrence)
        return 1;

    /* register listIterator method */
    LinkedList_listIterator =
        (*env)->GetMethodID(
            env, LinkedList, "listIterator", "(I)Ljava/util/ListIterator;");
    if (NULL == LinkedList_listIterator)
        return 1;

    /* register descendingIterator method */
    LinkedList_descendingIterator =
        (*env)->GetMethodID(
            env, LinkedList, "descendingIterator", "()Ljava/util/Iterator;");
    if (NULL == LinkedList_descendingIterator)
        return 1;

    /* register clone method */
    LinkedList_clone =
        (*env)->GetMethodID(
            env, LinkedList, "clone", "()Ljava/lang/Object;");
    if (NULL == LinkedList_clone)
        return 1;

    /* register toArray method */
    LinkedList_toArray =
        (*env)->GetMethodID(
            env, LinkedList, "toArray", "()[Ljava/lang/Object;");
    if (NULL == LinkedList_toArray)
        return 1;

    /* register toArray(Object) method */
    LinkedList_toArray_Object =
        (*env)->GetMethodID(
            env, LinkedList, "toArray",
            "([Ljava/lang/Object;)[Ljava/lang/Object;");
    if (NULL == LinkedList_toArray_Object)
        return 1;

    /* register spliterator method */
    LinkedList_spliterator =
        (*env)->GetMethodID(
            env, LinkedList, "spliterator", "()Ljava/util/Spliterator;");
    if (NULL == LinkedList_spliterator)
        return 1;

    /* globals invariant in place. */
    MODEL_ASSERT(MODEL_PROP_GLOBALS_SET);

    /* success */
    LinkedList_registered = true;
    return 0;
}
