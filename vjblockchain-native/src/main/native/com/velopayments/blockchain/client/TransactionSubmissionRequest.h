/**
 * \file TransactionSubmissionRequest.h
 *
 * Class and method exports for TransactionSubmissionRequest.  This header
 * includes a static registration mechanism for creating global references to
 * the TransactionSubmissionRequest class, so that TransactionSubmissionRequest
 * instances can be created from C and methods for these instances can be called
 * from C.
 */

#ifndef  PRIVATE_TRANSACTION_SUBMISSION_REQUEST_HEADER_GUARD
# define PRIVATE_TRANSACTION_SUBMISSION_REQUEST_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following TransactionSubmissionRequest references and make them
 * global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int TransactionSubmissionRequest_register(JNIEnv* env);

/* public class com.velopayments.blockchain.client.TransactionSubmissionRequest
 */
extern jclass TransactionSubmissionRequest;

/* public com.velopayments.blockchain.client.TransactionSubmissionRequest(
 *      com.velopayments.blockchain.cert.Certificate);
 * descriptor: (Lcom/velopayments/blockchain/cert/Certificate;)V
 */
extern jmethodID TransactionSubmissionRequest_init;

/* public com.velopayments.blockchain.client.TransactionStatus getStatus();
 * descriptor: ()Lcom/velopayments/blockchain/client/TransactionStatus;
 */
extern jmethodID TransactionSubmissionRequest_getStatus;

/* public void setStatus(com.velopayments.blockchain.client.TransactionStatus);
 * descriptor: (Lcom/velopayments/blockchain/client/TransactionStatus;)V
 */
extern jmethodID TransactionSubmissionRequest_setStatus;

/* public java.util.concurrent.CompletableFuture<
 *              com.velopayments.blockchain.client.TransactionStatus>
 * getStatusFuture();
 * descriptor: ()Ljava/util/concurrent/CompletableFuture;
 */
extern jmethodID TransactionSubmissionRequest_getStatusFuture;

/* public com.velopayments.blockchain.cert.Certificate getTxnData();
 * descriptor: ()Lcom/velopayments/blockchain/cert/Certificate;
 */
extern jmethodID TransactionSubmissionRequest_getTxnData;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_TRANSACTION_SUBMISSION_REQUEST_HEADER_GUARD*/
