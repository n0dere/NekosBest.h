#include "nekosbest.h"

#include <stdio.h>

#include "httpclient.h"
#include "version.h"

static const NbApiInfo nbApiInfo = {
    .apiVersion = 2,
    .pApiVersionString = "v2",
    .apiVersionStringLen = 2,
    .pApiBaseUrl = "https://nekos.best/api/v2",
    .apiBaseUrlLen = 25,
    .pLibVersion = NB_LIB_VERSION,
    .libVersionLen = (sizeof(NB_LIB_VERSION) / sizeof(char)) - 1,
};

const NbApiInfo *nbGetApiInfo(void)
{
    return &nbApiInfo;
}

#define safe_puts(str) (puts((str) ? (str) : "NULL"))

int main(void)
{
    NbHttpClient client;
    NbHttpResponse *response;
    char buffer[256] = {0};

    nbHttpClientCreate(&client);
    nbHttpClientApiGet(client, &response, "/search?query=%s&type=2&amount=%d", "Senko", 5);
    safe_puts(response->body.pPtr);
    nbHttpResponseDestroy(response);
    nbHttpClientDestroy(client);

    return 0;
}