/**
 * \file uuid_conv.c
 *
 * Convert a byte array to a Java UUID.
 */

#include <cbmc/model_assert.h>
#include <stdint.h>

#include "../java/util/UUID.h"
#include "uuid_conv.h"

/**
 * Convert a C byte array to a Java UUID.
 *
 * \param env               Java environment.
 * \param uuid_bytes        The 128-bit UUID in serialized byte form.
 *
 * \returns a Java UUID object, or NULL if the UUID could not be converted.
 * Note that if this method returns NULL, it will throw an
 * IllegalArgumentException.
 */
jobject uuidFromBytes(JNIEnv* env, const uint8_t* uuid_bytes)
{
    uint64_t msb, lsb;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != uuid_bytes);

    msb = (((uint64_t)uuid_bytes[ 0]) << 56)
        | (((uint64_t)uuid_bytes[ 1]) << 48)
        | (((uint64_t)uuid_bytes[ 2]) << 40)
        | (((uint64_t)uuid_bytes[ 3]) << 32)
        | (((uint64_t)uuid_bytes[ 4]) << 24)
        | (((uint64_t)uuid_bytes[ 5]) << 16)
        | (((uint64_t)uuid_bytes[ 6]) <<  8)
        | (((uint64_t)uuid_bytes[ 7])      );

    lsb = (((uint64_t)uuid_bytes[ 8]) << 56)
        | (((uint64_t)uuid_bytes[ 9]) << 48)
        | (((uint64_t)uuid_bytes[10]) << 40)
        | (((uint64_t)uuid_bytes[11]) << 32)
        | (((uint64_t)uuid_bytes[12]) << 24)
        | (((uint64_t)uuid_bytes[13]) << 16)
        | (((uint64_t)uuid_bytes[14]) <<  8)
        | (((uint64_t)uuid_bytes[15])      );

    return
        (*env)->NewObject(env, UUID, UUID_init, (jlong)msb, (jlong)lsb);
}
