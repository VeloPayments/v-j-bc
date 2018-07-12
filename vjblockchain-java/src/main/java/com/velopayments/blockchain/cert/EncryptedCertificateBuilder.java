package com.velopayments.blockchain.cert;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import java.util.UUID;
import java.util.Date;

public class EncryptedCertificateBuilder extends CertificateBuilder {

    /**
     * Protected constructor.  The create*() static methods should be used.
     */
    protected EncryptedCertificateBuilder(EncryptionKeyPair keyPair) {
        super();

        this.keyPair = keyPair;
        this.secretKey = generateEncryptionKey();
        this.iv = 0;
    }

    /**
     * Create an EncryptedCertificateBuilder instance from an EncryptionKeyPair
     * and a certificate type.
     */
    public static EncryptedCertificateBuilder
    createCertificateBuilder(EncryptionKeyPair keyPair, UUID certificateType) {
        CertificateBuilder other = createCertificateBuilder(certificateType);
        EncryptedCertificateBuilder that =
            new EncryptedCertificateBuilder(keyPair);
        that.fields = other.fields;
        other = null;

        return that;
    }

    /**
     * Add an encrypted byte field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The byte value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedByte(int fieldId, byte value) {
        return
            addEncryptedByteArray(fieldId, serializeByte(value));
    }

    /**
     * Add an encrypted short field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The short value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedShort(int fieldId, int value) {
        return
            addEncryptedByteArray(fieldId, serializeShort(value));
    }

    /**
     * Add an encrypted int field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The int value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedInt(int fieldId, int value) {
        return
            addEncryptedByteArray(fieldId, serializeInt(value));
    }

    /**
     * Add an encrypted long field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The long value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedLong(int fieldId, long value) {
        return
            addEncryptedByteArray(fieldId, serializeLong(value));
    }

    /**
     * Add an encrypted UUID field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The UUID value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedUUID(int fieldId, UUID value) {
        return
            addEncryptedByteArray(fieldId, serializeUUID(value));
    }

    /**
     * Add an encrypted String field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The String value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedString(int fieldId, String value) {
        return
            addEncryptedByteArray(fieldId, serializeString(value));
    }

    /**
     * Add an encrypted Date field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The Date value for this field.  It is encoded as
     *                seconds since Epoch.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedDate(int fieldId, Date value) {
        return
            addEncryptedByteArray(fieldId, serializeDate(value));
    }

    /**
     * Add an encrypted byte array field to the certificate list.
     *
     * @param fieldId The short identifier for this field.  16-bit
     *                unsigned.
     * @param value   The byte array value for this field.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedByteArray(int fieldId, byte[] value) {
        byte[] encryptedValue =
            encryptData(secretKey, serializeLong(iv++), value);

        addByteArray(fieldId, encryptedValue);

        return this;
    }

    /**
     * Add an encrypted shared secret for a given entity and public encryption
     * key.
     *
     * @param entityId  The identifier of the entity for this shared secret.
     * @param publicKey The public encryption key for this entity.
     * @return this builder for additional operations.
     */
    public EncryptedCertificateBuilder
    addEncryptedSharedSecret(UUID entityId, EncryptionPublicKey publicKey) {
        byte[] encryptedKey =
            encryptKey(keyPair.getPrivateKey(), publicKey, secretKey);

        //build a dummy fragment using the entity UUID and public key for now
        CertificateBuilder fragmentBuilder =
            CertificateBuilder.createCertificateFragmentBuilder();
        fragmentBuilder
            .addUUID(Field.VELO_ENCRYPTED_SHARED_SECRET_ENTITY_UUID, entityId)
            .addByteArray(
                Field.VELO_ENCRYPTED_SHARED_SECRET_KEY,
                encryptedKey);

        //add this fragment to the builder.
        addByteArray(
            Field.VELO_ENCRYPTED_SHARED_SECRET_FRAGMENT,
            fragmentBuilder.emit().toByteArray());

        return this;
    }

    private EncryptionKeyPair keyPair;
    private byte[] secretKey;
    private long iv;

    /**
     * Generate an encryption key suitable for this builder.
     */
    private static native byte[] generateEncryptionKey();

    /**
     * Encrypt data in the input buffer, returning an encrypted buffer.
     *
     * @param key   The key to use to encrypt this data.
     * @param iv    The initialization vector to use.
     * @param input The input to encrypt.
     *
     * @return the encrypted data with iv / HMAC prepended / appended.
     */
    private static native byte[] encryptData(
        byte[] key, byte[] iv, byte[] input);

    /**
     * Encrypt the key with a long-term key between two entities.
     *
     * @param localPrivateKey   The private key for this entity.
     * @param peerPublicKey     The public key of the peer.
     * @param inputKey          The input key to be encrypted.
     *
     * @return the encrypted key with iv / HMAC prepended / appended.
     */
    private static native byte[] encryptKey(
        EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
        byte[] inputKey);
}
