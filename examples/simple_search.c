#include <stdio.h>

#include "nekosbest.h"

int main(void)
{
    NbClient client;
    NbResponse *pYuruYuri = NULL;
    int i;

    NbSearchOptions searchOptions = {
        .amount = 5,
        .imageFormat = NB_IMAGE_FORMAT_GIF,
    };
    
    if (nbCreateClient(&client) != NB_RESULT_OK)
        return -1;
    
    pYuruYuri = nbClientSearch(client, "Yuru Yuri", &searchOptions);

    if (pYuruYuri != NULL) {
        
        for (i = 0; i < pYuruYuri->resultsCount; ++i) {
            printf("pYuruYuri->pResults[%d]\n", i);
            printf("├─ Anime Name: %s\n", pYuruYuri->pResults[i].pAnimeName);
            printf("└─ Url: %s\n", pYuruYuri->pResults[i].pUrl);
        }

        nbDestroyResponse(pYuruYuri);
    }

    nbDestroyClient(client);
    return 0;
}