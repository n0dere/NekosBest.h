/*
    Copyright (c) 2023 n0dere
    This software is licensed under the MIT License.
     _   _      _             ____            _     _     
    | \ | | ___| | _____  ___| __ )  ___  ___| |_  | |__  
    |  \| |/ _ \ |/ / _ \/ __|  _ \ / _ \/ __| __| | '_ \ 
    | |\  |  __/   < (_) \__ \ |_) |  __/\__ \ |_ _| | | |
    |_| \_|\___|_|\_\___/|___/____/ \___||___/\__(_)_| |_|    

    https://github.com/n0dere/NekosBest.h
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

NbHttpResponse *nbClientApiGet(NbClient client, const char *pQueryFormat, ...);

const char *nbClientPickRandomCategory(NbClient client);

NbImageFormat nbGetCategoryImageFormat(const char *pCategory);

bool nbValidateCategory(const char *pCategory);

#endif /* _CLIENT_H_ */