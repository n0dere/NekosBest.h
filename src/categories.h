/*
    Copyright (c) 2023 n0dere
    This software is licensed under the MIT License.
     _   _      _             ____            _     _     
    | \ | | ___| | _____  ___| __ )  ___  ___| |_  | |__  
    |  \| |/ _ \ |/ / _ \/ __|  _ \ / _ \/ __| __| | '_ \ 
    | |\  |  __/   < (_) \__ \ |_) |  __/\__ \ |_ _| | | |
    |_| \_|\___|_|\_\___/|___/____/ \___||___/\__(_)_| |_|    

    https://github.com/n0dere/NekosBest.h
*/

#ifndef _CATEGORIES_H_
#define _CATEGORIES_H_

#include "nekosbest.h"

#include <stdbool.h>

const char *nbPickRandomCategory(NbClient client);

NbImageFormat nbGetCategoryImageFormat(const char *pCategory);

bool nbValidateCategory(const char *pCategory);

#endif /* _CATEGORIES_H_ */