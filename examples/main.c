#include <stdio.h>

#include "nekosbest.h"

#if 1

#define CATEGORY_NAME "nope"

#define safe_str(str) ((str) ? (str) : "NULL")

int main(void)
{
    NbClient pClient;
    NbBufferResponse *pResponse;
    FILE *fp;

    if (nbCreateClient(&pClient) != NB_RESULT_OK)
        return -1;
    
    pResponse = nbClientFetchFile(pClient, CATEGORY_NAME);

    if (pResponse == NULL) {
        printf("%d\n", (int) nbClientGetLastError(pClient));
        return -1;
    }

    printf("%s\n", nbGetApiInfo()->pLibVersion);

    printf("Anime Name: %s\n", safe_str(pResponse->meta.pAnimeName));
    printf("Artist Href: %s\n", safe_str(pResponse->meta.pArtistHref));
    printf("Artist Name: %s\n", safe_str(pResponse->meta.pArtistName));
    printf("Source Url: %s\n", safe_str(pResponse->meta.pSourceUrl));

    if (pResponse->imageFormat == NB_IMAGE_FORMAT_PNG)
        fp = fopen("random_"CATEGORY_NAME".png", "wb");
    else if (pResponse->imageFormat == NB_IMAGE_FORMAT_GIF)
        fp = fopen("random_"CATEGORY_NAME".gif", "wb");
    else
        fp = fopen("random_"CATEGORY_NAME".unknown", "wb");
    
    if (fp == NULL) {
        nbDestroyBufferResponse(pResponse);
        return -1;
    }
    fwrite(pResponse->pBytes, pResponse->byteCount, 1, fp);
    fclose(fp);

    nbDestroyBufferResponse(pResponse);
    return 0;
}
#else

static void printResponse(const NbResponse *pResponse)
{
    int i;

    if (pResponse != NULL) {
        for (i = 0; i < pResponse->resultsCount; ++i)
            puts(pResponse->pResults[i].pUrl);
    }
}

int main(void)
{
    NbClient client;
    NbResponse *pResponse;
    NbSearchOptions options = {0};
    int i;

    options.amount = 1;
    options.imageFormat = NB_IMAGE_FORMAT_GIF;

    if (nbCreateClient(&client) != NB_RESULT_OK)
        return -1;
    
    for (i = 0; i < 100; ++i) {
        pResponse = nbClientSearch(client, "Yuru Yuri", &options);

        if (pResponse != NULL)
            printResponse(pResponse);
        else
            printf("%d\n", (int) nbClientGetLastError(client));

        puts("----------");
    }

    pResponse = nbClientFetch(client, "waifu", 10);



    printResponse(pResponse);
    nbDestroyResponse(pResponse);
    return 0;
}
#endif