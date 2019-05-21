package com.velopayments.blockchain.util;

import java.nio.ByteBuffer;
import java.util.Optional;
import java.util.UUID;

/**
 * Utility class for operations involving UUIDs.
 */
public class UuidUtil {

    public static final UUID EMPTY_UUID = UUID.fromString("00000000-0000-0000-0000-000000000000");

    /**
     * Convert a UUID to a byte array.  The UUID is converted using Big Endian format.
     *
     *    For example, the UUID
     *
     *    23000000-0000-0000-0000-000000000012
     *
     *    is converted into a 16 byte array with byte '23' in the first element (0)
     *    and byte '12' in the last element (15)
     *
     * @param uuid to be converted
     * @return byte array
     */
    public static byte[] getBytesFromUUID(UUID uuid) {
        ByteBuffer bb = ByteBuffer.wrap(new byte[16]);

        bb.putLong(uuid.getMostSignificantBits());
        bb.putLong(uuid.getLeastSignificantBits());

        return bb.array();
    }

    /**
     * Convert byte array to a UUID.  The byte array is converted using Big Endian format.
     *
     *    For example, a byte array
     *
     *    1 0 0 0 0 0 0 0 0 0 0 0 0 0 0
     *
     *    where byte 15==1, will be converted into a UUID
     *
     *    00000000-0000-0000-0000-000000000001
     *
     * @param bytes byte array to be converted
     * @return uuid
     */
    public static UUID getUUIDFromBytes(byte[] bytes) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(bytes);

        Long high = byteBuffer.getLong();
        Long low = byteBuffer.getLong();

        return new UUID(high, low);
    }

    /**
     * Convert byte array to an Optional<UUID>.  If the byte array contains
     * all zeros, the returned Optional is empty.
     *
     * @param bytes byte array to be converted
     * @return Optional containing converted UUID, or empty if
     *   bytes are all zero.
     */
    public static Optional<UUID> getOptUUIDFromBytes(byte[] bytes) {
        UUID uuid = getUUIDFromBytes(bytes);
        if (EMPTY_UUID.equals(uuid)) {
            return Optional.empty();
        }
        return Optional.of(uuid);
    }

}
