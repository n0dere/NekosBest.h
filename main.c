#include <stdio.h>

#include "nekosbest.h"

#if 0
int main(void)
{
    NbClient pClient;
    NbBufferResponse *pResponse;
    FILE *fp;
    int i;

    if (nbCreateClient(&pClient) != NB_RESULT_OK)
        return -1;
    
    pResponse = nbClientFetchFile(pClient, "yawn");

    if (pResponse == NULL) {
        printf("%d\n", (int) nbClientGetLastError(pClient));
        return -1;
    }
    
    fp = fopen("random_yawn.gif", "wb");

    if (fp == NULL) {
        nbDestroyBufferResponse(pResponse);
        return -1;
    }

    fwrite(pResponse->pBytes, pResponse->byteCount, 1, fp);
    fclose(fp);

    nbDestroyBufferResponse(pResponse);
    return 0;
}
#endif

#if 1
int main(void)
{
    NbClient pClient;
    NbResponse *pResponse;
    int i;
    NbSearchOptions options = {0};

    options.amount = 5;
    options.imageFormat = NB_IMAGE_FORMAT_GIF;

    if (nbCreateClient(&pClient) != NB_RESULT_OK)
        return -1;
    
    pResponse = nbClientSearch(pClient, "Yuru Yuri", &options);

    if (pResponse == NULL) {
        printf("%d\n", (int) nbClientGetLastError(pClient));
        return -1;
    }
    
    for (i = 0; i < pResponse->resultsCount; ++i)
        puts(pResponse->pResults[i].pUrl);

    nbDestroyResponse(pResponse);
    return 0;
}
#endif