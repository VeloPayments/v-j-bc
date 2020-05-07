/**
 * \file init.h
 *
 * Reference to init flag and Velo C related control structures.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  PRIVATE_INIT_HEADER_GUARD
# define PRIVATE_INIT_HEADER_GUARD

#include <stdbool.h>
#include <vccert/builder.h>
#include <vccert/parser.h>
#include <vccrypt/stream_cipher.h>
#include <vccrypt/suite.h>
#include <vpr/allocator.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

struct vjblockchain_native_instance;
typedef struct vjblockchain_native_instance vjblockchain_native_instance;

struct vjblockchain_native_instance
{
    bool initialized;
    allocator_options_t alloc_opts;
    vccrypt_suite_options_t crypto_suite;
    vccert_builder_options_t builder_opts;
};

extern vjblockchain_native_instance* native_inst;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_INIT_HEADER_GUARD*/
