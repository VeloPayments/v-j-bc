package com.velopayments.blockchain.cert;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.time.Instant;
import java.time.ZonedDateTime;
import java.time.ZoneOffset;
import java.util.Date;
import java.util.UUID;

public class CertificateFieldReader {

    private static Charset charSet = Charset.forName("UTF-8");
    private ByteBuffer buf;

    /**
     * Create a CertificateFieldReader from a byte array.
     */
    public CertificateFieldReader(byte[] fieldValue) {
        this.buf = ByteBuffer.wrap(fieldValue);
    }

    /**
     * Attempt to convert a field to a byte value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  byte value (e.g. it is larger than a byte).
     */
    public byte asByte() throws FieldConversionException {
        if (buf.remaining() < 1 || buf.remaining() > 1) {
            throw new FieldConversionException(
                "Field cannot be converted to a byte.");
        }

        return buf.get();
    }

    /**
     * Attempt to convert a field to a short value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  short value (e.g. it is larger than a short).
     */
    public short asShort() throws FieldConversionException {
        if (buf.remaining() < 2 || buf.remaining() > 2) {
            throw new FieldConversionException(
                "Field cannot be converted to a short.");
        }

        return buf.getShort();
    }

    /**
     * Attempt to convert a field to an int value.
     *
     * @throws FieldConversionException if this field cannot be converted to an
     *                                  int value (e.g. it is larger than an int).
     */
    public int asInt() throws FieldConversionException {
        if (buf.remaining() < 4 || buf.remaining() > 4) {
            throw new FieldConversionException(
                "Field cannot be converted to an int.");
        }

        return buf.getInt();
    }

    /**
     * Attempt to convert a field to a long value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  long value (e.g. it is larger than a long).
     */
    public long asLong() throws FieldConversionException {
        if (buf.remaining() < 8 || buf.remaining() > 8) {
            throw new FieldConversionException(
                "Field cannot be converted to a long.");
        }

        return buf.getLong();
    }

    /**
     * Attempt to convert a field to a UUID value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  UUID value (e.g. it is larger than a UUID).
     */
    public UUID asUUID() throws FieldConversionException {
        if (buf.remaining() < 16 || buf.remaining() > 16) {
            throw new FieldConversionException(
                "Field cannot be converted to a UUID.");
        }

        long msb = buf.getLong();
        long lsb = buf.getLong();

        return new UUID(msb, lsb);
    }

    /**
     * Attempt to convert a field to a String value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  String value (e.g. it contains unmappable characters)
     */
    public String asString() throws FieldConversionException {
        if (buf.remaining() < 1) {
            return "";
        }

        try {
            return charSet.newDecoder().decode(buf).toString();
        } catch (Throwable err) {
            //this will be some charset conversion error.  In all cases, turn it
            //into a field conversion exception with a nested exception
            throw new FieldConversionException(
                "Encountered exception while converting string.", err);
        }
    }

    /**
     * Attempt to convert a field to a Date value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  Date value (e.g. it is too small for a date value)
     */
    public Date asDate() throws FieldConversionException {
        if (buf.remaining() < 8 || buf.remaining() > 8) {
            throw new FieldConversionException(
                "Field cannot be converted to a Date.");
        }

        return new Date(buf.getLong() * 1000);
    }

    /**
     * Attempt to convert a field to a ZonedDateTime value.
     *
     * @throws FieldConversionException if this field cannot be converted to a
     *                                  ZonedDateTime value (e.g. it is too
     *                                  small for a date value)
     */
    public ZonedDateTime asZonedDateTime() throws FieldConversionException {
        if (buf.remaining() < 12 || buf.remaining() > 12) {
            throw new FieldConversionException(
                "Field cannot be converted to a ZonedDateTime.");
        }

        long timeEpochSeconds = buf.getLong(0);
        int timeOffset = buf.getInt(8);

        return
            ZonedDateTime.ofInstant(
                Instant.ofEpochSecond(timeEpochSeconds),
                ZoneOffset.ofTotalSeconds(timeOffset));
    }

    /**
     * Convert the field to a byte array.
     */
    public byte[] asByteArray() {
        return buf.array();
    }
}
