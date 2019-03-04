/**
 * \file CertificateBuilder_signNative.c
 *
 * Sign the certificate and emit it as a byte array.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/cert/Certificate.h>
#include <com/velopayments/blockchain/cert/CertificateBuilder.h>
#include <com/velopayments/blockchain/init/init.h>
#include <java/lang/IllegalStateException.h>
#include <java/lang/IllegalArgumentException.h>
#include <java/lang/Integer.h>
#include <java/lang/NullPointerException.h>
#include <java/util/AbstractMap_SimpleEntry.h>
#include <java/util/LinkedList.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>
#include <com_velopayments_blockchain_cert_CertificateBuilder.h>

/*
 * Class:     com_velopayments_blockchain_cert_CertificateBuilder
 * Method:    signNative
 * Signature: ([B[B)Lcom/velopayments/blockchain/cert/Certificate;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_cert_CertificateBuilder_signNative(
    JNIEnv *env, jobject that, jbyteArray signer_id, jbyteArray private_key)
{
    jobject retval = NULL;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(NULL != that);
    MODEL_ASSERT(NULL != signer_id);
    MODEL_ASSERT(NULL != private_key);

    /* verify that the vjblockchain library has been initialized. */
    if (!vjblockchain_initialized)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "vjblockchain not initialized.");
        return NULL;
    }

    /* verify that the signer_id parameter is not null. */
    if (NULL == signer_id)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "signerId");
        return NULL;
    }

    /* verify that the private_key parameter is not null. */
    if (NULL == private_key)
    {
        (*env)->ThrowNew(
            env, NullPointerException, "privateKey");
        return NULL;
    }

    /* verify the signer id size matches */
    jsize signer_id_size = (*env)->GetArrayLength(env, signer_id);
    if (16 != signer_id_size)
    {
        (*env)->ThrowNew(
            env, IllegalArgumentException, "Signer ID is the wrong size");
        return NULL;
    }

    /* compute the size of the certificate to emit */
    jobject lst =
        (*env)->GetObjectField(env, that,CertificateBuilder_field_fields);
    jint lst_size = (*env)->CallIntMethod(env, lst, LinkedList_size);
    jsize cert_size = 0;
    for (jint i = 0; i < lst_size; ++i)
    {
        jobject pair = (*env)->CallObjectMethod(env, lst, LinkedList_get, i);
        jbyteArray array =
            (jbyteArray)(*env)->CallObjectMethod(
                env, pair, SimpleEntry_getValue);
        jsize array_len = 
            (*env)->GetArrayLength(env, array);

        cert_size += FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + array_len;

        /* clean up references */
        (*env)->DeleteLocalRef(env, pair);
        (*env)->DeleteLocalRef(env, array);
    }

    /* calculate the signature size and add this to the cert size */
    cert_size += FIELD_TYPE_SIZE*2 + FIELD_SIZE_SIZE*2 + 16 +
                 crypto_suite.sign_opts.signature_size;

    /* create builder context */
    vccert_builder_context_t builder;
    if (0 != vccert_builder_init(&builder_opts, &builder, cert_size))
    {
        (*env)->ThrowNew(env, IllegalStateException, "general error.");
        return NULL;
    }

    /* for each field; add it to the builder */
    for (jint i = 0; i < lst_size; ++i)
    {
        jobject pair = (*env)->CallObjectMethod(env, lst, LinkedList_get, i);
        jobject int_obj =
            (*env)->CallObjectMethod(env, pair, SimpleEntry_getKey);
        jint field_type =
            (*env)->CallIntMethod(env, int_obj, Integer_intValue);
        jbyteArray array =
            (jbyteArray)(*env)->CallObjectMethod(
                env, pair, SimpleEntry_getValue);
        jsize array_len = 
            (*env)->GetArrayLength(env, array);
        jbyte* field_val = (*env)->GetByteArrayElements(env, array, NULL);

        if (0 !=
            vccert_builder_add_short_buffer(
                &builder, field_type, (const uint8_t*)field_val, array_len))
        {
            (*env)->ReleaseByteArrayElements(env, array, field_val, JNI_ABORT);
            (*env)->ThrowNew(env, IllegalStateException, "general error.");
            retval = NULL;
            goto dispose_builder;
        }

        /* clean up references */
        (*env)->DeleteLocalRef(env, pair);
        (*env)->DeleteLocalRef(env, int_obj);
        (*env)->ReleaseByteArrayElements(env, array, field_val, JNI_ABORT);
        (*env)->DeleteLocalRef(env, array);
    }

    /* create a buffer for the private key */
    vccrypt_buffer_t priv;
    if (0 != vccrypt_suite_buffer_init_for_signature_private_key(
                &crypto_suite, &priv))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "Couldn't create private key buffer");
        retval = NULL;
        goto dispose_builder;
    }

    /* verify that the private key size matches the crypto suite size */
    jsize priv_size = (*env)->GetArrayLength(env, private_key);
    if ((unsigned)priv_size != priv.size)
    {
        (*env)->ThrowNew(
            env, IllegalArgumentException, "Private key is the wrong size");
        retval = NULL;
        goto dispose_priv;
    }

    /* get the private key from Java */
    jbyte* priv_bytes = (*env)->GetByteArrayElements(env, private_key, NULL);
    memcpy(priv.data, priv_bytes, priv.size);
    (*env)->ReleaseByteArrayElements(env, private_key, priv_bytes, JNI_ABORT);

    /* get the signer id raw bytes from Java */
    jbyte* signer_id_bytes =
        (*env)->GetByteArrayElements(env, signer_id, NULL);

    /* sign the certificate */
    if (0 != vccert_builder_sign(
                    &builder, (const uint8_t*)signer_id_bytes, &priv))
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "signature error");
        retval = NULL;
        goto release_signer_id;
    }

    /* use emit() to get the current size and assert that it matches */
    size_t emitted_size = 0;
    const uint8_t* outbuf = vccert_builder_emit(&builder, &emitted_size);
    if (emitted_size != (unsigned)cert_size)
    {
        (*env)->ThrowNew(
            env, IllegalStateException, "emitted_size != cert_size");
        retval = NULL;
        goto release_signer_id;
    }

    /* allocate a byte array of the appropriate size */
    jbyteArray out = (*env)->NewByteArray(env, cert_size);

    /* get the buffer of this array */
    jbyte* outData = (*env)->GetByteArrayElements(env, out, NULL);

    /* copy the buffer data to the output array */
    memcpy(outData, outbuf, cert_size);

    /* commit data to the byte array */
    (*env)->ReleaseByteArrayElements(env, out, outData, 0);

    /* set retval to the byte array */
    retval = 
        (*env)->CallStaticObjectMethod(
            env, Certificate, Certificate_fromByteArray, out);

    /* clean up */
release_signer_id:
    (*env)->ReleaseByteArrayElements(
        env, signer_id, signer_id_bytes, JNI_ABORT);

dispose_priv:
    dispose((disposable_t*)&priv);

dispose_builder:
    dispose((disposable_t*)&builder);

    return retval;
}
