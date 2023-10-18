/*   __  _      
    |  \| |     Copyright (c) 2023 n0dere
    | . ` |     This software is licensed under the MIT License.
    |_|\__|     https://github.com/n0dere/NekosBest.h
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
    long                    status;
    struct {
        size_t              size;
        char*               pPtr;
    } body;
    struct {
        char*               pXRateLimitRemaining;
        char*               pXRateLimitReset;
    } header;
};

NbResult nbHttpClientCreate(
    NbHttpClient*           pHttpClient
);

NbResult nbHttpClientGet(
    NbHttpClient            httpClient, 
    NbHttpResponse**        ppResponse, 
    const char*             pUrl
);

NbResult nbHttpClientApiGet(
    NbHttpClient            httpClient, 
    NbHttpResponse**        ppResponse, 
    const char*             pQueryFormat,
    ...
);

void nbHttpResponseDestroy(
    NbHttpResponse*         pResponse
);

void nbHttpClientDestroy(
    NbHttpClient            pHttpClient
);

#endif /* _HTTPCLIENT_H_ */