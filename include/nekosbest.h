/*   __  _      
    |  \| |     Copyright (c) 2023 n0dere
    | . ` |     This software is licensed under the MIT License.
    |_|\__|     https://github.com/n0dere/NekosBest.h
*/

#ifndef _NEKOSBEST_H_
#define _NEKOSBEST_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _NbResult NbResult;
typedef struct _NbApiInfo NbApiInfo;
typedef struct _NbClient *NbClient;
typedef struct _NbIndividualResponse NbIndividualResponse;
typedef struct _NbResponse NbResponse;
typedef struct _NbBufferResponse NbBufferResponse;
typedef struct _NbSearchOptions NbSearchOptions;

enum _NbResult {
    NB_RESULT_OK,
    NB_RESULT_NO_MEMORY,
    NB_RESULT_INVALID_PARAMETER,
    NB_RESULT_HTTPCLIENT_INITIALIZATION_ERROR,
    NB_RESULT_HTTPCLIENT_INTERNAL_ERROR,
};

struct _NbApiInfo {
    size_t                  apiVersion;
    const char*             pApiVersionString;
    size_t                  apiVersionStringLen;
    const char*             pApiBaseUrl;
    size_t                  apiBaseUrlLen;
    const char*             pLibVersion;
    size_t                  libVersionLen;
};

struct _NbIndividualResponse {
    char*                   pAnimeName;
    char*                   pArtistHref;
    char*                   pArtistName;
    char*                   pSourceUrl;
    char*                   pUrl;
};

struct _NbResponse {
    NbIndividualResponse*   pResults;
    size_t                  resultsCount;
};

struct _NbBufferResponse {
    NbIndividualResponse    info;
    char*                   pData;
    size_t                  dataSize;
};

struct _NbSearchOptions {
    size_t                  imageFormat;
    size_t                  amount;
    const char*             pCategory;
};

const NbApiInfo *nbGetApiInfo(void);

NbResult nbCreateClient(
    NbClient*               pClient
);

NbResponse *nbClientFetch(
    NbClient*               pClient,
    const char*             pCategory,
    size_t                  amount
);

NbResponse *nbClientFetchRandom(
    NbClient*               pClient,
    size_t                  amount
);

NbResponse *nbClientSearch(
    NbClient*               pClient,
    const NbSearchOptions*  pOptions
);

NbBufferResponse *nbClientFetchFile(
    NbClient*               pClient
);

void nbDestroyClient(
    NbClient*               pClient
);

void nbDestroyResponse(
    NbResponse*             pResponse
);

void nbDestroyBufferResponse(
    NbBufferResponse*       pResponse
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _NEKOSBEST_H_ */