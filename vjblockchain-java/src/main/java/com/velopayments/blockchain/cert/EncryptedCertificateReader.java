package com.velopayments.blockchain.cert;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.init.Initializer;
import java.util.List;
import java.util.UUID;

public class EncryptedCertificateReader extends CertificateReader {

    /**
     * Create an EncryptedCertificateReader from a UUID, a private key, a public
     * key, and a CertificateParser.
     *
     * @param uuid              The uuid of the entity reading the certificate.
     * @param localPrivateKey   The private key of the entity reading this cert.
     * @param peerPublicKey     The public key of the peer that created this
     *                          cert.
     * @param parser            AThe parser to read with this reader.
     */
    public EncryptedCertificateReader(
                UUID uuid, EncryptionPrivateKey localPrivateKey,
                EncryptionPublicKey peerPublicKey, CertificateParser parser) {

        super(parser);
        this.uuid = uuid;
        this.localPrivateKey = localPrivateKey;
        this.peerPublicKey = peerPublicKey;
    }

    /**
     * Get the first encrypted field matching a given short fieldId.
     *
     * @param fieldId The short fieldId to retrieve.
     * @return a CertificateFieldReader for this field.
     * @throws MissingFieldException if the field does not exist.
     */
    public CertificateFieldReader getFirstEncrypted(int fieldId)
        throws FieldConversionException, MissingFieldException {

        return getEncrypted(fieldId, 0);
    }

    /**
     * Get the nth encrypted field matching a given short fieldId.
     *
     * @param fieldId The short fieldId to retrieve.
     * @param index   The nth field to return.
     * @return a CertificateFieldReader for this field.
     * @throws MissingFieldException if the field does not exist.
     */
    public CertificateFieldReader getEncrypted(int fieldId, int index)
        throws FieldConversionException, MissingFieldException {

        /* the loadSecretKey method must be called first. */
        if (null == secretKey) {
            throw new IllegalStateException("Secret key must be loaded.");
        }

        /* get all matching fields. */
        List<byte[]> fields = certFields.get(fieldId);

        /* we need a valid field list and appropriate index to continue. */
        if (null == fields || fields.size() <= index) {
            throw new MissingFieldException("Missing field.");
        } else {
            byte[] rawValue = fields.get(index);

            /* the field must be greater than 8 bytes in length. */
            if (8 >= rawValue.length) {
                throw new FieldConversionException("Invalid encrypted field.");
            }

            return
                new CertificateFieldReader(
                        decryptNative(
                            Initializer.getInstance(), secretKey, rawValue));
        }
    }

    /**
     * Scan through the encrypted keys, loading the encrypted key for the local
     * entity.
     */
    public void loadSecretKey()
        throws FieldConversionException, MissingFieldException {

        List<byte[]> fields =
            certFields.get(Field.VELO_ENCRYPTED_SHARED_SECRET_FRAGMENT);

        if (null == fields) {
            throw new MissingFieldException("Missing secret key field.");
        } else {
            for (byte[] i : fields) {
                CertificateReader r =
                    new CertificateReader(
                            new CertificateParser(
                                Certificate.fromByteArray(i)));
                UUID euuid =
                    r.getFirst(Field.VELO_ENCRYPTED_SHARED_SECRET_ENTITY_UUID)
                        .asUUID();

                if (euuid.equals(uuid)) {
                    secretKey =
                        decryptSecretNative(
                            Initializer.getInstance(), localPrivateKey,
                            peerPublicKey,
                            r.getFirst(Field.VELO_ENCRYPTED_SHARED_SECRET_KEY)
                                .asByteArray());
                    return;
                }
            }

            /* if we have made it this far and have not decrypted a secret key,
             * then our secret key is missing. */
            if (null == secretKey) {
                throw new MissingFieldException("Missing secret key.");
            }
        }
    }

    /**
     * Recover the secret key from the given local private key, peer public key,
     * and encrypted key.
     *
     * @param nativeInst the native instance pointer.
     * @param localPrivateKey   The local private key.
     * @param peerPublicKey     The peer public key.
     * @param encryptedKey      The encrypted key to decrypt.
     *
     * @return the decrypted key.
     */
    private static native byte[] decryptSecretNative(
        long nativeInst, EncryptionPrivateKey localPrivateKey,
        EncryptionPublicKey peerPublicKey, byte[] encryptedKey);

    /**
     * Decrypt the input value using the provided secret key.
     *
     * @param nativeInst the native instance pointer.
     * @param secretKey     The secret key to use to decrypt this value.
     * @param input         The input value to decrypt.
     *
     * @return the decrypted value.
     */
    private static native byte[] decryptNative(
        long nativeInst, byte[] secretKey, byte[] input);

    private UUID uuid;
    private EncryptionPrivateKey localPrivateKey;
    private EncryptionPublicKey peerPublicKey;
    private byte[] secretKey;
}
