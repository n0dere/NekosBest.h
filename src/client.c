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
#include <stdarg.h>
#include <stdio.h>

#include <tinycthread.h>

#include "nekosbest.h"
#include "httpclient.h"
#include "version.h"

#define URL_MAX             1024
#define CATEGORIES_COUNT    (sizeof aCategories / sizeof *aCategories)
#define CATEGORIES_PNG      4

static const char *aCategories[] = {
    /* PNG: */
    "kitsune", "neko", "husbando", "waifu",
    
    /* GIF: */
    "baka", "bite", "blush", "bored", "cry", "cuddle", "dance", "facepalm",
    "feed", "happy", "highfive", "hug", "kiss", "laugh", "pat", "pout",
    "shrug", "slap", "sleep", "smile", "smug", "stare", "think", "thumbsup",
    "tickle", "wave", "wink", "kick", "handhold", "punch", "shoot", "yeet",
    "poke", "nod", "nom", "nope", "handshake", "lurk", "peck", "yawn",
};

/* https://en.wikipedia.org/wiki/Xorshift */
static uint32_t XORSHIFT32(uint32_t prevState)
{
    prevState ^= prevState << 13;
	prevState ^= prevState >> 17;
	prevState ^= prevState << 5;

    return prevState;
}

unsigned int nbClientRandom(NbClient client, unsigned int min,
                            unsigned int max)
{
    if (client == NULL)
        return 0;

    client->randomState = XORSHIFT32(client->randomState);

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

NbHttpResponse *nbClientApiGet(NbClient client, const char *pQueryFormat, ...)
{
    char url[URL_MAX] = {0};
    NbHttpResponse *pResponse = NULL;
    NbResult result = NB_RESULT_OK;
    size_t urlLen;
    va_list args;

    if (client == NULL)
        return NULL;

    if (pQueryFormat == NULL) {
        nbClientSetLastError(client, NB_RESULT_INVALID_PARAMETER);
        return NULL;
    }

    urlLen = URL_MAX - nbGetApiInfo()->apiBaseUrlLen - 1;

    strncpy(url, nbGetApiInfo()->pApiBaseUrl, urlLen);

    va_start(args, pQueryFormat);
    vsnprintf(url + nbGetApiInfo()->apiBaseUrlLen, urlLen, pQueryFormat, args);
    va_end(args);

    result = nbHttpClientGet(client->httpClient, &pResponse, url);

    nbClientSetLastError(client, result);

    return pResponse;
}

const char *nbClientPickRandomCategory(NbClient client)
{
    if (client == NULL)
        return aCategories[0];
    
    return aCategories[nbClientRandom(client, 0, CATEGORIES_COUNT)];
}

NbImageFormat nbGetCategoryImageFormat(const char *pCategory)
{
    size_t i;

    if (pCategory == NULL)
        return NB_IMAGE_FORMAT_UNKNOWN;

    for (i = 0; i < CATEGORIES_COUNT; ++i) {
        if (strcmp(pCategory, aCategories[i]) == 0)
            break;
    }

    if (i >= CATEGORIES_COUNT)
        return NB_IMAGE_FORMAT_UNKNOWN;

    if (i < CATEGORIES_PNG)
        return NB_IMAGE_FORMAT_PNG;

    return NB_IMAGE_FORMAT_GIF;
}

bool nbValidateCategory(const char *pCategory)
{
    return nbGetCategoryImageFormat(pCategory) != NB_IMAGE_FORMAT_UNKNOWN;
}

NB_API const NbApiInfo *nbGetApiInfo(void)
{
    static const NbApiInfo nbApiInfo = {
        .apiVersion = 2,
        .pApiVersionString = "v2",
        .apiVersionStringLen = 2,
        .pApiBaseUrl = "https://nekos.best/api/v2",
        .apiBaseUrlLen = 25,
        .pLibVersion = NB_LIB_VERSION,
        .libVersionLen = (sizeof(NB_LIB_VERSION) / sizeof(char)) - 1,
    };

    return &nbApiInfo;
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
    
    pTmpClient->randomState = XORSHIFT32((uint32_t) time(NULL));

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