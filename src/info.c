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

#include "nekosbest.h"

#include "version.h"

NB_API const NbApiInfo *nbGetApiInfo(void)
{
    static const NbApiInfo nbApiInfo = {
        .apiVersion = 2,
        .pApiVersionString = "v2",
        .apiVersionStringLen = 2,
        .pApiBaseUrl = "https://nekos.best/api/v2",
        .apiBaseUrlLen = 25,
        .pLibVersion = NB_LIB_VERSION,
        .libVersionLen = (sizeof(NB_LIB_VERSION) / sizeof(char)) - 1,
    };

    return &nbApiInfo;
}