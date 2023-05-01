#include <iostream>
#include <sstream>
#include <fstream>
#include <Poco/URI.h>
#include <Poco/Path.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>

using namespace Poco;
using namespace Poco::Net;

bool SendGetRequest() {
    // 设定服务器的地址
    Poco::URI uri;
    uri.setScheme("http");
    uri.setHost("127.0.0.1");
    uri.setPort(8080);

    // 定义客户端
    Poco::Net::HTTPClientSession* http_session = new HTTPClientSession();
    http_session->setPort(uri.getPort());
    http_session->setHost(uri.getHost());

    // 定义GET请求
    std::string pathAndQuery(uri.getPathAndQuery());
    // Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, pathAndQuery, Poco::Net::HTTPMessage::HTTP_1_0);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, Poco::Net::HTTPMessage::HTTP_1_1);
    request.set("Content-Type", "application/json");

    // 发送请求等待响应
    Poco::Net::HTTPResponse response;
    http_session->sendRequest(request);
    std::istream& streamIn = http_session->receiveResponse(response);

    // 获取响应内容
    std::ostringstream responseStream;
    Poco::StreamCopier::copyStream(streamIn, responseStream);
    std::string responseContent = responseStream.str();
    std::cout << "reponse: " << responseContent << std::endl;

    // 释放资源
    delete http_session;
    http_session = nullptr;
    return response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK;
}

bool SendPostRequest(const char *fileName) {
    // 设定服务器的地址
    Poco::URI uri;
    uri.setScheme("http");
    uri.setHost("127.0.0.1");
    uri.setPort(8080);

    // 定义客户端
    Poco::Net::HTTPClientSession* http_session = new HTTPClientSession();
    http_session->setPort(uri.getPort());
    http_session->setHost(uri.getHost());

    std::string logFileName = fileName;

    // 读取日志文件内容
    Path logFilePath(logFileName);
    std::ifstream logFile(logFilePath.toString().c_str());
    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContent = buffer.str();

    // 定义GET请求
    std::string pathAndQuery(uri.getPathAndQuery());
    // Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, pathAndQuery, Poco::Net::HTTPMessage::HTTP_1_0);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, Poco::Net::HTTPMessage::HTTP_1_1);
    request.set("Content-Type", "text/plain");
    request.setContentLength(logContent.length());

    // 发送请求等待响应
    Poco::Net::HTTPResponse response;
    http_session->sendRequest(request) << logContent;
    std::istream& streamIn = http_session->receiveResponse(response);

    // 获取响应内容
    std::ostringstream responseStream;
    Poco::StreamCopier::copyStream(streamIn, responseStream);
    std::string responseContent = responseStream.str();
    std::cout << "reponse: " << responseContent << std::endl;

    // 释放资源
    delete http_session;
    http_session = nullptr;
    return response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK;
}

int main(int argc, char **argv)
{
    int32_t option = 0;
    if (argc > 1) {
        option = argv[1][0] - '0';
    }
    bool ret = false;
    if (option == 0) {
        ret = SendGetRequest();
    } else {
        ret = SendPostRequest("upload_test.txt");
    }
    std::cout << "response code: " << ret << std::endl;
    return 0;
}