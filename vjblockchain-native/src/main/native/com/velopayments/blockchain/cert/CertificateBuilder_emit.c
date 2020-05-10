/**
 * \file CertificateBuilder_emit.c
 *
 * Emit the certificate as a byte array.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <com/velopayments/blockchain/init/init.h>
#include <string.h>
#include <vccrypt/suite.h>
#include <vpr/parameters.h>
#include <com_velopayments_blockchain_cert_CertificateBuilder.h>

/*
 * Class:     com_velopayments_blockchain_cert_CertificateBuilder
 * Method:    emit
 * Signature: ()Lcom/velopayments/blockchain/cert/Certificate;
 */
JNIEXPORT jobject JNICALL
Java_com_velopayments_blockchain_cert_CertificateBuilder_emit(
    JNIEnv *env, jobject that, jlong nativeInst)
{
    jobject retval = NULL;

    /* function contract enforcement */
    MODEL_ASSERT(MODEL_PROP_VALID_JNI_ENV(env));
    MODEL_ASSERT(0 != nativeInst);
    MODEL_ASSERT(NULL != that);

    /* get a pointer to the native instance. */
    vjblockchain_native_instance* native_inst =
        (vjblockchain_native_instance*)nativeInst;

    /* verify that the vjblockchain library has been initialized. */
    if (!native_inst || !native_inst->initialized)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vjblockchain not initialized.");
        return NULL;
    }

    /* don't emit an empty certificate */
    jobject lst =
        (*env)->GetObjectField(
            env, that, native_inst->CertificateBuilder.field_fields);
    jint lst_size =
        (*env)->CallIntMethod(
            env, lst, native_inst->LinkedList.size);
    if (lst_size <= 0)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "cannot emit empty certificate fragment.");
        return NULL;
    }
    
    /* compute the size of the certificate to emit */
    jsize cert_size = 0;
    for (jint i = 0; i < lst_size; ++i)
    {
        jobject pair =
            (*env)->CallObjectMethod(
                env, lst, native_inst->LinkedList.get, i);
        jbyteArray array =
            (jbyteArray)(*env)->CallObjectMethod(
                env, pair, native_inst->SimpleEntry.getValue);
        jsize array_len = 
            (*env)->GetArrayLength(env, array);

        cert_size += FIELD_TYPE_SIZE + FIELD_SIZE_SIZE + array_len;

        /* clean up references */
        (*env)->DeleteLocalRef(env, pair);
        (*env)->DeleteLocalRef(env, array);
    }

    /* create builder context */
    vccert_builder_context_t builder;
    if (0 != vccert_builder_init(
                    &native_inst->builder_opts, &builder, cert_size))
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "vccert_builder_init");
        return NULL;
    }

    /* for each field; add it to the builder */
    for (jint i = 0; i < lst_size; ++i)
    {
        jobject pair =
            (*env)->CallObjectMethod(
                env, lst, native_inst->LinkedList.get, i);
        jobject int_obj =
            (*env)->CallObjectMethod(
                env, pair, native_inst->SimpleEntry.getKey);
        jint field_type =
            (*env)->CallIntMethod(
                env, int_obj, native_inst->Integer.intValue);
        jbyteArray array =
            (jbyteArray)(*env)->CallObjectMethod(
                env, pair, native_inst->SimpleEntry.getValue);
        jsize array_len = 
            (*env)->GetArrayLength(env, array);
        jbyte* field_val = (*env)->GetByteArrayElements(env, array, NULL);

        if (0 !=
            vccert_builder_add_short_buffer(
                &builder, field_type, (const uint8_t*)field_val, array_len))
        {
            (*env)->ReleaseByteArrayElements(env, array, field_val, JNI_ABORT);
            (*env)->ThrowNew(
                env, native_inst->IllegalStateException.classid,
                "vccert_builder_add_short_buffer");
            retval = NULL;
            goto dispose_builder;
        }

        /* clean up references */
        (*env)->DeleteLocalRef(env, pair);
        (*env)->DeleteLocalRef(env, int_obj);
        (*env)->ReleaseByteArrayElements(env, array, field_val, JNI_ABORT);
        (*env)->DeleteLocalRef(env, array);
    }

    /* use emit() to get the current size and assert that it matches */
    size_t emitted_size = 0;
    const uint8_t* outbuf = vccert_builder_emit(&builder, &emitted_size);
    if (emitted_size != (unsigned)cert_size)
    {
        (*env)->ThrowNew(
            env, native_inst->IllegalStateException.classid,
            "emitted_size != cert_size");
        retval = NULL;
        goto dispose_builder;
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
            env, native_inst->Certificate.classid,
            native_inst->Certificate.fromByteArray, out);

    /* clean up */
dispose_builder:
    dispose((disposable_t*)&builder);

    return retval;
}
