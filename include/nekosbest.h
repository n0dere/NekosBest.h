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

#ifndef _NEKOSBEST_H_
#define _NEKOSBEST_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(_WIN32) && defined(_NB_SHARED)
    #define NB_API __declspec(dllexport)
#elif defined(_WIN32) && defined(_NB_DLL_EXPORT)
    #define NB_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(_NB_SHARED)
    #define NB_API __attribute__((visibility("default")))
#else
    #define NB_API
#endif

typedef enum _NbResult NbResult;
typedef enum _NbImageFormat NbImageFormat;

typedef struct _NbApiInfo NbApiInfo;
typedef struct _NbClient *NbClient;
typedef struct _NbIndividualResponse NbIndividualResponse;
typedef struct _NbResponse NbResponse;
typedef struct _NbBufferResponse NbBufferResponse;
typedef struct _NbSearchOptions NbSearchOptions;

enum _NbResult {
    /* The operation was successful */
    NB_RESULT_OK,
    /* There was not enough memory to complete the operation */
    NB_RESULT_NO_MEMORY,
    /* One or more of the parameters were invalid */
    NB_RESULT_INVALID_PARAMETER,
    /* There was an error initializing the HTTP client */
    NB_RESULT_HTTPCLIENT_INITIALIZATION_ERROR,
    /* There was an internal error in the HTTP client (no internet) */
    NB_RESULT_HTTPCLIENT_INTERNAL_ERROR,
    /* There was an error initializing the client object */
    NB_RESULT_CLIENT_INTERNAL_INIT_ERROR,
    /* There was an error parsing the JSON response */
    NB_RESULT_JSON_PARSE_ERROR,
    /* The amount parameter was out of range (0 < amount <= 20) */
    NB_RESULT_AMOUNT_IS_INCORRECT,
    /* The response status code was not 200 OK */
    NB_RESULT_BAD_RESPONSE_STATUS_CODE,
    /* The category parameter was not recognized by the API */
    NB_RESULT_UNKNOWN_CATEGORY,
    /* The search request was rate limited by the API */
    NB_RESULT_SEARCH_RATE_LIMITED,
    /* The query parameter was too long or too short
       (3 < strlen(query) < 150) */
    NB_RESULT_QUERY_LEN_IS_INCORRECT,
};

enum _NbImageFormat {
    /* The image format is unknown or not supported by the library */
    NB_IMAGE_FORMAT_UNKNOWN,
    /* The image format is PNG */
    NB_IMAGE_FORMAT_PNG,
    /* The image format is GIF */
    NB_IMAGE_FORMAT_GIF,
    /* The image format is random (same as unknown) */
    NB_IMAGE_FORMAT_RANDOM = NB_IMAGE_FORMAT_UNKNOWN,
};

struct _NbApiInfo {
    /* The version number of the nekos.best API */
    size_t apiVersion;
    /* The version string of the nekos.best API, e.g. "v2" */
    const char *pApiVersionString;
    /* The length of the version string of the nekos.best API */
    size_t apiVersionStringLen;
    /* The base URL of the nekos.best API, e.g. "https://nekos.best/api/v2" */
    const char *pApiBaseUrl;
    /* The length of the base URL of the nekos.best API */
    size_t apiBaseUrlLen;
    /* The version string of the library, e.g. "2.0.0" */
    const char *pLibVersion;
    /* The length of the version string of the library */
    size_t libVersionLen;
};

struct _NbIndividualResponse {
    /* The name of the anime that the image belongs to, or NULL if unknown */
    char *pAnimeName;
    /* The URL of the artist's profile page, or NULL if unknown */
    char *pArtistHref;
    /* The name of the artist who created the image, or NULL if unknown */
    char *pArtistName;
    /* The URL of the original source of the image, or NULL if unknown */
    char *pSourceUrl;
    /* The URL of the image on nekos.best server */
    char *pUrl;
};

struct _NbResponse {
    /* An array of NbIndividualResponse objects */
    NbIndividualResponse *pResults;
    /* The number of elements in the pResults array */
    size_t resultsCount;
};

struct _NbBufferResponse {
    /* A NbIndividualResponse object that contains the information about the
       file returned by the API */
    NbIndividualResponse meta;
    /* A pointer to a buffer that contains the data of the file returned by
       the API */
    char *pBytes;
    /* The size of the data buffer in bytes */
    size_t byteCount;
    /* Downloaded image format */
    NbImageFormat imageFormat;
};

struct _NbSearchOptions {
    /* The desired image format (NB_IMAGE_FORMAT_PNG or NB_IMAGE_FORMAT_GIF) */
    NbImageFormat imageFormat;
    /* The number of results to return (0 < amount <= 20) */
    size_t amount;
    /* The category to search within, e.g. "neko", "kitsune", etc. (optional) */
    const char *pCategory;
};

/* Returns a pointer to a constant NbApiInfo object that contains information
 * about the API and the library.
 *
 * Returns: a pointer to a constant NbApiInfo structure. This pointer should
 *          not be modified or freed by the caller.
 */
NB_API const NbApiInfo *nbGetApiInfo(void);

/* Creates a new client object that can be used to interact with the API.
 * 
 * Args:    pClient - a pointer to a NbClient variable that will receive the
 *          created client object.
 *
 * Returns: NB_RESULT_OK if the operation was successful, or an error
 *          code otherwise.
 */
NB_API NbResult nbCreateClient(NbClient *pClient);

/* Returns the last error code that occurred in the current thread using the
 * given client object.
 *
 * Args:    client - a client object
 *
 * Returns: an error code that represents the last error that occurred in the
 *          current thread, or NB_RESULT_OK if no error occurred.
 */
NB_API NbResult nbClientGetLastError(const NbClient client);

/* Fetches images from the API using the given category and amount parameters.
 *
 * Args:    client - a client object
 *          pCategory - a string that contains the category to fetch images from
 *          amount - the number of images to fetch (0 < amount <= 20)
 *
 * Returns: a pointer to a NbResponse object that contains the results from the
 *          API, or NULL if an error occurred(To get the error code (NbResult)
 *          call nbClientGetLastError). The caller is responsible for
 *          freeing the response object with nbDestroyResponse when it is no
 *          longer needed.
 */
NB_API NbResponse *nbClientFetch(NbClient client, const char *pCategory,
                                 size_t amount);

/* Searches images from the API using the given query and options parameters.
 *
 * Args:    client - a client object
 *          pQuery - a string that contains the query to search for images
 *          pOptions - a pointer to a NbSearchOptions structure that contains
 *          the options for the search (optional)
 *
 * Returns: a pointer to a NbResponse object that contains the results from the
 *          API, or NULL if an error occurred(To get the error code (NbResult)
 *          call nbClientGetLastError). The caller is responsible for
 *          freeing the response object with nbDestroyResponse when it is no
 *          longer needed.
 */
NB_API NbResponse *nbClientSearch(NbClient client, const char *pQuery,
                                  const NbSearchOptions *pOptions);

/* Fetches a single image file from the API using the given category parameter.
 *
 * Args:    client - a client object
 *          pCategory - a string that contains the category to fetch
 *          an image from
 *
 * Returns: a pointer to a NbBufferResponse object that contains the result
 *          from the API, or NULL if an error occurred(To get the error code
 *          (NbResult) call nbClientGetLastError). The caller is responsible for
 *          freeing the buffer response object with nbDestroyBufferResponse when
 *          it is no longer needed.
 */
NB_API NbBufferResponse *nbClientFetchFile(NbClient client,
                                           const char *pCategory);

/* Downloads a single image file from the API using the given response object.
 *
 * Args:    client - a client object
 *          pResponse - a pointer to a NbIndividualResponse object
 *
 * Returns: a pointer to a NbBufferResponse object that contains the result from
 *          the API, or NULL if an error occurred. The caller is responsible for
 *          freeing the buffer response object with nbDestroyBufferResponse when
 *          it is no longer needed.
 */
NB_API NbBufferResponse *nbClientDownloadResponse(NbClient client,
        const NbIndividualResponse *pResponse);

/* Destroys a client object that was created by nbCreateClient and frees the
 * memory allocated for it.
 *
 * Args:    client - a NbClient object
 */
NB_API void nbDestroyClient(NbClient client);

/* Destroys a response object that was created by nbClientFetch or
 * nbClientSearch and frees the memory allocated for it.
 *
 * Args:    pResponse - a pointer to a NbResponse object
 */
NB_API void nbDestroyResponse(NbResponse *pResponse);

/* Destroys a buffer response object that was created by nbClientFetchFile
 * and frees the memory allocated for it and the file data.
 * 
 * Args:    pBufferResponse - a pointer to a NbBufferResponse object
 */
NB_API void nbDestroyBufferResponse(NbBufferResponse *pBufferResponse);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _NEKOSBEST_H_ */