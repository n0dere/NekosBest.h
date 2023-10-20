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

#ifndef _NEKOSBEST_H_
#define _NEKOSBEST_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NB_API

typedef enum _NbResult NbResult;
typedef enum _NbImageFormat NbImageFormat;

typedef struct _NbApiInfo NbApiInfo;
typedef struct _NbClient *NbClient;
typedef struct _NbIndividualResponse NbIndividualResponse;
typedef struct _NbResponse NbResponse;
typedef struct _NbBufferResponse NbBufferResponse;
typedef struct _NbSearchOptions NbSearchOptions;

enum _NbResult {
    NB_RESULT_OK,
    NB_RESULT_NO_MEMORY,
    NB_RESULT_INVALID_PARAMETER,
    NB_RESULT_HTTPCLIENT_INITIALIZATION_ERROR,
    NB_RESULT_HTTPCLIENT_INTERNAL_ERROR,
    NB_RESULT_CLIENT_INTERNAL_INIT_ERROR,
    NB_RESULT_JSON_PARSE_ERROR,
    NB_RESULT_AMOUNT_IS_INCORRECT,
    NB_RESULT_BAD_RESPONSE_STATUS_CODE,
    NB_RESULT_UNKNOWN_CATEGORY,
};

enum _NbImageFormat {
    /* The image format is unknown or not supported by the library */
    NB_IMAGE_FORMAT_UNKNOWN,
    NB_IMAGE_FORMAT_PNG,
    NB_IMAGE_FORMAT_GIF,
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
    /* The number of elements in the pResults array  */
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
};

struct _NbSearchOptions {
    /* The desired image format (NB_IMAGE_FORMAT_PNG / NB_IMAGE_FORMAT_GIF) */
    NbImageFormat imageFormat;
    /* The maximum number of results to return */
    size_t amount;
    /* The category to search within, e.g. "neko", "kitsune", etc. (optional) */
    const char *pCategory;
};

/* Returns a pointer to a constant NbApiInfo object that contains information
 * about the API and the library.
 *
 * This function does not perform any network requests and does not allocate
 * any memory.
 */
NB_API const NbApiInfo *nbGetApiInfo(void);

/* Creates a new client object that can be used to interact with the internal
 * state of the client.
 * 
 * The caller is responsible for destroying the client object with
 * nbDestroyClient when it is no longer needed.
 * 
 * Args:    pClient - a pointer to a NbClient variable that will receive the
 *          created client object.
 */
NB_API NbResult nbCreateClient(NbClient *pClient);

/* Returns the last error code that occurred during an API call by the client.
 * 
 * Args:    client - a NbClient object that represents the communication with
 *          the API
 */
NB_API NbResult nbClientGetLastError(const NbClient client);

/* Fetches a collection of images from the nekos.best API based on the given
 * category and amount.
 *
 * This function performs a network request to the API and parses the JSON
 * response into a NbResponse object that contains the information about each
 * image.
 * 
 * The caller is responsible for destroying the NbResponse object with
 * nbDestroyResponse when it is no longer needed.
 * 
 * Args:    client - a NbClient object that represents the communication with
 *          the API
 *          pCategory - a null-terminated string that specifies the category
 *          of images to fetch, e.g. "neko", "kitsune", etc. (optional)
 *          amount - a integer that specifies the maximum number of images
 *          to fetch
*/
NB_API NbResponse *nbClientFetch(NbClient client, const char *pCategory,
                                 size_t amount);

/* Searches for images from the nekos.best API based on the given query and
 * options.
 * 
 * This function performs a network request to the API and parses the JSON
 * response into a NbResponse object that contains the information about each
 * image.
 * 
 * The caller is responsible for destroying the NbResponse object with
 * nbDestroyResponse when it is no longer needed.
 * 
 * Args:    client - a NbClient object that represents the communication with
 *          the API
 *          pQuery - a null-terminated string that specifies the query
 *          to search for, e.g. "Senko", "Yuru Yuri", etc.
 *          pOptions - a pointer to a NbSearchOptions object that specifies
 *          the options for the search, e.g. image format, amount, category,
 *          etc. (optional)
 */
NB_API NbResponse *nbClientSearch(NbClient client, const char *pQuery,
                                  const NbSearchOptions *pOptions);

/* Fetches and download a file with its metadata from the nekos.best API
 * based on the given category.
 * 
 * This function performs a network request to the API and downloads the file
 * data into a NbBufferResponse object that contains the information and
 * the data of the file.
 * 
 * The caller is responsible for destroying the NbBufferResponse object with
 * nbDestroyBufferResponse when it is no longer needed.
 * 
 * Args:    client - a NbClient object that represents the communication with
 *          the API
 *          pCategory - a null-terminated string that specifies the category
 *          of images to fetch, e.g. "neko", "kitsune", etc. (optional)
 */
NB_API NbBufferResponse *nbClientFetchFile(NbClient client,
                                           const char *pCategory);

/* Destroys a client object that was created by nbCreateClient and frees the
 * memory allocated for it.
 *
 * This function also cleans up any internal resources used by the client
 * object, such as the HTTP client
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