package com.velopayments.blockchain.cert;

import com.velopayments.blockchain.crypt.SigningPrivateKey;
import com.velopayments.blockchain.init.Initializer;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.AbstractMap;
import java.util.Date;
import java.util.LinkedList;
import java.util.UUID;

public class CertificateBuilder {

    /**
     * UTF-8 string transcoder.
     */
    private static Charset stringTranscoder = Charset.forName("UTF-8");

    static {
        Initializer.init();
    }

    /**
     * Field list.
     */
    protected LinkedList<AbstractMap.SimpleEntry<Integer, byte[]>> fields;

    /**
     * Protected constructor.  The create*() static methods should be used.
     */
    protected CertificateBuilder() {
        fields = new LinkedList<>();
    }

    /**
     * Create a CertificateBuilder instance for certificate fragments.
     */
    public static CertificateBuilder createCertificateFragmentBuilder() {
        return new CertificateBuilder();
    }

    /**
     * Create a Builder instance to build complete certificates.
     */
    public static CertificateBuilder
    createCertificateBuilder(UUID certificateType) {
        CertificateBuilder that = new CertificateBuilder();

        //default fields that are part of every certificate
        that.addInt(Field.CERTIFICATE_VERSION, 0x00010000);
        that.addShort(Field.CERTIFICATE_CRYPTO_SUITE, 0x0001);
        that.addUUID(Field.CERTIFICATE_TYPE, certificateType);

        return that;
    }

    /**
     * Add a byte field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The byte value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addByte(int fieldId, byte value) {
        return
            addByteArray(fieldId, serializeByte(value));
    }

    /**
     * Add a short field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The short value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addShort(int fieldId, int value) {
        return
            addByteArray(fieldId, serializeShort(value));
    }

    /**
     * Add an int field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The int value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addInt(int fieldId, int value) {
        return
            addByteArray(fieldId, serializeInt(value));
    }

    /**
     * Add a long field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The long value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addLong(int fieldId, long value) {
        return
            addByteArray(fieldId, serializeLong(value));
    }

    /**
     * Add a UUID field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The UUID value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addUUID(int fieldId, UUID value) {
        return
            addByteArray(fieldId, serializeUUID(value));
    }

    /**
     * Add a String field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The String value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addString(int fieldId, String value) {
        return
            addByteArray(fieldId, serializeString(value));
    }

    /**
     * Add a Date field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The Date value for this field.  It is encoded as
     *                seconds since Epoch.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addDate(int fieldId, Date value) {
        return
            addByteArray(fieldId, serializeDate(value));
    }

    /**
     * Add a byte array field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The byte array value for this field.
     * @return this builder for additional operations.
     */
    public CertificateBuilder addByteArray(int fieldId, byte[] value) {
        fields.add(
            new AbstractMap.SimpleEntry<>(
                (int) (short) fieldId, value
            )
        );

        return this;
    }

    /**
     * Emit an unsigned certificate.
     * <p>
     * This method is used to create untrusted certificates, such as
     * certificates for private keys.
     *
     * @return a raw unsigned certificate.
     */
    public native byte[] emit();

    /**
     * Sign and emit the completed certificate.
     *
     * @param signerId   The UUID of the signer.
     * @param privateKey The private key of the signer.
     * @return the certificate as a byte array.
     */
    public byte[] sign(UUID signerId, SigningPrivateKey privateKey) {
        return signNative(serializeUUID(signerId), privateKey.getRawBytes());
    }

    /**
     * Serialize a byte value.
     *
     * @param value   The byte value to serialize.
     * @return a byte array representation of this value.
     */
    public byte[] serializeByte(byte value) {
        ByteBuffer buf = ByteBuffer.allocate(1);
        buf.put(value);
        return buf.array();
    }

    /**
     * Serialize a short value.
     *
     * @param value   The short value to serialize.
     * @return a byte array representation of this short.
     */
    public byte[] serializeShort(int value) {
        ByteBuffer buf = ByteBuffer.allocate(2);
        buf.putShort((short) value);
        return buf.array();
    }

    /**
     * Serialize an int value.
     *
     * @param value   The int value to serialize.
     * @return a byte array representation of this int value.
     */
    public byte[] serializeInt(int value) {
        ByteBuffer buf = ByteBuffer.allocate(4);
        buf.putInt(value);
        return buf.array();
    }

    /**
     * Serialize a long value.
     *
     * @param value   The long value to serialize.
     * @return the byte representation of this value.
     */
    protected byte[] serializeLong(long value) {
        ByteBuffer buf = ByteBuffer.allocate(8);
        buf.putLong(value);
        return buf.array();
    }

    /**
     * Serialize a String value.
     *
     * @param value   The String value to serialize.
     * @return a byte array representation of this String.
     */
    protected byte[] serializeString(String value) {
        ByteBuffer buf = stringTranscoder.encode(value);
        byte[] stringArr = new byte[buf.remaining()];
        buf.get(stringArr);

        return stringArr;
    }

    /**
     * Serialize a Date value.
     *
     * @param value   The Date value to serialize.
     * @return a byte array representation of this Date value.
     */
    protected byte[] serializeDate(Date value) {
        ByteBuffer buf = ByteBuffer.allocate(8);
        long epochSecond = value.toInstant().getEpochSecond();
        buf.putLong(epochSecond);

        return buf.array();
    }

    /**
     * Transform a UUID into a byte array for certificate serialization.
     *
     * @param uuid The UUID to convert.
     * @return the byte buffer array representation of this uuid.
     */
    protected byte[] serializeUUID(UUID uuid) {
        ByteBuffer buf = ByteBuffer.allocate(16);
        buf.putLong(uuid.getMostSignificantBits());
        buf.putLong(uuid.getLeastSignificantBits());

        return buf.array();
    }

    /**
     * Sign and emit the completed certificate.
     *
     * @param privateKey The private key of the signer.
     * @return the certificate as a byte array.
     */
    private native byte[] signNative(byte[] signerId, byte[] privateKey);
}
