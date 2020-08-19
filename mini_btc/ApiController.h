//
// Created by artem on 14.08.20.
//

#ifndef SIMPLE_WEB_SERVER_APICONTROLLER_H
#define SIMPLE_WEB_SERVER_APICONTROLLER_H

#include "client_http.hpp"
#include "server_http.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif

#include "Miner.h"
#include "mini_btc/data_types/data_types.h"

using namespace std;
using namespace boost::property_tree;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

class ApiController {
public:
  void addTransaction(const std::shared_ptr<HttpServer::Request> &request,
                      std::shared_ptr<HttpServer::Response> &response);
  void getBlocks(const std::shared_ptr<HttpServer::Request> &request,
                 std::shared_ptr<HttpServer::Response> &response);

protected:
  static void handleAddTransactionResponse(TransactionCheckResult res, std::shared_ptr<HttpServer::Response> &response);
  static void returnSuccessResponse(std::shared_ptr<HttpServer::Response> &response);
  static void returnErrorResponse(const std::string &message, std::shared_ptr<HttpServer::Response> &response);

private:
  Miner miner;
};


#endif //SIMPLE_WEB_SERVER_APICONTROLLER_H
