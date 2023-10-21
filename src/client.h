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
    mtx_t mutex;
    tss_t errorStorage;
    uint32_t randomState;
    NbHttpClient httpClient;
    struct {
        size_t remaining;
        time_t resetsIn;
    } ratelimit;
};

unsigned int nbClientRandom(NbClient client, unsigned int min,
                            unsigned int max);

void nbClientSetLastError(NbClient client, NbResult error);

#endif /* _CLIENT_H_ */