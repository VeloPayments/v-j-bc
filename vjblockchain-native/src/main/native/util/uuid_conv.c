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

/**
 * Convert a Java UUID to a C byte array.
 *
 * \param env               Java environment.
 * \param uuid              The Java UUID object.
 * \param uuid_bytes        The buffer to receive the UUID.
 */
void uuidToBytes(JNIEnv* env, jobject uuid, uint8_t* uuid_bytes)
{
    uint64_t msb, lsb;

    msb = (*env)->CallLongMethod(env, uuid, UUID_getMostSignificantBits);
    lsb = (*env)->CallLongMethod(env, uuid, UUID_getLeastSignificantBits);

    uuid_bytes[ 0] = (msb & 0xFF00000000000000UL) >> 56;
    uuid_bytes[ 1] = (msb & 0x00FF000000000000UL) >> 48;
    uuid_bytes[ 2] = (msb & 0x0000FF0000000000UL) >> 40;
    uuid_bytes[ 3] = (msb & 0x000000FF00000000UL) >> 32;
    uuid_bytes[ 4] = (msb & 0x00000000FF000000UL) >> 24;
    uuid_bytes[ 5] = (msb & 0x0000000000FF0000UL) >> 16;
    uuid_bytes[ 6] = (msb & 0x000000000000FF00UL) >>  8;
    uuid_bytes[ 7] = (msb & 0x00000000000000FFUL);
    uuid_bytes[ 8] = (lsb & 0xFF00000000000000UL) >> 56;
    uuid_bytes[ 9] = (lsb & 0x00FF000000000000UL) >> 48;
    uuid_bytes[10] = (lsb & 0x0000FF0000000000UL) >> 40;
    uuid_bytes[11] = (lsb & 0x000000FF00000000UL) >> 32;
    uuid_bytes[12] = (lsb & 0x00000000FF000000UL) >> 24;
    uuid_bytes[13] = (lsb & 0x0000000000FF0000UL) >> 16;
    uuid_bytes[14] = (lsb & 0x000000000000FF00UL) >>  8;
    uuid_bytes[15] = (lsb & 0x00000000000000FFUL);
}
