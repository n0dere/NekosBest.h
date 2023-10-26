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

#include "client.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include <tinycthread.h>

#include "nekosbest.h"
#include "httpclient.h"

/* https://en.wikipedia.org/wiki/Xorshift */
static uint32_t xorshift32(uint32_t prevState)
{
    prevState ^= prevState << 13;
	prevState ^= prevState >> 17;
	prevState ^= prevState << 5;

    return prevState;
}

uint32_t nbClientRandom(NbClient client, uint32_t min, uint32_t max)
{
    if (client == NULL)
        return 0;

    mtx_lock(&client->mutex);
    client->randomState = xorshift32(client->randomState);
    mtx_unlock(&client->mutex);

    return min + (client->randomState % (max - min + 1));
}

void nbClientSetLastError(NbClient client, NbResult error)
{
    NbResult *tssResultPtr = NULL;

    if (client == NULL)
        return;

    tssResultPtr = tss_get(client->errorStorage);

    if (tssResultPtr != NULL) {
        *tssResultPtr = error;
        return;
    }

    tssResultPtr = calloc(1, sizeof *tssResultPtr);

    if (tssResultPtr == NULL)
        return;
    
    if (tss_set(client->errorStorage, tssResultPtr) != thrd_success) {
        free(tssResultPtr);
        return;
    }

    *tssResultPtr = error;
}

NbHttpResponse *nbClientApiGet(NbClient client, const char *pQuery, ...)
{
    NbHttpResponse *pResponse = NULL;
    char url[NB_URL_MAX_SIZE] = {0};
    NbResult result = NB_RESULT_OK;
    size_t urlLen;
    va_list args;

    if (client == NULL)
        return NULL;

    if (pQuery == NULL) {
        nbClientSetLastError(client, NB_RESULT_INVALID_PARAMETER);
        return NULL;
    }

    urlLen = NB_URL_MAX_SIZE - nbGetApiInfo()->apiBaseUrlLen - 1;

    strncpy(url, nbGetApiInfo()->pApiBaseUrl, urlLen);

    va_start(args, pQuery);

    vsnprintf(url + nbGetApiInfo()->apiBaseUrlLen, urlLen, pQuery, args);
    
    va_end(args);

    mtx_lock(&client->mutex);
    result = nbHttpClientGet(client->httpClient, &pResponse, url);
    mtx_unlock(&client->mutex);

    nbClientSetLastError(client, result);

    return pResponse;
}

NB_API NbResult nbCreateClient(NbClient *pClient)
{
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
    
    pTmpClient->randomState = xorshift32((uint32_t) time(NULL));

    *pClient = pTmpClient;

    return NB_RESULT_OK;
}

NB_API NbResult nbClientGetLastError(const NbClient client)
{
    NbResult *tssResultPtr = NULL;

    if (client == NULL)
        return NB_RESULT_INVALID_PARAMETER;

    tssResultPtr = tss_get(client->errorStorage);

    if (tssResultPtr != NULL)
        return *tssResultPtr;
    
    return NB_RESULT_OK;
}

NB_API void nbDestroyClient(NbClient client)
{
    if (client != NULL) {
        nbHttpClientDestroy(client->httpClient);
        mtx_destroy(&client->mutex);
        tss_delete(client->errorStorage);
        free(client);
    }
}