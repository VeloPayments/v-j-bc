/**
 * \file AgentConnection.h
 *
 * Class and method exports for AgentConnection.  This header includes a
 * static registration mechanism for creating global references to the
 * AgentConnection class, so that AgentConnection instances can be created
 * from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_AGENT_CONNECTION_HEADER_GUARD
# define PRIVATE_AGENT_CONNECTION_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following AgentConnection references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int AgentConnection_register(JNIEnv* env);

/* public class com.velopayments.blockchain.client.AgentConnection {
 */
extern jclass AgentConnection;

/* public com.velopayments.blockchain.client.AgentConnection(
 *      java.lang.String, java.util.UUID,
 *      com.velopayments.blockchain.crypt.EncryptionPrivateKey)
 *  throws java.io.IOException;
 * descriptor:
 *      (Ljava/lang/String;Ljava/util/UUID;
 *       Lcom/velopayments/blockchain/crypt/EncryptionPrivateKey;)V
 */
extern jmethodID AgentConnection_init;

/* public void close() throws java.io.IOException;
 * descriptor: ()V
 */
extern jmethodID AgentConnection_close;

/* public java.util.concurrent.CompletableFuture<
 *      com.velopayments.blockchain.client.TransactionStatus>
 * submit(com.velopayments.blockchain.cert.Certificate)
 * throws java.io.IOException;
 * descriptor: (Lcom/velopayments/blockchain/cert/Certificate;)
 *             Ljava/util/concurrent/CompletableFuture;
 */
extern jmethodID AgentConnection_submit;

/* public java.util.UUID getLatestBlockId() throws java.io.IOException;
 * descriptor: ()Ljava/util/UUID;
 */
extern jmethodID AgentConnection_getLatestBlockId;

/* public java.util.Optional<Certificate>
 * getBlockById(java.util.UUID) throws java.io.IOException;
 * descriptor: (Ljava/util/UUID;)Ljava/util/Optional;
 */
extern jmethodID AgentConnection_getBlockById;

/* public java.util.Optional<Certificate>
 * getTransactionById(java.util.UUID) throws java.io.IOException;
 * descriptor: (Ljava/util/UUID;)Ljava/util/Optional;
 */
extern jmethodID AgentConnection_getTransactionById;

/* privatejava.util.List<
 *      com.velopayments.blockchain.client.TransactionSubmissionRequest>
 * submissionList;
 * descriptor: Ljava/util/List;
 */
extern jfieldID AgentConnection_field_submissionList;

/* private long handle;
 * descriptor: J
 */
extern jfieldID AgentConnection_field_handle;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_AGENT_CONNECTION_HEADER_GUARD*/
