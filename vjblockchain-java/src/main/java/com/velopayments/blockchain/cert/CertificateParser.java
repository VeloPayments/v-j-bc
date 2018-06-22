package com.velopayments.blockchain.cert;

import com.velopayments.blockchain.init.Initializer;
import java.util.*;
import java.util.function.BiFunction;

public class CertificateParser {

    static {
        Initializer.init();
    }

    private byte[] certificate;
    private int size;
    private int rawSize;

    /**
     * Create a CertificateParser instance to parse the given certificate.
     *
     * @param rawCertificate The certificate to parse.
     */
    public CertificateParser(byte[] rawCertificate) {
        certificate = rawCertificate;
        rawSize = size = rawCertificate.length;
    }

    /**
     * Append a field to a certificate field list for a certificate map.
     *
     * @param appendValue The value to append.
     * @return the updated list.
     */
    private static BiFunction<Integer, List<byte[]>, List<byte[]>>
    appendField(List<byte[]> appendValue) {
        return (k, v) -> {
            if (v != null) {
                //if set, append this value
                v.addAll(appendValue);
                return v;
            } else {
                //if not set, return this value
                return appendValue;
            }
        };
    }

    /**
     * Parse a certificate, returning a Map of key-value pairs.
     *
     * @return the fields of a certificate, as key-value pairs.
     */
    public Map<Integer, List<byte[]>> parse() {
        List<AbstractMap.SimpleEntry<Integer, byte[]>> certFields =
            parseNative(certificate, size);

        HashMap<Integer, List<byte[]>> map =
            new HashMap<>();

        for (AbstractMap.SimpleEntry<Integer, byte[]> v : certFields) {
            List<byte[]> lst = new LinkedList<>();
            lst.add(v.getValue());

            map.compute(v.getKey(), appendField(lst));
        }

        return map;
    }

    /**
     * Perform certificate attestation, using the provided
     * CertificateParserDelegate to manage entity and contract resolution.
     *
     * @param delegate         The certificate delegate to use during
     *                         attestation to resolve entities.
     * @param blockHeight      Height of the blockchain at time of attestation.
     * @param validateContract Set to true if the contract should be validated.
     * @return true if the certificate is valid, and false otherwise.
     */
    public boolean attest(
        CertificateParserDelegate delegate, long blockHeight,
        boolean validateContract)
    throws AttestationException, UnknownArtifactTypeException,
           UnknownArtifactException, UnknownEntityException {

        return attestNative(delegate, blockHeight, validateContract);
    }

    /**
     * Internal parse method for parsing a certificate.
     *
     * @param cert     The raw certificate to parse.
     * @param certSize The size override to use for finding the end of
     *                 the certificate.
     * @return the fields of the certificate, as key-value pairs.
     */
    private native List<AbstractMap.SimpleEntry<Integer, byte[]>>
    parseNative(byte[] cert, int certSize);

    /**
     * Internal attestation method for performing certificate attestation.
     *
     * @param delegate         The parser delegate to use during attestation.
     * @param blockHeight      Height of the blockchain at time of attestation.
     * @param validateContract Set to true if the contract should be validated.
     * @return true if the certificate is valid, and false otherwise.
     */
    private native boolean
    attestNative(
        CertificateParserDelegate delegate, long blockHeight,
        boolean validateContract)
    throws AttestationException, UnknownArtifactTypeException,
           UnknownArtifactException, UnknownEntityException;
}
