# NekosBest.h

[<img align="right" style="padding: 2px" src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white">](https://github.com/search?q=repo%3An0dere%2FNekosBest.h++language%3AC&type=code)
[<img align="right" style="padding: 2px" src="https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white">](https://cmake.org/)
[<img align="right" style="padding: 2px" src="https://img.shields.io/badge/nekos.best-%23E91E63.svg?style=for-the-badge">](https://nekos.best/)

NekosBest.h is a wrapper over the nekos.best API.  
It is written in pure C and is easy to use.

## Example

```c
#include <stdio.h>

#include <nekosbest.h>

int main(void)
{
    NbResponse *pNeko = NULL;
    NbClient client;

    if (nbCreateClient(&client) != NB_RESULT_OK)
        return -1;
    
    if ((pNeko = nbClientFetch(client, "neko", 1)) != NULL) {
        printf("Artist Href:\t%s\n", pNeko->pResults[0].pArtistHref);
        printf("Artist Name:\t%s\n", pNeko->pResults[0].pArtistName);
        printf("Source Url: \t%s\n", pNeko->pResults[0].pSourceUrl);
        printf("Url:        \t%s\n", pNeko->pResults[0].pUrl);
        nbDestroyResponse(pNeko);
    }

    nbDestroyClient(client);
    return 0;
}
```

This code creates a client object and fetches one image of a neko from the API.
It then prints the artist’s name, link, source URL, and image URL to
the standard output. The output looks like this:

```
Artist Href:    https://www.pixiv.net/en/users/29694453
Artist Name:    ツユハ
Source Url:     https://www.pixiv.net/en/artworks/98258625
Url:            https://nekos.best/api/v2/neko/84211882-59a0-4229-8ec2-47ae2f095ce7.png
```

For more examples, see [examples/](examples) directory

## Dependencies

|             | Req Version | OS                          | Category |
|:------------|:-----------:|-----------------------------|:--------:|
| WinHTTP     | >= 5.1      | Windows                     | Runtime  |
| libcurl     | > 7.82.0    | Windows, Linux and other    | Runtime  |
| parson      |             | any                         | Build    |
| tinycthread |             | any                         | Build    |

 - [parson](https://github.com/kgabis/parson) ([MIT](
    https://opensource.org/license/mit/)) for parsing json files (build only)
 - [tinycthread](https://github.com/tinycthread/tinycthread) ([zlib](
    https://opensource.org/license/zlib/)) for thread support (build only)
 - [libcurl](https://curl.se/) ([MIT based](
    https://curl.se/docs/copyright.html)) is used as an http client
 - [winhttp](
    https://learn.microsoft.com/en-us/windows/win32/winhttp/about-winhttp)
     (Windows) is used as an http client (Windows only)

## Building with NekosBest.h & Installation

### `add_subdirectory` (CMake)
1. Clone the NekosBest.h repository from GitHub into your project directory,
or add it as a submodule to your Git repository:
```sh
git submodule add https://github.com/n0dere/NekosBest.h
```
2. Add the following lines to your CMakeLists.txt file, where `MyCoolApp` is
the name of your target:
```cmake
add_subdirectory(NekosBest.h)
target_link_libraries(MyCoolApp PRIVATE NekosBest)
```

### Install using CMake on Linux
1. Clone the [NekosBest.h](https://github.com/n0dere/NekosBest.h) repository
from GitHub:
```sh
git clone https://github.com/n0dere/NekosBest.h
```
2. Go to the NekosBest.h directory using `cd NekosBest.h`
3. Run the following command to install the library on the system:
```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build --target install --config Release 
```
4. Build your project using the compiler flags `-lNekosBest -lcurl`
```sh
gcc -o main.out main.c -lNekosBest -lcurl
```

## Documentation
See the [nekosbest.h](include/nekosbest.h) file for information on using the
library's functions.

## License
The project is distributed under the `MIT license`. See [LICENSE](LICENSE) file
for details.
