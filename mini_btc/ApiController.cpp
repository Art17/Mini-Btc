//
// Created by artem on 14.08.20.
//

#include <boost/algorithm/hex.hpp>

#include "ApiController.h"
#include "utils.h"

#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif


void ApiController::addTransaction(const std::shared_ptr<HttpServer::Request> &request,
                                   std::shared_ptr<HttpServer::Response> &response) {
  try {
    ptree pt;
    read_json(request->content, pt);

    Transaction tx;

    for(ptree::value_type &child : pt.get_child("inputs")) {
      const boost::property_tree::ptree &subtree = child.second;

      boost::optional<std::string> opt_hash = subtree.get_optional<std::string>("hash");
      boost::optional<std::string> opt_sig = subtree.get_optional<std::string>("sig");
      boost::optional<std::string> opt_index = subtree.get_optional<std::string>("index");

      if(!opt_hash.is_initialized()) {
        returnErrorResponse("missing 'hash' field of input", response);
        return;
      }
      if(!opt_sig.is_initialized()) {
        returnErrorResponse("missing 'sig' field of input", response);
        return;
      }
      if(!opt_index.is_initialized()) {
        returnErrorResponse("missing 'index' field of input", response);
        return;
      }

      std::string hash = opt_hash.get();
      std::string sig = opt_sig.get();
      uint32_t index = std::stoul(opt_index.get());

      remove0x(hash);
      remove0x(sig);

      if(hash.size() != sizeof(Hash)*2) {
        returnErrorResponse("Invalid 'hash' value", response);
        return;
      }
      if(sig.size() != sizeof(Signature)*2) {
        returnErrorResponse("Invalid 'sig' value", response);
        return;
      }

      Input input = {hexStrToHash(hash), index, hexStrToSig(sig)};
      tx.inputs.push_back(input);
    }

    for(ptree::value_type &child : pt.get_child("outputs")) {
      const boost::property_tree::ptree &subtree = child.second;

      boost::optional<std::string> opt_key = subtree.get_optional<std::string>("key");
      boost::optional<std::string> opt_amount = subtree.get_optional<std::string>("amount");

      if(!opt_key.is_initialized()) {
        returnErrorResponse("missing 'key' field of output", response);
        return;
      }
      if(!opt_amount.is_initialized()) {
        returnErrorResponse("missing 'amount' field of output", response);
        return;
      }

      std::string key = opt_key.get();
      uint32_t amount = std::stoul(opt_amount.get());

      remove0x(key);

      if(key.size() != sizeof(Hash)*2) {
        returnErrorResponse("Invalid 'key' value", response);
        return;
      }

      Output output = {amount, hexStrToHash(key)};
      tx.outputs.push_back(output);
    }

    handleAddTransactionResponse(miner.addTransaction(tx), response);
  }
  catch(const exception &e) {
    returnErrorResponse("Invalid transaction data", response);
  }
}

void ApiController::handleAddTransactionResponse(TransactionCheckResult res, std::shared_ptr<HttpServer::Response> &response) {
  std::string message;
  bool success = false;

  switch(res) {
  case AddedToMempool:
    success = true;
    break;
  case UnknownInputHash:
    message = "Input doesn't exist";
    break;
  case InvalidInputIndex:
    message = "Invalid input index";
    break;
  case AlreadySpentOutput:
    message = "Output is already spent";
    break;
  case InvalidOutputAmount:
    message = "Invalid output amount";
    break;
  case InvalidSignature:
    message = "Signature doesn't match";
    break;
  }

  if(success)
    returnSuccessResponse(response);
  else
    returnErrorResponse(message, response);
}

void ApiController::returnSuccessResponse(std::shared_ptr<HttpServer::Response> &response)
{
  std::string jsonResponse = "{\"success\": " + std::to_string(true) + "}";
  *response << "HTTP/1.1 200 OK\r\n"
            << "Content-Length: " << jsonResponse.length() << "\r\n\r\n"
            << jsonResponse;
}

void ApiController::returnErrorResponse(const std::string& message, std::shared_ptr<HttpServer::Response> &response)
{
  std::string jsonResponse = "{\"success\": " + std::to_string(false) + ", \"error\": \"" + message + "\"}";
  *response << "HTTP/1.1 400 Bad Request\r\n"
            << "Content-Length: " << jsonResponse.length() << "\r\n\r\n"
            << jsonResponse;
}

void ApiController::getBlocks(const std::shared_ptr<HttpServer::Request> &request,
                              std::shared_ptr<HttpServer::Response> &response) {
  std::string blocksStr = Blockchain::getInstance()->jsonify();
  *response << "HTTP/1.1 200 OK\r\n"
            << "Content-Length: " << blocksStr.length() << "\r\n"
            << "Content-Type: application/json"
            << "\r\n\r\n"
            << blocksStr;
}
