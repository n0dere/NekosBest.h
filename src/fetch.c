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

#include <stdint.h>

#include "client.h"
#include "httpclient.h"
#include "response.h"
#include "categories.h"

NB_API NbResponse *nbClientFetch(NbClient client, const char *pCategory,
                                 size_t amount)
{
    NbHttpResponse *pHttpResponse = NULL;
    NbResponse *pResponse = NULL;

    if (client == NULL)
        return NULL;

    if (pCategory == NULL)
        pCategory = nbPickRandomCategory(client);
    else {
        if (nbValidateCategory(pCategory) == false) {
            nbClientSetLastError(client, NB_RESULT_UNKNOWN_CATEGORY);
            return NULL;
        }
    }

    if (amount < 1 || amount > 20) {
        nbClientSetLastError(client, NB_RESULT_AMOUNT_IS_INCORRECT);
        return NULL;
    }

    pHttpResponse = nbClientApiGet(client, "/%s?amount=%u", pCategory,
                                   (uint32_t) amount);

    if (pHttpResponse == NULL)
        return NULL;

    if (pHttpResponse->status != 200) {
        nbClientSetLastError(client, NB_RESULT_BAD_RESPONSE_STATUS_CODE);
        nbHttpResponseDestroy(pHttpResponse);
        return NULL;
    }

    pResponse = nbResponseFromHttpResponse(client, pHttpResponse);

    nbHttpResponseDestroy(pHttpResponse);
    return pResponse;
}

NB_API NbBufferResponse *nbClientFetchFile(NbClient client,
                                           const char *pCategory)
{
    NbResponse *pResponse = nbClientFetch(client, pCategory, 1);
    NbHttpResponse *pHttpResponse = NULL;
    NbBufferResponse *pBufferResponse = NULL;
    NbResult result;

    if (pResponse == NULL)
        return NULL;
    
    result = nbHttpClientGet(client->httpClient, &pHttpResponse,
                             pResponse->pResults[0].pUrl);
    
    if (result != NB_RESULT_OK) {
        nbDestroyResponse(pResponse);
        nbClientSetLastError(client, result);
        return NULL;
    }

    pBufferResponse = nbBufferResponseFromHttpResponse(client, pHttpResponse);

    nbDestroyResponse(pResponse);

    return pBufferResponse;
}