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

#include "client.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include <tinycthread.h>

#include "nekosbest.h"
#include "httpclient.h"

/* https://en.wikipedia.org/wiki/Xorshift */
static uint32_t XORSHIFT32(
    uint32_t prevState
) {
    prevState ^= prevState << 13;
	prevState ^= prevState >> 17;
	prevState ^= prevState << 5;

    return prevState;
}

unsigned int nbClientRandom(
    NbClient client,
    unsigned int min,
    unsigned int max
) {
    if (client == NULL)
        return 0;

    client->randomState = XORSHIFT32(client->randomState);

    return min + (client->randomState % (max - min + 1));
}

void nbClientSetLastError(
    NbClient client,
    NbResult error
) {
    NbResult *tssResultPtr = NULL;

    if (client == NULL)
        return;

    tssResultPtr = tss_get(client->errorStorage);

    if (tssResultPtr == NULL) {
        tssResultPtr = calloc(1, sizeof *tssResultPtr);

        if (tssResultPtr == NULL)
            return;
        
        if (tss_set(client->errorStorage, tssResultPtr) != thrd_success) {
            free(tssResultPtr);
            return;
        }
    }

    *tssResultPtr = error;
}

NB_API NbResult nbCreateClient(
    NbClient *pClient
) {
    NbResult result;
    NbClient pTmpClient = calloc(1, sizeof *pTmpClient);

    if (pTmpClient == NULL)
        return NB_RESULT_NO_MEMORY;
    
    if (tss_create(&pTmpClient->errorStorage, free) != thrd_success) {
        free(pTmpClient);
        return NB_RESULT_CLIENT_INTERNAL_INIT_ERROR;
    }

    if (mtx_init(&pTmpClient->mutex, mtx_plain) != thrd_success) {
        tss_delete(pTmpClient->errorStorage);
        free(pTmpClient);
        return NB_RESULT_CLIENT_INTERNAL_INIT_ERROR;
    }

    result = nbHttpClientCreate(&pTmpClient->httpClient);

    if (result != NB_RESULT_OK) {
        mtx_destroy(&pTmpClient->mutex);
        tss_delete(pTmpClient->errorStorage);
        free(pTmpClient);
        return result;
    }
    
    pTmpClient->randomState = XORSHIFT32((uint32_t) time(NULL));

    *pClient = pTmpClient;

    return NB_RESULT_OK;
}

NB_API NbResult nbClientGetLastError(
    const NbClient client
) {
    NbResult *tssResultPtr = NULL;

    if (client == NULL)
        return NB_RESULT_INVALID_PARAMETER;

    tssResultPtr = tss_get(client->errorStorage);

    if (tssResultPtr != NULL)
        return *tssResultPtr;
    
    return NB_RESULT_OK;
}

NB_API void nbDestroyClient(
    NbClient client
) {
    if (client != NULL) {
        nbHttpClientDestroy(client->httpClient);
        mtx_destroy(&client->mutex);
        tss_delete(client->errorStorage);
        free(client);
    }
}