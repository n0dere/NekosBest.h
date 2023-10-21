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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

extern size_t nbHttpResponseAppendBody(char *pContents, size_t size,
        size_t nmemb, NbHttpResponse *pResponse);

static void parseHeaderValue(
    const char *pContents,
    const char *pHeaderKey,
    char **ppValueDest
) {
    char format[512] = {0};

    if (pContents == NULL || pHeaderKey == NULL || ppValueDest == NULL)
        return;
    
    if (strstr(pContents, pHeaderKey) == NULL)
        return;
    
    /* "x-rate-limit-remaining: %s\r\n" */
    snprintf(format, 512, "%s: %%s\r\n", pHeaderKey);

    *ppValueDest = calloc(strlen(pContents), sizeof(char));
    
    if (*ppValueDest == NULL)
        return;

    sscanf(pContents, format, *ppValueDest);
}

static size_t curlHttpHeaderFunction(
    char *pContents,
    size_t size,
    size_t nmemb,
    NbHttpResponse *pResponse
) {
    parseHeaderValue(pContents, "x-rate-limit-remaining",
        &pResponse->header.pXRateLimitRemaining
    );

    parseHeaderValue(pContents, "x-rate-limit-reset",
        &pResponse->header.pXRateLimitReset
    );

    parseHeaderValue(pContents, "artist_name",
        &pResponse->header.pArtistName
    );
    
    parseHeaderValue(pContents, "artist_href",
        &pResponse->header.pArtistHref
    );
    
    parseHeaderValue(pContents, "anime_name",
        &pResponse->header.pAnimeName
    );
    
    parseHeaderValue(pContents, "source_url",
        &pResponse->header.pSourceUrl
    );

    return size * nmemb;
}

static void curlHttpSetOptions(
    CURL *pCurl,
    const char *pUrl,
    NbHttpResponse *pResponse
) {
    curl_easy_setopt(pCurl, CURLOPT_USERAGENT, NB_HTTPCLIENT_USERAGENT);

    curl_easy_setopt(pCurl, CURLOPT_URL, pUrl);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pResponse);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, nbHttpResponseAppendBody);
    
    curl_easy_setopt(pCurl, CURLOPT_HEADERDATA, pResponse);
    curl_easy_setopt(pCurl, CURLOPT_HEADERFUNCTION, curlHttpHeaderFunction);
    
    curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);
}

NbResult nbHttpClientCreate(
    NbHttpClient *pHttpClient
) {
    CURL* pCurlSession = curl_easy_init();

    if (pCurlSession == NULL)
        return NB_RESULT_HTTPCLIENT_INITIALIZATION_ERROR;
    
    *pHttpClient = (NbHttpClient) pCurlSession;

    return NB_RESULT_OK;
}

NbResult nbHttpClientGet(
    NbHttpClient httpClient, 
    NbHttpResponse **ppResponse, 
    const char *pUrl
) {
    CURL *pCurlSession = (CURL*) httpClient;
    NbResult result = NB_RESULT_OK;
    NbHttpResponse *pResponse = NULL;
    CURLcode code;

    if (ppResponse == NULL || pUrl == NULL || pCurlSession == NULL)
        return NB_RESULT_INVALID_PARAMETER;

    pResponse = calloc(1, sizeof *pResponse);

    if (pResponse == NULL)
        return NB_RESULT_NO_MEMORY;

    curlHttpSetOptions(pCurlSession, pUrl, pResponse);
    
    if (curl_easy_perform(pCurlSession) != CURLE_OK) {
        free(pResponse);
        return NB_RESULT_HTTPCLIENT_INTERNAL_ERROR;
    }

    curl_easy_getinfo(
        pCurlSession, CURLINFO_RESPONSE_CODE, &pResponse->status
    );

    *ppResponse = pResponse;

    return NB_RESULT_OK;
}

void nbHttpClientDestroy(
    NbHttpClient pHttpClient
) {
    if (pHttpClient != NULL)
        curl_easy_cleanup((CURL*) pHttpClient);
}

char *nbHttpEscape(
    const char *pString
) {
    char *pCurlString, *pEscaped = NULL;
    size_t size;

    if (pString != NULL) {
        pCurlString = curl_easy_escape(NULL, pString, 0);
        size = strlen(pCurlString);
        pEscaped = calloc(size + 1, sizeof *pEscaped);
        strncpy(pEscaped, pCurlString, size);
        curl_free(pCurlString);
    }

    return pEscaped;
}

char *nbHttpUnescape(
    const char *pString
) {
    char *pCurlString, *pUnescaped = NULL;
    int decodeLen;

    if (pString != NULL) {
        pCurlString = curl_easy_unescape(NULL, pString, 0, &decodeLen);
        pUnescaped = calloc(decodeLen + 1, sizeof *pUnescaped);
        strncpy(pUnescaped, pCurlString, decodeLen);
        curl_free(pCurlString);
    }

    return pUnescaped;
}