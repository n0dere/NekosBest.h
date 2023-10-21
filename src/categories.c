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

#include "categories.h"

#include <string.h>

#include "nekosbest.h"
#include "client.h"

#define CATEGORIES_PNG_COUNT (sizeof aCategoriesPNG / sizeof *aCategoriesPNG)
#define CATEGORIES_GIF_COUNT (sizeof aCategoriesGIF / sizeof *aCategoriesGIF)
#define CATEGORIES_TOTAL     (CATEGORIES_GIF_COUNT  + CATEGORIES_PNG_COUNT)

static const char *aCategoriesPNG[] = {
    "kitsune", "neko", "husbando", "waifu",
};

static const char *aCategoriesGIF[] = {
    "baka", "bite", "blush", "bored", "cry", "cuddle", "dance", "facepalm",
    "feed", "happy", "highfive", "hug", "kiss", "laugh", "pat", "pout",
    "shrug", "slap", "sleep", "smile", "smug", "stare", "think", "thumbsup",
    "tickle", "wave", "wink", "kick", "handhold", "punch", "shoot", "yeet",
    "poke", "nod", "nom", "nope", "handshake", "lurk", "peck", "yawn",
};

const char *nbPickRandomCategory(NbClient client)
{
    size_t n;

    if (client != NULL) {
        n = nbClientRandom(client, 0, CATEGORIES_TOTAL);
        
        if (n < CATEGORIES_PNG_COUNT)
            return aCategoriesPNG[n];
        else
            return aCategoriesGIF[n - CATEGORIES_PNG_COUNT];
    }

    return "NULL";
}

NbImageFormat nbGetCategoryImageFormat(const char *pCategory)
{
    size_t i;

    if (pCategory == NULL)
        return NB_IMAGE_FORMAT_UNKNOWN;

    for (i = 0; i < CATEGORIES_PNG_COUNT; ++i) {
        if (strcmp(pCategory, aCategoriesPNG[i]) == 0)
            return NB_IMAGE_FORMAT_PNG;
    }

    for (i = 0; i < CATEGORIES_GIF_COUNT; ++i) {
        if (strcmp(pCategory, aCategoriesGIF[i]) == 0)
            return NB_IMAGE_FORMAT_GIF;
    }
    
    return NB_IMAGE_FORMAT_UNKNOWN;
}

bool nbValidateCategory(const char *pCategory)
{
    return nbGetCategoryImageFormat(pCategory) != NB_IMAGE_FORMAT_UNKNOWN;
}