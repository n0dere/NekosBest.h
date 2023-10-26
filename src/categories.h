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

#ifndef _CATEGORIES_H_
#define _CATEGORIES_H_

#include "nekosbest.h"

#include <stdbool.h>

/* Picks a random category from the list of available categories from the API.
 *
 * Args:    client - a NbClient object for nbClientRandom.
 * 
 * Returns: a pointer to a null-terminated string that contains the name of the
 *          random category.
 */
const char *nbPickRandomCategory(NbClient client);

/* Gets the image format(NbImageFormat) of a given category.
 * 
 * Args:    pCategory - a pointer to a null-terminated string that specifies
 *          the category
 * 
 * Returns: the image format (NbImageFormat) that corresponds to the given
 *          category. For example, if the category is "neko", the image
 *          format is NB_IMAGE_FORMAT_PNG.
 */
NbImageFormat nbGetCategoryImageFormat(const char *pCategory);

/* Validates a given category.
 *
 * This function checks if the given category is valid and supported by the API.
 *
 * Args:    pCategory - a pointer to a null-terminated string that specifies
 *          the category
 * 
 * Returns: true if the category is valid and supported by the API, or false
 *          otherwise.
 */
bool nbValidateCategory(const char *pCategory);

#endif /* _CATEGORIES_H_ */