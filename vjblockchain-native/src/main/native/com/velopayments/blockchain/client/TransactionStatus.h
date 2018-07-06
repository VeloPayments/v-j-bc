/**
 * \file TransactionStatus.h
 *
 * Class and method exports for TransactionStatus.  This header includes a
 * static registration mechanism for creating global references to the
 * TransactionStatus class, so that TransactionStatus instances can be created
 * from C and methods for these instances can be called from C.
 */

#ifndef  PRIVATE_TRANSACTION_STATUS_HEADER_GUARD
# define PRIVATE_TRANSACTION_STATUS_HEADER_GUARD

#include <jni.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * Register the following TransactionStatus references and make them global.
 *
 * Note: this method must be called in a synchronized static initialization
 * block in Java to ensure that there isn't a registration race.  This method
 * must be called before any of the following references are used.
 *
 * \param env   JNI environment to use.
 *
 * \returns 0 on success and non-zero on failure.
 */
int TransactionStatus_register(JNIEnv* env);

/* public class com.velopayments.blockchain.client.TransactionStatus {
 */
extern jclass TransactionStatus;

/* public static final com.velopayments.blockchain.client.TransactionStatus
 * PENDING;
 * descriptor: Lcom/velopayments/blockchain/client/TransactionStatus;
 */
extern jfieldID TransactionStatus_field_PENDING;

/* public static final com.velopayments.blockchain.client.TransactionStatus
 * FAILED;
 * descriptor: Lcom/velopayments/blockchain/client/TransactionStatus;
 */
extern jfieldID TransactionStatus_field_FAILED;

/* public static final com.velopayments.blockchain.client.TransactionStatus
 * CANCELED;
 * descriptor: Lcom/velopayments/blockchain/client/TransactionStatus;
 */
extern jfieldID TransactionStatus_field_CANCELED;

/* public static final com.velopayments.blockchain.client.TransactionStatus
 * SUCCEEDED;
 * descriptor: Lcom/velopayments/blockchain/client/TransactionStatus;
 */
extern jfieldID TransactionStatus_field_SUCCEEDED;

/* public static final com.velopayments.blockchain.client.TransactionStatus
 * UNKNOWN_DISCONNECTED;
 * descriptor: Lcom/velopayments/blockchain/client/TransactionStatus;
 */
extern jfieldID TransactionStatus_field_DISCONNECTED;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_TRANSACTION_STATUS_HEADER_GUARD*/
