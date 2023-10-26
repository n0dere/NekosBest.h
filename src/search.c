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

#include "nekosbest.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "client.h"
#include "response.h"
#include "categories.h"

static const NbSearchOptions defaultOptions = {
    .amount = 1,
    .imageFormat = NB_IMAGE_FORMAT_RANDOM,
    .pCategory = NULL,
};

static time_t parseTimeStringISO8601(const char *pTimeString)
{
    struct tm time;

    if (pTimeString == NULL)
        return 0;

    /* 2023-10-21T20:16:30.0000000Z */
    sscanf(pTimeString, "%d-%d-%dT%d:%d:%d", &time.tm_year, &time.tm_mon,
           &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);

    time.tm_year -= 1900; /* offsets */
    time.tm_mon -= 1;

    return mktime(&time);
}

static bool isRateLimited(NbClient client)
{
    time_t curTime = time(NULL);
    time_t gmCurTime = mktime(gmtime(&curTime));
    time_t resetsIn = client->ratelimit.resetsIn;
    
    return (gmCurTime < resetsIn) && (client->ratelimit.remaining == 0);
}

static void updateRateLimit(NbClient client, const NbHttpResponse *pResponse)
{
    const char *pReset, *pRemaining;

    if (pResponse == NULL || client == NULL)
        return;
    
    pReset = pResponse->header.pXRateLimitReset;
    pRemaining = pResponse->header.pXRateLimitRemaining;
    
    if (pReset != NULL)
        client->ratelimit.resetsIn = parseTimeStringISO8601(pReset);

    if (pRemaining != NULL)
        client->ratelimit.remaining = atoi(pRemaining);
}

NB_API NbResponse *nbClientSearch(NbClient client, const char *pQuery,
                                  const NbSearchOptions *pOptions)
{
    NbHttpResponse *pHttpResponse = NULL;
    NbImageFormat imageFormat = NB_IMAGE_FORMAT_UNKNOWN;
    char *pEscapedQuery = NULL;
    NbResponse *pResponse = NULL;
    size_t queryLen;

    if (client == NULL)
        return NULL;
    
    if (pQuery == NULL) {
        nbClientSetLastError(client, NB_RESULT_INVALID_PARAMETER);
        return NULL;
    }

    if (isRateLimited(client) == true) {
        nbClientSetLastError(client, NB_RESULT_SEARCH_RATE_LIMITED);
        return NULL;
    }

    if (pOptions == NULL)
        pOptions = &defaultOptions;

    queryLen = strlen(pQuery);

    if (queryLen < 3 || queryLen > 150) {
        nbClientSetLastError(client, NB_RESULT_QUERY_LEN_IS_INCORRECT);
        return NULL;
    }

    if (pOptions->amount < 1 || pOptions->amount > 20) {
        nbClientSetLastError(client, NB_RESULT_AMOUNT_IS_INCORRECT);
        return NULL;
    }
    
    pEscapedQuery = nbHttpEscape(pQuery);

    if (pEscapedQuery == NULL) {
        nbClientSetLastError(client, NB_RESULT_NO_MEMORY);
        return NULL;
    }

    if (pOptions->imageFormat == NB_IMAGE_FORMAT_RANDOM)
        /* NB_IMAGE_FORMAT_PNG == 1 / NB_IMAGE_FORMAT_GIF == 2 */
        imageFormat = nbClientRandom(client, 1, 2);
    else
        imageFormat = pOptions->imageFormat;

    if (pOptions->pCategory == NULL) {
        pHttpResponse = nbClientApiGet(client,
            "/search?query=%s&type=%u&amount=%u", pEscapedQuery,
            (uint32_t) imageFormat, (uint32_t) pOptions->amount
        );
    }
    else {
        imageFormat = nbGetCategoryImageFormat(pOptions->pCategory);

        if (imageFormat == NB_IMAGE_FORMAT_UNKNOWN) {
            nbClientSetLastError(client, NB_RESULT_UNKNOWN_CATEGORY);
            free(pEscapedQuery);
            return NULL;
        }

        pHttpResponse = nbClientApiGet(client,
            "/search?query=%s&type=%u&amount=%u&category=%s",
            pEscapedQuery, (uint32_t) imageFormat,
            (uint32_t) pOptions->amount, pOptions->pCategory
        );
    }

    free(pEscapedQuery);

    if (pHttpResponse == NULL)
        return NULL;

    if (pHttpResponse->status != 200) {
        nbClientSetLastError(client, NB_RESULT_BAD_RESPONSE_STATUS_CODE);
        nbHttpResponseDestroy(pHttpResponse);
        return NULL;
    }

    updateRateLimit(client, pHttpResponse);

    pResponse = nbResponseFromHttpResponse(client, pHttpResponse);

    nbHttpResponseDestroy(pHttpResponse);

    return pResponse;
}