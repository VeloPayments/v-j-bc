package com.velopayments.blockchain.cert;

/**
 * Certificate Contract verification interface.
 */
public interface CertificateContract {

    /**
     * A contract function verifies a certificate.  It should return true if the
     * certificate is valid, and false otherwise.
     */
    boolean verify(CertificateParser p);
}
