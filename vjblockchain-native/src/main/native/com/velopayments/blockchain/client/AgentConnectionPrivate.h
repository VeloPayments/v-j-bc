/**
 * \file AgentConnectionPrivate.h
 *
 * Private data used by the JNI methods for AgentConnection.
 */

#ifndef  PRIVATE_AGENT_CONNECTION_PRIVATE_HEADER_GUARD
# define PRIVATE_AGENT_CONNECTION_PRIVATE_HEADER_GUARD

#include <lmdb.h>

/**
 * \brief Key in the master database for the latest block record.
 */
extern uint8_t AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK[16];

/**
 * \brief Internal structure holding the database details needed by the client
 * library.
 */
typedef struct agent_connection_details
{
    MDB_env* env;
    MDB_dbi master_db;
    MDB_dbi artifact_db;
    MDB_dbi block_db;
    MDB_dbi txn_db;
    MDB_dbi block_height_db;
} agent_connection_details_t;

/**
 * \brief Internal structure holding the transaction record for a transaction.
 */
typedef struct transaction_record
{
    uint8_t transaction_uuid[16];
    uint8_t block_uuid[16];
    uint8_t prev_transaction_uuid[16];
    uint8_t next_transaction_uuid[16];
    size_t transaction_size;
} transaction_record_t;

/**
 * \brief Internal structure holding the artifact record for an artifact.
 */
typedef struct artifact_record
{
    uint8_t artifact_uuid[16];
    uint8_t first_block_uuid[16];
    uint8_t first_transaction_uuid[16];
    uint8_t last_block_uuid[16];
    uint8_t last_transaction_uuid[16];
} artifact_record_t;

/**
 * \brief Internal structure holding the block record for a block.
 */
typedef struct block_record
{
    uint8_t block_uuid[16];
    uint8_t previous_block_id[16];
    uint8_t next_block_id[16];
    uint64_t block_height;
    size_t block_size;
} block_record_t;

#endif /*PRIVATE_AGENT_CONNECTION_PRIVATE_HEADER_GUARD*/
