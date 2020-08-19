# Mini Btc
Description - https://bpfkorea.github.io/infrastructure/

## Compile and run

Compile with a C++11 compliant compiler:
```sh
cmake -H. -Bbuild
cmake --build build
```

### HTTP

Run the server and client examples: `./build/mini_btc`

### Examples

curl -X POST -H "Content-Type: application/json" http://127.0.0.1:2826/transaction --data-raw '{"outputs":[{"key":"0xee0edfbe581fa81481e858f75fda3414cd3a655ff17ec987af775e666412bf1f","amount":50000000},{"key":"0xf4012c71c9be512613c4820a3f6b1e55f8515a441913cf9cb06ccc3076ccbebd","amount":50000000}],"inputs":[]}'

curl -X POST -H "Content-Type: application/json" http://127.0.0.1:2826/transaction --data-raw '{"outputs":[{"key":"0x0493f48e3d5ff56c8e8da005f3d4471d694103694b611a91592f3a04d02fdf29","amount":4000000},{"key":"0xef220c2983cc459bd4928cef19a26c7c03821502495141e021a5754dad2c1e10","amount":96000000}],"inputs":[{"hash":"0xdec51b005b3a0014d831814fbb1f3d66d960be2e676fb92f731662576513054b","sig":"0x04eeb678a93c49a246e393961cdbea815dae0819580a01a6de0c5f6929c7fe9550ef800d2fcdb6191f7c659de07444a363de7f0ed4e885f705c692b22228b904","index":0},{"hash":"0xdec51b005b3a0014d831814fbb1f3d66d960be2e676fb92f731662576513054b","sig":"0x08ebc799428fe3b63bdc744bed1e5d6e8c56b36384b2e4ad2b074b90c8a8718ffd7b0afd8e8ccc931a62f511cba098f4444c48ea8b4ba4590015890bd9e81ae6","index":1}]}'

curl -X GET http://127.0.0.1:2826/blocks

# Simple-Web-Server

A very simple, fast, multithreaded, platform independent HTTP and HTTPS server and client library implemented using C++11 and Asio (both Boost.Asio and standalone Asio can be used). Created to be an easy way to make REST resources available from C++ applications. 

See https://gitlab.com/eidheim/Simple-WebSocket-Server for an easy way to make WebSocket/WebSocket Secure endpoints in C++. Also, feel free to check out the new C++ IDE supporting C++11/14/17: https://gitlab.com/cppit/jucipp. 

## Features

* Asynchronous request handling
* Thread pool if needed
* Platform independent
* HTTP/1.1 supported, including persistent connections
* HTTPS supported
* Chunked transfer encoding and server-sent events
* Can set timeouts for request/response and content
* Can set max request/response size
* Sending outgoing messages is thread safe
* Client creates necessary connections and perform reconnects when needed

See also [benchmarks](https://gitlab.com/eidheim/Simple-Web-Server/blob/master/docs/benchmarks.md) for a performance comparisons to a few other HTTP libraries.

## Usage

See [http_examples.cpp](https://gitlab.com/eidheim/Simple-Web-Server/blob/master/http_examples.cpp) or
[https_examples.cpp](https://gitlab.com/eidheim/Simple-Web-Server/blob/master/https_examples.cpp) for example usage.
The following server resources are setup using regular expressions to match request paths:
* `POST /string` - responds with the posted string.
* `POST /json` - parses the request content as JSON, and responds with some of the parsed values.
* `GET /info` - responds with information extracted from the request.
* `GET /match/([0-9]+)` - matches for instance `/match/123` and responds with the matched number `123`.
* `GET /work` - starts a thread, simulating heavy work, and responds when the work is done.
* `GET` - a special default_resource handler is called when a request path does not match any of the above resources.
This resource responds with the content of files in the `web/`-folder if the request path identifies one of these files.

[Documentation](https://eidheim.gitlab.io/Simple-Web-Server/annotated.html) is also available, generated from the master branch.

## Dependencies

* Boost.Asio or standalone Asio
* Boost is required to compile the examples
* For HTTPS: OpenSSL libraries

Installation instructions for the dependencies needed to compile the examples on a selection of platforms can be seen below.
Default build with Boost.Asio is assumed. Turn on CMake option `USE_STANDALONE_ASIO` to instead use standalone Asio.

### Debian based distributions

```sh
sudo apt-get install libssl-dev libboost-filesystem-dev libboost-thread-dev
```

### Arch Linux based distributions

```sh
sudo pacman -S boost
```

### MacOS

```sh
brew install openssl boost
```

## Compile and run

Compile with a C++11 compliant compiler:
```sh
cmake -H. -Bbuild
cmake --build build
```

### HTTP

Run the server and client examples: `./build/http_examples`

Direct your favorite browser to for instance http://localhost:8080/

### HTTPS

Before running the server, an RSA private key (server.key) and an SSL certificate (server.crt) must be created.

Run the server and client examples: `./build/https_examples`

Direct your favorite browser to for instance https://localhost:8080/

## Contributing

Contributions are welcome, either by creating an issue or a merge request.
However, before you create a new issue or merge request, please search for previous similar issues or requests.
A response will normally be given within a few days.
