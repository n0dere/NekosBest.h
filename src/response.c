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

#include "response.h"

#include <string.h>
#include <stdlib.h>

#include <parson.h>

#include "httpclient.h"
#include "client.h"

#define nbStrDup(pString)                                           \
    ((pString) ? nbStrDupN((pString), strlen(pString)) : NULL)

static char *nbStrDupN(const char *pString, size_t stringSize)
{
    char *pBuffer = NULL;

    if (pString != NULL) {
        pBuffer = calloc(stringSize + 1, sizeof *pBuffer);

        if (pBuffer == NULL)
            return NULL;
        
        strncpy(pBuffer, pString, stringSize);
    }
    
    return pBuffer;
}

NbBufferResponse *nbBufferResponseFromHttpResponse(NbClient client,
                                                   NbHttpResponse *pResponse)
{
    NbBufferResponse *pBufResponse;
    const char *pAnimeName, *pArtistHref, *pArtistName;
    const char *pSourceUrl;

    if (client == NULL || pResponse == NULL) {
        nbClientSetLastError(client, NB_RESULT_INVALID_PARAMETER);
        return NULL;
    }

    pBufResponse = calloc(1, sizeof *pBufResponse);

    if (pBufResponse == NULL) {
        nbClientSetLastError(client, NB_RESULT_NO_MEMORY);
        return NULL;
    }

    pBufResponse->pBytes = calloc(pResponse->body.size,
                                  sizeof *pBufResponse->pBytes);

    if (pBufResponse->pBytes == NULL) {
        free(pBufResponse);
        nbClientSetLastError(client, NB_RESULT_NO_MEMORY);
        return NULL;
    }

    pBufResponse->byteCount = pResponse->body.size;

    memcpy(pBufResponse->pBytes, pResponse->body.pPtr, pResponse->body.size);

    pAnimeName = pResponse->header.pAnimeName;
    pArtistHref = pResponse->header.pArtistHref;
    pArtistName = pResponse->header.pArtistName;
    pSourceUrl = pResponse->header.pSourceUrl;

    pBufResponse->meta.pAnimeName = nbHttpUnescape(pAnimeName);
    pBufResponse->meta.pArtistHref = nbHttpUnescape(pArtistHref);
    pBufResponse->meta.pArtistName = nbHttpUnescape(pArtistName);
    pBufResponse->meta.pSourceUrl = nbHttpUnescape(pSourceUrl);

    return pBufResponse;
}

NbResponse *nbResponseFromHttpResponse(NbClient client,
                                       NbHttpResponse *pHttpResponse)
{
    const char *pAnimeName, *pArtistHref, *pArtistName;
    const char *pSourceUrl, *pUrl;
    NbResponse *pResponse = NULL;
    JSON_Value *pRootValue;
    JSON_Object *pRoot, *pResult;
    JSON_Array *pResults;
    size_t i;

    if (client == NULL || pHttpResponse == NULL) {
        nbClientSetLastError(client, NB_RESULT_INVALID_PARAMETER);
        return NULL;
    }

    pRootValue = json_parse_string(pHttpResponse->body.pPtr);

    if (pRootValue == NULL) {
        nbClientSetLastError(client, NB_RESULT_JSON_PARSE_ERROR);
        return NULL;
    }

    pResponse = calloc(1, sizeof *pResponse);

    if (pResponse == NULL) {
        json_value_free(pRootValue);
        nbClientSetLastError(client, NB_RESULT_NO_MEMORY);
        return NULL;
    }

    pRoot = json_value_get_object(pRootValue);
    pResults = json_object_get_array(pRoot, "results");
    
    if (pResults == NULL) {
        free(pResponse);
        json_value_free(pRootValue);
        nbClientSetLastError(client, NB_RESULT_JSON_PARSE_ERROR);
        return NULL;
    }

    pResponse->resultsCount = json_array_get_count(pResults);

    pResponse->pResults = calloc(pResponse->resultsCount,
                                 sizeof *pResponse->pResults);

    if (pResponse->pResults == NULL) {
        free(pResponse);
        json_value_free(pRootValue);
        nbClientSetLastError(client, NB_RESULT_NO_MEMORY);
        return NULL;
    }

    for (i = 0; i < pResponse->resultsCount; ++i) {
        pResult = json_array_get_object(pResults, i);

        pAnimeName = json_object_get_string(pResult, "anime_name");
        pArtistHref = json_object_get_string(pResult, "artist_href");
        pArtistName = json_object_get_string(pResult, "artist_name");
        pSourceUrl = json_object_get_string(pResult, "source_url");
        pUrl = json_object_get_string(pResult, "url");

        pResponse->pResults[i].pAnimeName = nbStrDup(pAnimeName);
        pResponse->pResults[i].pArtistHref = nbStrDup(pArtistHref);
        pResponse->pResults[i].pArtistName = nbStrDup(pArtistName);
        pResponse->pResults[i].pSourceUrl = nbStrDup(pSourceUrl);
        pResponse->pResults[i].pUrl = nbStrDup(pUrl);
    }

    json_value_free(pRootValue);
    return pResponse;
}

NB_API void nbDestroyResponse(NbResponse *pResponse)
{
    size_t i;

    if (pResponse == NULL)
        return;

    for (i = 0; i < pResponse->resultsCount; ++i) {
        free(pResponse->pResults[i].pAnimeName);
        free(pResponse->pResults[i].pArtistHref);
        free(pResponse->pResults[i].pArtistName);
        free(pResponse->pResults[i].pSourceUrl);
        free(pResponse->pResults[i].pUrl);
    }

    free(pResponse->pResults);
    free(pResponse);
}

NB_API void nbDestroyBufferResponse(NbBufferResponse *pBufferResponse)
{
    if (pBufferResponse == NULL)
        return;

    free(pBufferResponse->meta.pAnimeName);
    free(pBufferResponse->meta.pArtistHref);
    free(pBufferResponse->meta.pArtistName);
    free(pBufferResponse->meta.pSourceUrl);
    free(pBufferResponse->meta.pUrl);

    free(pBufferResponse->pBytes);
    free(pBufferResponse);
}