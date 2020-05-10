/**
 * \file uuid_conv.h
 *
 * Convert a byte array to a Java UUID.
 */
#ifndef  PRIVATE_UUID_CONV_HEADER_GUARD
# define PRIVATE_UUID_CONV_HEADER_GUARD

#include <jni.h>

/**
 * Convert a C byte array to a Java UUID.
 *
 * \param env               Java environment.
 * \param nativeInst        Native instance pointer.
 * \param uuid_bytes        The 128-bit UUID in serialized byte form.
 *
 * \returns a Java UUID object, or NULL if the UUID could not be converted.
 * Note that if this method returns NULL, it will throw an
 * IllegalArgumentException.
 */
jobject uuidFromBytes(
    JNIEnv* env, jlong nativeInst, const uint8_t* uuid_bytes);

/**
 * Convert a Java UUID to a C byte array.
 *
 * \param env               Java environment.
 * \param nativeInst        Native instance pointer.
 * \param uuid              The Java UUID object.
 * \param uuid_bytes        The buffer to receive the UUID.
 */
void uuidToBytes(
    JNIEnv* env, jlong nativeInst, jobject uuid, uint8_t* uuid_bytes);

#endif //PRIVATE_UUID_CONV_HEADER_GUARD
