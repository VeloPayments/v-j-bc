/**
 * \file init.h
 *
 * Reference to init flag and Velo C related control structures.
 */

#ifndef  PRIVATE_INIT_HEADER_GUARD
# define PRIVATE_INIT_HEADER_GUARD

#include <stdbool.h>
#include <vccert/builder.h>
#include <vccert/parser.h>
#include <vccrypt/suite.h>
#include <vpr/allocator.h>

/* make this header C++ friendly */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

extern volatile bool vjblockchain_initialized;
extern allocator_options_t alloc_opts;
extern vccrypt_suite_options_t crypto_suite;
extern vccert_builder_options_t builder_opts;

/* make this header C++ friendly */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*PRIVATE_INIT_HEADER_GUARD*/
