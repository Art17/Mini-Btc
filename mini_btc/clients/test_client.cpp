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

  /*
   * public key: D25F2E1496034197C17B595CAC3CFBDAA3D04C0A46C17F62F8DABA4B8415B053
   * private key: 209EFEC81D4E08E875C3C8F939300003ECFACC4E75A75D8D615282D087032F35
   */

  std::string genesisTx = "{"
                        "\"outputs\":["
                        "{"
                        "\"key\":\"0xD25F2E1496034197C17B595CAC3CFBDAA3D04C0A46C17F62F8DABA4B8415B053\","
                        "\"amount\":50000000"
                        "},"
                        "{"
                        "\"key\":\"0xD25F2E1496034197C17B595CAC3CFBDAA3D04C0A46C17F62F8DABA4B8415B053\","
                        "\"amount\":50000000"
                        "}"
                        "],"
                        "\"inputs\":["
                        "]"
                        "}";
  /* input doesn't exist */
  std::string nonExistingInputTx = "{"
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
  /* valid input invalid sig*/
  std::string validInpInvalidSig = "{"
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
                         "\"hash\": \"0xECA999415A749308245688CC7027AFBD76FC861309F5BBE3A2BEAADABA707D6A\","
                         "\"sig\": \"0x021dc84d9fd80270d0241a0ec86e85074982d04ccd62f4d05ca6993206d0f8cac9543b36eddf73b3236919a6d9a32e7a7c51d28b30873492c952ce22d63b75a1\","
                         "\"index\": 1"
                         "}"
                         "]"
                         "}";
  std::string outputGreaterThanInput = "{"
                        "\"outputs\": ["
                        "{"
                        "\"key\": \"0x0493f48e3d5ff56c8e8da005f3d4471d694103694b611a91592f3a04d02fdf29\","
                        "\"amount\": 89999999"
                        "},"
                        "{"
                        "\"key\": \"0xef220c2983cc459bd4928cef19a26c7c03821502495141e021a5754dad2c1e10\","
                        "\"amount\": 29999999"
                        "}"
                        "],"
                        "\"inputs\": ["
                        "{"
                        "\"hash\": \"0xECA999415A749308245688CC7027AFBD76FC861309F5BBE3A2BEAADABA707D6A\","
                        "\"sig\": \"0xACD5785BA43BEE5E6C2E1433EBC9CD72CB03C1E2D99F69A3982A3D80B0CFBEA683AD03849569E4E653A6970FFE77092A01EDBFC445260604EF3566DFC13C750D\","
                        "\"index\": 1"
                        "}"
                        "]"
                        "}";
  /* valid input valid sig
   * tx hash - 0x3BCE02D274ED8E656BE742F2571451380B55D7E54856A862471A4183EB48D6F8
   * */
  std::string validTx = "{"
                         "\"outputs\": ["
                         "{"
                         "\"key\": \"0x0493f48e3d5ff56c8e8da005f3d4471d694103694b611a91592f3a04d02fdf29\","
                         "\"amount\": 8000000"
                         "},"
                         "{"
                         "\"key\": \"0xef220c2983cc459bd4928cef19a26c7c03821502495141e021a5754dad2c1e10\","
                         "\"amount\": 2000000"
                         "}"
                         "],"
                         "\"inputs\": ["
                         "{"
                         "\"hash\": \"0xECA999415A749308245688CC7027AFBD76FC861309F5BBE3A2BEAADABA707D6A\","
                         "\"sig\": \"0x8E0AE95547EA0CFBF957BBF9F00123EB2833A6D5817F2194970CB7A2E7E3E0938BA7EF5F91BA66CB0AF1F06DE2F039A9AC0DFE0B244F174B4D6925B4B76FD009\","
                         "\"index\": 1"
                         "}"
                         "]"
                         "}";
  std::vector<std::string> txs = {genesisTx, nonExistingInputTx, validInpInvalidSig, outputGreaterThanInput, validTx};

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
        sleep(1); // wait for transaction to be mined
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