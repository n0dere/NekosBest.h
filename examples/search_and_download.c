#include <stdio.h>

#include "nekosbest.h"

#define ANIME_NAME "Kobayashi-san Chi no Maid Dragon"
#define FILE_NAME "Miss Kobayashi's Dragon Maid.gif"

static NbSearchOptions searchOptions = {
    .amount = 1,
    .imageFormat = NB_IMAGE_FORMAT_GIF,
};

int main(void)
{
    NbClient client;
    NbResponse *pDragonMaid = NULL;
    NbBufferResponse *pBufferResp = NULL;
    FILE *fp = NULL;
    
    if (nbCreateClient(&client) != NB_RESULT_OK) {
        printf("NekosBest.h error: %d\n", (int) nbClientGetLastError(client));
        goto error;
    }
    
    pDragonMaid = nbClientSearch(client, ANIME_NAME, &searchOptions);

    if (pDragonMaid == NULL) {
        printf("NekosBest.h error: %d\n", (int) nbClientGetLastError(client));
        goto error;
    }

    pBufferResp = nbClientDownloadResponse(client, &pDragonMaid->pResults[0]);

    if (pBufferResp == NULL) {
        printf("NekosBest.h error: %d\n", (int) nbClientGetLastError(client));
        goto error;
    }

    fp = fopen(FILE_NAME, "wb");

    if (fp == NULL) {
        puts("fopen error");
        goto error;
    }

    if (fwrite(pBufferResp->pBytes, pBufferResp->byteCount, 1, fp) <= 0) {
        puts("fwrite error");
        goto error;
    }
    else
        printf("\"%s\" saved...", FILE_NAME);

error:
    fclose(fp);
    nbDestroyBufferResponse(pBufferResp);
    nbDestroyResponse(pDragonMaid);
    nbDestroyClient(client);
    return 0;
}