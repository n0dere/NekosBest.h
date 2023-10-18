/*   __  _      
    |  \| |     Copyright (c) 2023 n0dere
    | . ` |     This software is licensed under the MIT License.
    |_|\__|     https://github.com/n0dere/NekosBest.h
*/

#include "httpclient.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define URL_MAX_SIZE                        1024

size_t _nbHttpClientAppendBody(
    char*                   pContents,
    size_t                  size,
    size_t                  nmemb,
    NbHttpResponse*         pResponse
) {
    char* pNewBody = NULL;
    size_t realSize = size * nmemb;
    size_t newBodySize = 0;

    if (pResponse == NULL || realSize <= 0)
        return 0;

    newBodySize = pResponse->body.size + realSize + 1;
    pNewBody = realloc(pResponse->body.pPtr, newBodySize);

    if (pNewBody == NULL)
        return 0;

    pResponse->body.pPtr = pNewBody;
        
    memmove(
        pResponse->body.pPtr + pResponse->body.size, pContents, realSize
    );
    
    pResponse->body.pPtr[newBodySize - 1] = '\0';
    pResponse->body.size += realSize;

    return realSize;
}

NbResult nbHttpClientApiGet(
    NbHttpClient            httpClient, 
    NbHttpResponse**        ppResponse, 
    const char*             pQueryFormat,
    ...
) {
    char url[URL_MAX_SIZE] = {0};
    NbHttpResponse *pResponse = NULL;
    size_t urlLen;
    va_list args;

    if (ppResponse == NULL || pQueryFormat == NULL)
        return NB_RESULT_INVALID_PARAMETER;
    
    urlLen = URL_MAX_SIZE - nbGetApiInfo()->apiBaseUrlLen - 1;
    
    strncpy(url, nbGetApiInfo()->pApiBaseUrl, urlLen);

    va_start(args, pQueryFormat);

    vsnprintf(
        url + nbGetApiInfo()->apiBaseUrlLen, urlLen, pQueryFormat, args
    );

    va_end(args);

    return nbHttpClientGet(httpClient, ppResponse, url);
}

void nbHttpResponseDestroy(
    NbHttpResponse*         pResponse
) {
    if (pResponse == NULL) {
        free(pResponse->body.pPtr);
        free(pResponse->header.pXRateLimitRemaining);
        free(pResponse->header.pXRateLimitReset);
        free(pResponse);
    }
}