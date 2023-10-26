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
#include "httpclient.h"

/* Creates a buffer response object from a HTTP response object.
 *
 * Args:    client - a client object
 *          pResponse - a pointer to a NbHttpResponse object that contains the
 *          HTTP response data.
 *
 * Returns: a pointer to a NbBufferResponse object that contains the result from
 *          the API, or NULL if an error occurred(To get the error code
 *          (NbResult) call nbClientGetLastError). The caller is responsible for
 *          freeing the buffer response object with nbDestroyBufferResponse when
 *          it is no longer needed.
 */
NbBufferResponse *nbBufferResponseFromHttpResponse(NbClient client,
                                                   NbHttpResponse *pResponse);

/* Creates a response object from a HTTP response object.
 *
 * Args:    client - a client object
 *          pHttpResponse - a pointer to a NbHttpResponse object that contains
 *          the HTTP response data.
 *
 * Returns: a pointer to a NbResponse object that contains the results from the
 *          API, or NULL if an error occurred(To get the error code (NbResult)
 *          call nbClientGetLastError). The caller is responsible for
 *          freeing the response object with nbDestroyResponse when it is no
 *          longer needed.
 */
NbResponse *nbResponseFromHttpResponse(NbClient client,
                                       NbHttpResponse *pHttpResponse);