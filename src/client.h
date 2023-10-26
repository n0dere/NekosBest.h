/*                  _             _               _     _     
 *       _ __   ___| | _____  ___| |__   ___  ___| |_  | |__  
 *      | '_ \ / _ \ |/ / _ \/ __| '_ \ / _ \/ __| __| | '_ \ 
 *      | | | |  __/   < (_) \__ \ |_) |  __/\__ \ |_ _| | | |
 *      |_| |_|\___|_|\_\___/|___/_.__/ \___||___/\__(_)_| |_|
 *                                                  
 *      Copyright (c) 2023 n0dere
 *      This software is licensed under the MIT License.
 * 
 *      https://github.com/n0dere/NekosBest.h
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include <tinycthread.h>

#include "httpclient.h"

struct _NbClient {
    /* A mutex that protects the access to the client fields */
    mtx_t mutex;
    /* A thread-specific storage for the last error code */
    tss_t errorStorage;
    /* A state for the random number generator */
    uint32_t randomState;
    /* A HTTP client that handles the network requests */
    NbHttpClient httpClient;
    struct {
        /* The remaining number of requests allowed by the rate limit */
        size_t remaining;
        /* The time when the rate limit will be reset */
        time_t resetsIn;
    } ratelimit;
};

/* Generate a random number between min and max using the client's random
 * state. (thread-safe)
 *
 * Args:    client - a client object
 *          min - the lower bound of the random number (inclusive)
 *          max - the upper bound of the random number (exclusive)
 *
 * Returns: a random number between min and max
 */
uint32_t nbClientRandom(NbClient client, uint32_t min, uint32_t max);

/* Sets the last error code for the current thread using the client's error
 * storage.
 *
 * Args:    client - a client object
 *          error - an error code to be set
 */
void nbClientSetLastError(NbClient client, NbResult error);

/* Sends a GET request to the API endpoint with the given query and parameters.
 *
 * Args:    client - a client object
 *          pQuery - a format string for the query (ex: "/neko?amount=&d")
 *          ... - additional arguments for the format string
 *
 * Returns: a pointer to a NbHttpResponse object that contains the response
 *          data, or NULL if an error occurred. The caller is responsible for
 *          freeing the response object with nbHttpResponseDestroy when it is
 *          no longer needed.
 */
NbHttpResponse *nbClientApiGet(NbClient client, const char *pQuery, ...);

#endif /* _CLIENT_H_ */