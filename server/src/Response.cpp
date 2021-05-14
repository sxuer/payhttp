#include <string>
#include <vector>
#include "server/include/Response.hpp"
#include "web/core/Json.hpp"
#include "web/src/model/Transaction.hpp"

namespace payhttp {
    Response Response::stockResponse(const unsigned short status) {
        Response rep;
        rep.status_ = status;
        rep.content_ = stockReplies::toHtml(status);
        rep.headers_["Content-Length"] = std::to_string(rep.content_.size());
        rep.headers_["Content-Type"] = "text/html; charset=utf-8";
        return rep;
    }

    void Response::html(Response& response, unsigned short status, const std::string& msg) {
        response.status_ = status;

        std::string retMsg = "<div>" + msg + "</div>";

        response.content_ = stockReplies::toHtml(status);
        unsigned long i = response.content_.find("</body>");
        if (i != std::string::npos || (i > 20 && i < response.content_.size())) {
            response.content_.insert(i, retMsg);
        }
        response.headers_["Content-Length"] = std::to_string(response.content_.size());
        response.headers_["Content-Type"] = "text/html; charset=utf-8";
    }

    std::string Response::toString() {
        std::string res("HTTP/1.1 ");

        res += std::to_string(status_);
        res += ' ';
        res += stockReplies::toString(status_);
        res += "\r\n";
        // 响应头
        for (auto& h : headers_) {
            res += h.first;
            res += ": ";
            res += h.second;
            res += "\r\n";
        }
        // 分隔符
        res += "\r\n";
        // 响应体
        res += content_;
        return res;
    }


    void Response::json(Response& response, unsigned short status, const std::map<std::string, std::string>& data) {
        response.setStatus(status);
        response.setHeader("Content-Type", "text/json; charset=utf-8");
        // map转为json
        std::string content;
        web::Json::map2Json(data, content);
        response.setContent(content);
    }

    void Response::json(Response& response, unsigned short status, const std::vector<web::Transaction>& data) {
        response.setStatus(status);
        response.setHeader("Content-Type", "text/json; charset=utf-8");
        // vector转为json
        std::string content;
        web::Json::vector2Json(data, content);
        response.setContent(content);
    }

    void Response::json(Response& response, unsigned short status, const std::vector<web::User>& data) {
        response.setStatus(status);
        response.setHeader("Content-Type", "text/json; charset=utf-8");
        // vector转为json
        std::string content;
        web::Json::vector2Json(data, content);
        response.setContent(content);
    }


    void Response::setHeader(const std::string& name, const std::string& value) {
        headers_[name] = value;
    }

    void Response::setStatus(unsigned short status) {
        Response::status_ = status;
    }

    void Response::setContent(const std::string& content) {
        content_ = content;
    }

}