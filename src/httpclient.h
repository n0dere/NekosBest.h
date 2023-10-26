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

#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include <stddef.h>

#include "nekosbest.h"
#include "version.h"

#define NB_HTTPCLIENT_USERAGENT     "nekosbest.h "NB_LIB_VERSION
#define NB_URL_MAX_SIZE             1024

typedef void *NbHttpClient;

typedef struct _HttpResponse NbHttpResponse;

struct _HttpResponse {
    /* The status code of the response */
    long status;
    struct {
        /* The size of the response body in bytes */
        size_t size;
        /* A pointer to the response body data */
        char *pPtr;
    } body;
    struct {
        /* The value of the x-rate-limit-remaining header */
        char *pXRateLimitRemaining;
        /* The value of the x-rate-limit-reset header */
        char *pXRateLimitReset;
        /* The value of the artist_name header */
        char *pAnimeName;
        /* The value of the artist_href header */
        char *pArtistHref;
        /* The value of the anime_name header */
        char *pArtistName;
        /* The value of the source_url header */
        char *pSourceUrl;
    } header;
};

/* Creates a new HTTP client object that can be used to send and receive HTTP
 * requests and responses.
 *
 * Args:    pHttpClient - a pointer to a NbHttpClient variable that will
 *          receive the created HTTP client object.
 *
 * Returns: NB_RESULT_OK if the operation was successful, or an error code
 *          otherwise.
 */
NbResult nbHttpClientCreate(NbHttpClient *pHttpClient);

/* Sends a GET request to the given URL and receives a HTTP response object.
 *
 * Args:    client - a HTTP client object
 *          ppResponse - a pointer to a NbHttpResponse variable that
 *          will receive the response object.
 *          pUrl - a string that contains the URL to send the request to.
 *
 * Returns: NB_RESULT_OK if the operation was successful, or an error code
 *          otherwise.
 */
NbResult nbHttpClientGet(NbHttpClient client, NbHttpResponse **ppResponse, 
                         const char *pUrl);

/* Destroys a HTTP response object and frees its resources.
 *
 * Args:    pResponse - a pointer to a NbHttpResponse object to be destroyed.
 */
void nbHttpResponseDestroy(NbHttpResponse *pResponse);

/* Destroys a HTTP client object and frees its resources.
 *
 * Args:    pHttpClient - a pointer to a NbHttpClient object to be destroyed.
 */
void nbHttpClientDestroy(NbHttpClient pHttpClient);

/* Escapes a string for use in a URL query string.
 *
 * Args:    pString - a string to be escaped.
 *
 * Returns: a new string that contains the escaped version of pString, or NULL
 *          if an error occurred. The caller is responsible for freeing the
 *          returned string when it is no longer needed.
 */
char *nbHttpEscape(const char *pString);

/* Unescapes a string that was escaped for use in a URL query string.
 *
 * Args:    pString - a string to be unescaped.
 *
 * Returns: a new string that contains the unescaped version of pString,
 *          or NULL if an error occurred. The caller is responsible for
 *          freeing the returned string when it is no longer needed.
 */
char *nbHttpUnescape(const char *pString);

#endif /* _HTTPCLIENT_H_ */