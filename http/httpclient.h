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

#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include <stddef.h>

#include "nekosbest.h"

#include "version.h"

#define NB_HTTPCLIENT_USERAGENT "nekosbest.h "NB_LIB_VERSION

typedef void *NbHttpClient;
typedef struct _HttpResponse NbHttpResponse;

struct _HttpResponse {
    long status;
    struct {
        size_t size;
        char *pPtr;
    } body;
    struct {
        char *pXRateLimitRemaining;
        char *pXRateLimitReset;
        char *pAnimeName;
        char *pArtistHref;
        char *pArtistName;
        char *pSourceUrl;
    } header;
};

NbResult nbHttpClientCreate(NbHttpClient *pHttpClient);

NbResult nbHttpClientGet(NbHttpClient httpClient, NbHttpResponse **ppResponse, 
                         const char *pUrl);

void nbHttpResponseDestroy(NbHttpResponse *pResponse);

void nbHttpClientDestroy(NbHttpClient pHttpClient);

char *nbHttpEscape(const char *pString);

char *nbHttpUnescape(const char *pString);

#endif /* _HTTPCLIENT_H_ */