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

#include "httpclient.h"

#include <stdlib.h>
#include <string.h>
#include <string.h>

size_t nbHttpResponseAppendBody(char *pContents, size_t size, size_t nmemb,
                                NbHttpResponse *pResponse)
{
    size_t realSize = size * nmemb;
    size_t newBodySize = 0;

    if (pResponse == NULL || realSize == 0)
        return 0;

    newBodySize = pResponse->body.size + realSize + 1;
    pResponse->body.pPtr = realloc(pResponse->body.pPtr, newBodySize);

    if (pResponse->body.pPtr == NULL)
        return 0;
        
    memmove(pResponse->body.pPtr + pResponse->body.size, pContents, realSize);
    
    pResponse->body.pPtr[newBodySize - 1] = '\0';
    pResponse->body.size += realSize;

    return realSize;
}

void nbHttpResponseHeaders(void *pPtr, NbHttpResponse *pResponse,
        void (*pParseFn)(void *pPtr, const char *pKey, char **ppValOutBuffer))
{
    pParseFn(pPtr, "x-rate-limit-remaining",
             &pResponse->header.pXRateLimitRemaining);

    pParseFn(pPtr, "x-rate-limit-reset", &pResponse->header.pXRateLimitReset);

    pParseFn(pPtr, "artist_name", &pResponse->header.pArtistName);
    pParseFn(pPtr, "artist_href", &pResponse->header.pArtistHref);
    pParseFn(pPtr, "anime_name", &pResponse->header.pAnimeName);
    pParseFn(pPtr, "source_url", &pResponse->header.pSourceUrl);
}

void nbHttpResponseDestroy(NbHttpResponse *pResponse)
{
    if (pResponse != NULL) {
        free(pResponse->body.pPtr);
        free(pResponse->header.pXRateLimitRemaining);
        free(pResponse->header.pXRateLimitReset);
        free(pResponse);
    }
}