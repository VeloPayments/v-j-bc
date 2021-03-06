Changelog
=========

The following changelog catalogues changes between tagged releases.

Release 0.1.4
-------------

* more efficient implementation of EncryptedDocumentBuilder and EncryptedDocumentReader.
* Bugfix: AgentConnection.getLastBlockIdForArtifactById was throwing 
  java.lang.UnsatisfiedLinkError due to missing native code implementation.

Release 0.1.3
-------------

* Added EncryptedDocumentBuilder and EncryptedDocumentReader. 
* Fixed issue with wrong root block UUID being returned in some cases.
* Exposed a getCertificate method on CertificateReader.
* Added SimpleStreamCipher, which provides a safe way to encrypt data.
* Added Key.createFromPassword for creating a symmetric encryption key from a
  password using PBKDF2.

Release 0.1.2
-------------

* Added ZonedDateTime serialization methods to CertificateBuilder and
  CertificateParser.
* Added methods to AgentConnection to get the next and previous transaction
  identifiers from a given transaction identifier.
* Added AgentConnection.getFirstTransactionIdForArtifactById and
  AgentConnection.getLastTransactionIdForArtifactById.
* Added getBlockIdByHeight.
* Changed the internal representation of the root block ID from the zero UUID to
  the UUID used in the blockchain spec.  This breaks assumptions.
* Fixed consistency issues by disabling `MDB_WRITEMAP` feature.  Fixed JVM crash
  issues associated with `MDB_WRITEMAP` assumptions.
* Added null pointer checks that throw NPEs when null parameters are passed to
  native methods.

Release 0.1.1
-------------

* Added signature verification to Signing keys.
* Made exceptions unchecked.
* Updated AgentConnection.submit() to return CompletableFuture instead of
  Future.
* Fixed issue with setting previous block ID that caused block traversal errors.

Release 0.1.0
-------------

Simple "hack" version using an internal blockchain database.
