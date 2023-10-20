#include "categories.h"

#include "nekosbest.h"

#include <string.h>

#define CATEGORIES_COUNT    (sizeof aCategories / sizeof *aCategories)
#define CATEGORIES_PNG      4

static const char *aCategories[] = {
    /* PNG: */
    "kitsune", "neko", "husbando", "waifu",
    
    /* GIF: */
    "baka", "bite", "blush", "bored", "cry", "cuddle", "dance", "facepalm",
    "feed", "happy", "highfive", "hug", "kiss", "laugh", "pat", "pout",
    "shrug", "slap", "sleep", "smile", "smug", "stare", "think", "thumbsup",
    "tickle", "wave", "wink", "kick", "handhold", "punch", "shoot", "yeet",
    "poke", "nod", "nom", "nope", "handshake", "lurk", "peck", "yawn",
};

const char *nbPickRandomCategory(NbClient client)
{
    if (client == NULL)
        return aCategories[0];
    
    return aCategories[nbClientRandom(client, 0, CATEGORIES_COUNT)];
}

NbImageFormat nbGetCategoryImageFormat(const char *pCategory)
{
    size_t i;

    if (pCategory == NULL)
        return NB_IMAGE_FORMAT_UNKNOWN;

    for (i = 0; i < CATEGORIES_COUNT; ++i) {
        if (strcmp(pCategory, aCategories[i]) == 0)
            break;
    }

    if (i >= CATEGORIES_COUNT)
        return NB_IMAGE_FORMAT_UNKNOWN;

    if (i < CATEGORIES_PNG)
        return NB_IMAGE_FORMAT_PNG;

    return NB_IMAGE_FORMAT_GIF;
}

bool nbValidateCategory(const char *pCategory)
{
    return nbGetCategoryImageFormat(pCategory) != NB_IMAGE_FORMAT_UNKNOWN;
}