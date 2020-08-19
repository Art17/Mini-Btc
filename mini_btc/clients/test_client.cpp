//
// Created by artem on 15.08.20.
//

#include "client_http.hpp"
#include "server_http.hpp"

#include <fstream>

#include "mini_btc/ApiController.h"

using namespace std;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

int main() {
  const int serverPort = 2826;

  std::string firstTx = "{"
                        "\"outputs\":["
                        "{"
                        "\"key\":\"0xee0edfbe581fa81481e858f75fda3414cd3a655ff17ec987af775e666412bf1f\","
                        "\"amount\":50000000"
                        "},"
                        "{"
                        "\"key\":\"0xf4012c71c9be512613c4820a3f6b1e55f8515a441913cf9cb06ccc3076ccbebd\","
                        "\"amount\":50000000"
                        "}"
                        "],"
                        "\"inputs\":["
                        "]"
                        "}";

  std::string secondTx = "{"
                         "\"outputs\": ["
                         "{"
                         "\"key\": \"0x0493f48e3d5ff56c8e8da005f3d4471d694103694b611a91592f3a04d02fdf29\","
                         "\"amount\": 80000000"
                         "},"
                         "{"
                         "\"key\": \"0xef220c2983cc459bd4928cef19a26c7c03821502495141e021a5754dad2c1e10\","
                         "\"amount\": 20000000"
                         "}"
                         "],"
                         "\"inputs\": ["
                         "{"
                         "\"hash\": \"0x72e4571be2d9c51ab4128ceb4fa40e076940d5d8dd11f2582a977633ecbd3fcb\","
                         "\"sig\": \"0x0ecb65f6d3861665e6b2382488887b3e1323f8481132ff77a851b83d98842e5fb97841c2d66cca70f53ce9b2eabcfedff48fabd8fc094fe0be57b020a36b66a9\","
                         "\"index\": 0"
                         "},"
                         "{"
                         "\"hash\": \"0x72e4571be2d9c51ab4128ceb4fa40e076940d5d8dd11f2582a977633ecbd3fcb\","
                         "\"sig\": \"0x021dc84d9fd80270d0241a0ec86e85074982d04ccd62f4d05ca6993206d0f8cac9543b36eddf73b3236919a6d9a32e7a7c51d28b30873492c952ce22d63b75a1\","
                         "\"index\": 1"
                         "}"
                         "]"
                         "}";

  std::string validInp = "{"
                         "\"outputs\": ["
                         "{"
                         "\"key\": \"0x0493f48e3d5ff56c8e8da005f3d4471d694103694b611a91592f3a04d02fdf29\","
                         "\"amount\": 80000000"
                         "},"
                         "{"
                         "\"key\": \"0xef220c2983cc459bd4928cef19a26c7c03821502495141e021a5754dad2c1e10\","
                         "\"amount\": 20000000"
                         "}"
                         "],"
                         "\"inputs\": ["
                         "{"
                         "\"hash\": \"0x7983741BE049FF4F272E5758C093BA887D2157CA7A1915DC649A5091C88A6ACD\","
                         "\"sig\": \"0x021dc84d9fd80270d0241a0ec86e85074982d04ccd62f4d05ca6993206d0f8cac9543b36eddf73b3236919a6d9a32e7a7c51d28b30873492c952ce22d63b75a1\","
                         "\"index\": 1"
                         "}"
                         "]"
                         "}";
  std::vector<std::string> txs = {firstTx, secondTx, validInp};

  // Synchronous request examples
  {
    HttpClient client("localhost:" + std::to_string(serverPort));
    try {
      for(const auto& tx : txs)
      {
        cout << "Example POST request to http://localhost:"+ std::to_string(serverPort) +"/transaction" << endl;
        auto r2 = client.request("POST", "/transaction", tx);
        cout << "Response content: " << r2->content.rdbuf() << endl
             << endl;
      }

      cout << "Example GET request to http://localhost:"+ std::to_string(serverPort) +"/blocks" << endl;
      auto r2 = client.request("GET", "/blocks");
      cout << "Response content: " << r2->content.rdbuf() << endl
           << endl;
    }
    catch(const SimpleWeb::system_error &e) {
      cerr << "Client request error: " << e.what() << endl;
    }
  }
}