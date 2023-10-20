#include <stdio.h>

#include "nekosbest.h"

int main(void)
{
    NbClient pClient;
    NbBufferResponse *pResponse;
    FILE *fp;
    int i;

    if (nbCreateClient(&pClient) != NB_RESULT_OK)
        return -1;
    
    pResponse = nbClientFetchFile(pClient, "nekdo");

    if (pResponse == NULL) {
        printf("%d\n", (int) nbClientGetLastError(pClient));
        return -1;
    }
    
    fp = fopen("random_neko.png", "wb");

    if (fp == NULL) {
        nbDestroyBufferResponse(pResponse);
        return -1;
    }

    fwrite(pResponse->pBytes, pResponse->byteCount, 1, fp);
    fclose(fp);

    nbDestroyBufferResponse(pResponse);
    return 0;
}