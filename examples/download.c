#include <stdio.h>

#include "nekosbest.h"

#define CATEGORY    "happy"
#define FILE_NAME   "random_"CATEGORY

int main(void)
{
    NbClient client;
    NbBufferResponse *pResponse = NULL;
    FILE *fp;
    
    if (nbCreateClient(&client) != NB_RESULT_OK)
        return -1;
        
    if ((pResponse = nbClientFetchFile(client, CATEGORY)) == NULL) {
        nbDestroyClient(client);
        return -1;
    }

    if (pResponse->imageFormat == NB_IMAGE_FORMAT_GIF)
        fp = fopen(FILE_NAME".gif", "wb");
    else if (pResponse->imageFormat == NB_IMAGE_FORMAT_PNG)
        fp = fopen(FILE_NAME".png", "wb");

    if (fp == NULL) {
        nbDestroyBufferResponse(pResponse);
        nbDestroyClient(client);
        return -1;
    }

    if (fwrite(pResponse->pBytes, pResponse->byteCount, 1, fp) > 0) {
        printf("\"%s.%s\" saved...\n", FILE_NAME,
                (pResponse->imageFormat == NB_IMAGE_FORMAT_GIF)
                ? "gif" : "png");
    }

    fclose(fp);
    nbDestroyBufferResponse(pResponse);
    nbDestroyClient(client);
    return 0;
}