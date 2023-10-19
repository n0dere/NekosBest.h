#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdint.h>
#include <time.h>

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

unsigned int nbClientRandom(
    NbClient client,
    unsigned int min,
    unsigned int max
);

void nbClientSetLastError(
    NbClient client,
    NbResult error
);

#endif /* _CLIENT_H_ */