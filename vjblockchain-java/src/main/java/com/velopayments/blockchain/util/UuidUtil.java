package com.velopayments.blockchain.util;

import java.nio.ByteBuffer;
import java.util.Optional;
import java.util.UUID;

public class UuidUtil {

    public static final UUID EMPTY_UUID = UUID.fromString("00000000-0000-0000-0000-000000000000");

    public static byte[] getBytesFromUUID(UUID uuid) {
        ByteBuffer bb = ByteBuffer.wrap(new byte[16]);

        bb.putLong(uuid.getMostSignificantBits());
        bb.putLong(uuid.getLeastSignificantBits());

        return bb.array();
    }

    public static UUID getUUIDFromBytes(byte[] bytes) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(bytes);

        Long high = byteBuffer.getLong();
        Long low = byteBuffer.getLong();

        return new UUID(high, low);
    }

    public static Optional<UUID> getOptUUIDFromBytes(byte[] bytes) {
        UUID uuid = getUUIDFromBytes(bytes);
        if (EMPTY_UUID.equals(uuid)) {
            return Optional.empty();
        }
        return Optional.of(uuid);
    }

}
