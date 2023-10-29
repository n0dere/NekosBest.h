#include "nekosbest.h"

#include "response.h"
#include "httpclient.h"
#include "client.h"

NB_API NbBufferResponse *nbClientDownloadResponse(NbClient client,
        const NbIndividualResponse *pResponse)
{
    NbHttpResponse *pHttpResponse = NULL;
    NbResult result;

    if (client == NULL)
        return NULL;

    if (pResponse == NULL) {
        nbClientSetLastError(client, NB_RESULT_INVALID_PARAMETER);
        return NULL;
    }
    
    result = nbHttpClientGet(client->httpClient, &pHttpResponse,
                             pResponse->pUrl);
    
    if (result != NB_RESULT_OK) {
        nbClientSetLastError(client, result);
        return NULL;
    }

    return nbBufferResponseFromHttpResponse(client, pHttpResponse);
}