package com.velopayments.blockchain.cert;

import java.util.List;
import java.util.Map;
import java.util.Set;

public class CertificateReader {

    protected Map<Integer, List<byte[]>> certFields;

    /**
     * Create a CertificateReader from a CertificateParser.
     *
     * @param parser The parser to read with this reader.
     */
    public CertificateReader(CertificateParser parser) {
        this.certFields = parser.parse();
    }

    /**
     * Count the number of elements matching the given short field ID.
     *
     * @param fieldId The short fieldId for which a count should be
     *                taken.
     */
    public int count(int fieldId) {
        List<byte[]> fields = certFields.get(fieldId);

        if (null == fields) {
            return 0;
        } else {
            return fields.size();
        }
    }

    /**
     * Get the set of fieldIds found in this certificate.
     *
     * @return the set of fieldIds found in this certificate.
     */
    public Set<Integer> getFields() {
        return certFields.keySet();
    }

    /**
     * Get the first field matching a given short fieldId.
     *
     * @param fieldId The short fieldId to retrieve.
     * @return a CertificateFieldReader for this field.
     * @throws MissingFieldException if the field does not exist.
     */
    public CertificateFieldReader getFirst(int fieldId)
    throws MissingFieldException {
        return get(fieldId, 0);
    }

    /**
     * Get the nth field matching a given short fieldId.
     *
     * @param fieldId The short fieldId to retrieve.
     * @param index   The nth field to return.
     * @return a CertificateFieldReader for this field.
     * @throws MissingFieldException if the field does not exist.
     */
    public CertificateFieldReader get(int fieldId, int index)
        throws MissingFieldException {
        List<byte[]> fields = certFields.get(fieldId);

        if (null == fields || fields.size() <= index) {
            throw new MissingFieldException("Missing field.");
        } else {
            return new CertificateFieldReader(fields.get(index));
        }
    }
}
