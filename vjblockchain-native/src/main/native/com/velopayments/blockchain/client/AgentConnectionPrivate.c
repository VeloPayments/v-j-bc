/**
 * \file AgentConnectionPrivate.c
 *
 * \brief Private data and methods associated with AgentConnection.
 */

#include "AgentConnectionPrivate.h"

/**
 * \brief Key in the master database for the latest block record.
 */
uint8_t AGENT_CONNECTION_MASTER_KEY_LATEST_BLOCK[16] = {
    0x27, 0x54, 0xdf, 0x63, 0xdc, 0x3b, 0x42, 0xc0,
    0xa0, 0xe6, 0x3b, 0x95, 0x6d, 0x73, 0x6b, 0xf1 };
