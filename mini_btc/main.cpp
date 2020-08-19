//
// Created by artem on 11.08.20.
//

#include "client_http.hpp"
#include "server_http.hpp"

#include <future>
#include <fstream>

#include "ApiController.h"

using namespace std;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

int main() {
  // HTTP-server at port 8080 using 1 thread
  // Unless you do more heavy non-threaded processing in the resources,
  // 1 thread is usually faster than several threads
  HttpServer server;
  ApiController apiController;
  server.config.port = 2826;

  server.resource["^/transaction$"]["POST"] = [&apiController](shared_ptr<HttpServer::Response> response,
                                                               shared_ptr<HttpServer::Request> request) {
    apiController.addTransaction(request, response);
  };

  server.resource["^/blocks"]["GET"] = [&apiController](shared_ptr<HttpServer::Response> response,
                                                        shared_ptr<HttpServer::Request> request) {
    apiController.getBlocks(request, response);
  };


  server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
    // Handle errors here
    // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
  };

  // Start server and receive assigned port when server is listening for requests
  promise<unsigned short> server_port;
  thread server_thread([&server, &server_port]() {
    // Start server
    server.start([&server_port](unsigned short port) {
      server_port.set_value(port);
    });
  });
  cout << "Server listening on port " << server_port.get_future().get() << endl
       << endl;

  server_thread.join();
}