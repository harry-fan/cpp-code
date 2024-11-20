#include "connection.h"
#include "restclient.h"
#include "json.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <mutex>

/**
 * g++ -std=c++11 send_appsflyer_event.cpp json.hpp -lcurl ./librestclient-cpp.a
 * */

using namespace std;
string buildJsonBody() {
    nlohmann::json body;
    body["appsflyer_id"] = "1729465667312-1325924166172346648";
    body["eventName"] = "af_test";
    body["eventValue"] = "";
    body["advertising_id"] = "a37a6d61-67fb-4cc9-824a-08f87989fd23";
    body["customer_user_id"] = "90fce84e927d6670_sl";
    body["custom_data"]["ta_account_id"] = "3649994769_sl";
    body["custom_data"]["ta_distinct_id"] = "90fce84e927d6670_sl";
    return body.dump();
}

std::mutex connMutex;
map<int64_t, shared_ptr<RestClient::Connection>> connects;
shared_ptr<RestClient::Connection> getConnect(int64_t index, const string &addr, const string &auth) {
    std::lock_guard<std::mutex> guard(connMutex);
    if (connects.find(index) == connects.end()) {
        connects[index] = make_shared<RestClient::Connection>(addr);
        connects[index]->SetTimeout(5);
        connects[index]->AppendHeader("accept", "application/json");
        connects[index]->AppendHeader("content-type", "application/json");
        connects[index]->AppendHeader("authentication", "zHx7WvE4YpgrdSnFVZbWkQ");
    }
//    RestClient::HeaderFields header;
//    header["accept"] = "application/json";
//    header["content-type"] = "application/json";
//    header["authentication"] = auth;
//
//    connects[index]->SetHeaders(header);
    return connects[index];
}

void sendData(int64_t index) {
    auto jsonData = buildJsonBody();
    auto host = "https://api2.appsflyer.com/";
    auto uri = "inappevent/com.immersivegames.newroa.gp";
    auto conn = getConnect(index, host, "zHx7WvE4YpgrdSnFVZbWkQ");
    RestClient::Response response = conn->post(uri, jsonData);
    std::cout << index << "|" << response.code << "|" << response.body << "|size:" << jsonData.size() << endl;
}

int main() {
    for (int i = 0; i < 3; ++i) {
        sendData(i);
    }
    for (int i = 0; i < 3; ++i) {
        sendData(i);
    }
    return 0;
}
