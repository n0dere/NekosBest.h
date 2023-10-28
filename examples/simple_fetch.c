#include <stdio.h>

#include "nekosbest.h"

int main(void)
{
    NbResponse *pNeko = NULL;
    NbClient client;

    if (nbCreateClient(&client) != NB_RESULT_OK)
        return -1;
    
    if ((pNeko = nbClientFetch(client, "neko", 1)) != NULL) {
        printf("Artist Href:\t%s\n", pNeko->pResults[0].pArtistHref);
        printf("Artist Name:\t%s\n", pNeko->pResults[0].pArtistName);
        printf("Source Url: \t%s\n", pNeko->pResults[0].pSourceUrl);
        printf("Url:        \t%s\n", pNeko->pResults[0].pUrl);
        nbDestroyResponse(pNeko);
    }

    nbDestroyClient(client);
    return 0;
}