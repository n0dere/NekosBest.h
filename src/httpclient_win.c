#include "httpclient.h"

#include <windows.h>
#include <winhttp.h>

extern size_t _nbHttpClientAppendBody(
    char*                   pContents,
    size_t                  size,
    size_t                  nmemb,
    NbHttpResponse*         pResponse
);

static void httpClientHeader(
    HINTERNET               request,
    LPCWSTR                 wideKey,
    char**                  ppDestBufferPtr
) {
    DWORD size = 0;
    LPVOID wideBuffer = NULL;
    BOOL results = FALSE;

    WinHttpQueryHeaders(request, WINHTTP_QUERY_CUSTOM, wideKey, NULL, &size,
        WINHTTP_NO_HEADER_INDEX
    );
    
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        wideBuffer = calloc(size, 1 /* byte */);

        results = WinHttpQueryHeaders(request, WINHTTP_QUERY_CUSTOM, wideKey,
            wideBuffer, &size, WINHTTP_NO_HEADER_INDEX
        );
        
        size /= sizeof(WCHAR);
    }

    if (results != FALSE) {
        *ppDestBufferPtr = calloc(size + 1, sizeof(char));

        if (*ppDestBufferPtr != NULL) {
            WideCharToMultiByte(CP_UTF8, 0, wideBuffer, -1, *ppDestBufferPtr,
                (int) size, NULL, NULL
            );
        }
    }

    free(wideBuffer);
}

static NbResult httpParseResponse(
    NbHttpResponse**        ppResponse,
    HINTERNET               request
) {
    NbHttpResponse *pResponse = calloc(1, sizeof *pResponse);
    DWORD downloaded, size;
    char *pBuffer;

    if (pResponse == NULL)
        return NB_RESULT_NO_MEMORY;

    size = sizeof size;

    WinHttpQueryHeaders(request,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX, &pResponse->status, &size,
        WINHTTP_NO_HEADER_INDEX
    );

    do {
        size = 0;
        WinHttpQueryDataAvailable(request, &size);

        if ((pBuffer = calloc(size + 1, sizeof *pBuffer)) == NULL)
            break;
        
        WinHttpReadData(request, pBuffer, size, &downloaded);
        _nbHttpClientAppendBody(pBuffer, size, 1, pResponse);
        free(pBuffer);
    }
    while (size > 0);

    httpClientHeader(request, L"x-rate-limit-remaining",
        &pResponse->header.pXRateLimitRemaining
    );

    httpClientHeader(request, L"x-rate-limit-reset",
        &pResponse->header.pXRateLimitReset
    );

    *ppResponse = pResponse;

    return NB_RESULT_OK;
}

static NbResult httpClientGetPerform(
    HINTERNET               httpSession, 
    NbHttpResponse**        ppResponse,
    LPCWSTR                 hostname,
    LPCWSTR                 urlPath
) {
    NbResult result = NB_RESULT_OK;
    HINTERNET connect, request;
    BOOL winResult;

    connect = WinHttpConnect(httpSession, hostname, INTERNET_DEFAULT_PORT, 0);

    if (connect == NULL)
        return NB_RESULT_HTTPCLIENT_INTERNAL_ERROR;

    request = WinHttpOpenRequest(connect, L"GET", urlPath, NULL,
        WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE
    );
    
    if (request == NULL) {
        WinHttpCloseHandle(connect);
        return NB_RESULT_HTTPCLIENT_INTERNAL_ERROR;
    }

    winResult = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0
    );

    if (winResult == FALSE) {
        WinHttpCloseHandle(connect);
        WinHttpCloseHandle(request);
        return NB_RESULT_HTTPCLIENT_INTERNAL_ERROR;
    }

    if (WinHttpReceiveResponse(request, NULL) == TRUE)
        result = httpParseResponse(ppResponse, request);
    else
        result = NB_RESULT_HTTPCLIENT_INTERNAL_ERROR;

    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(request);
    return result;
}

NbResult nbHttpClientCreate(
    NbHttpClient*           pHttpClient
) {
    HINTERNET httpSession;

    if (pHttpClient == NULL)
        return NB_RESULT_INVALID_PARAMETER;

    httpSession = WinHttpOpen(L""NB_HTTPCLIENT_USERAGENT,
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0
    );

    if (httpSession == NULL) {
        if (GetLastError() == ERROR_NOT_ENOUGH_MEMORY)
            return NB_RESULT_NO_MEMORY;

        return NB_RESULT_HTTPCLIENT_INITIALIZATION_ERROR;
    }

    *(pHttpClient) = (NbHttpClient) httpSession;

    return NB_RESULT_OK;
}

NbResult nbHttpClientGet(
    NbHttpClient            httpClient, 
    NbHttpResponse**        ppResponse, 
    const char*             pUrl
) {
    URL_COMPONENTS urlComp = {0};
    HINTERNET httpSession = (HINTERNET) httpClient;
    NbResult result = NB_RESULT_OK;
    BOOL crackRes = FALSE;
    size_t urlLen = 0;
    LPWSTR urlWide;
    LPWSTR hostname;

    if (ppResponse == NULL || pUrl == NULL || httpSession == NULL)
        return NB_RESULT_INVALID_PARAMETER;
    
    urlLen = strnlen(pUrl, 2048);
    urlWide = calloc(urlLen + 1, sizeof *urlWide);

    if (urlWide == NULL) {
        return NB_RESULT_NO_MEMORY;
    }

    MultiByteToWideChar(CP_UTF8, 0, pUrl, -1, urlWide, (int) urlLen);

    urlComp.dwStructSize = sizeof urlComp;
    urlComp.dwHostNameLength = (DWORD) -1;
    urlComp.dwUrlPathLength = (DWORD) -1;
    
    crackRes = WinHttpCrackUrl((LPCWSTR) urlWide, (DWORD) urlLen, 0, &urlComp);

    if (crackRes == FALSE) {
        free(urlWide);
        return NB_RESULT_HTTPCLIENT_INTERNAL_ERROR;
    }

    hostname = calloc(urlComp.dwHostNameLength + 1, sizeof *hostname);

    if (hostname == NULL) {
        free(urlWide);
        return NB_RESULT_NO_MEMORY;
    }

    wcsncpy(hostname, urlComp.lpszHostName, urlComp.dwHostNameLength);

    result = httpClientGetPerform(httpSession, ppResponse, hostname,
        urlComp.lpszUrlPath
    );

    free(hostname);
    free(urlWide);

    return result;
}

void nbHttpClientDestroy(
    NbHttpClient            pHttpClient
) {
    if (pHttpClient != NULL)
        WinHttpCloseHandle((HINTERNET) pHttpClient);
}