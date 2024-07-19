#include "connection.h"
#include "restclient.h"
#include "json.hpp"
#include <string>
#include <iostream>

using namespace std;
string buildJsonBody() {
    nlohmann::json body;
    body["appsflyer_id"] = "1709971894353-766436164349772110";
    body["eventName"] = "af_completed_payment";
    body["eventValue"] = "0.99";
    body["advertising_id"] = "d172da8b-0553-42bb-b34c-2c1c15fcdafa";
    body["customer_user_id"] = "a008df0f79ef52b8_sl";
    body["custom_data"]["ta_account_id"] = "3197370371_sl";
    body["custom_data"]["ta_distinct_id"] = "a008df0f79ef52b8_sl";
    return body.dump();
}

int main() {
    auto jsonData = buildJsonBody();
    auto host = "https://api2.appsflyer.com/";
    auto uri = "inappevent/com.immersivegames.newroa.gp";
    auto conn = make_shared<RestClient::Connection>(host);
    conn->AppendHeader("accept", "application/json");
    conn->AppendHeader("content-type", "application/json");
    conn->AppendHeader("authentication", "zHx7WvE4YpgrdSnFVZbWkQ");
    RestClient::Response response = conn->post(uri, jsonData);
    std::cout << response.code << "|" << response.body << "|size:" << jsonData.size() << endl;
    return 0;
}
