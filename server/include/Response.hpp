
#ifndef payhttp_response_HPP
#define payhttp_response_HPP

#include <string>
#include <vector>
#include "Constant.hpp"
#include <map>
#include  "web/src/model/Transaction.hpp"
#include  "web/src/model/User.hpp"

namespace payhttp {
    class Response {
    public:
        // 响应状态  200 ok
        unsigned short status_;

        // 响应头
        std::map<std::string, std::string> headers_;

        // 响应体
        std::string content_;

        // 是否已经处理过
        bool handled_;

        // 转为string类型的响应体
        std::string toString();

        // 设置响应头
        void setHeader(const std::string& name, const std::string& value);

        // 获取 stock Response.
        static Response stockResponse(unsigned short status);

        // 返回html页面  前后耦合
        static void html(Response&, unsigned short status, const std::string& msg);

        // 返回json对象  前后分离
        static void json(Response&, unsigned short status, const std::map<std::string, std::string>& data);

        // TODO 本想写个特化版本，但是不知为何一直undefined reference  (原因：泛型的声明和定义都需要一起放在hpp中)
        static void json(Response& response, unsigned short status, const std::vector<web::Transaction>& data);

        static void json(Response& response, unsigned short status, const std::vector<web::User>& data);

        void setStatus(unsigned short status);

        void setContent(const std::string& content);

    };
}

#endif //payhttp_response_HPP
