//
// Created by sxuer on 2021/5/5.
//

#ifndef payhttp_REQUEST_HPP
#define payhttp_REQUEST_HPP

#include <string>
#include <vector>
#include <map>
#include "Header.hpp"

namespace payhttp {
// 请求对象
    class Request {
    public:
        std::string method_;

        int httpVersionMajor_;

        int httpVersionMinor_;

        std::vector<Header> headers_;

        // 完整uri
        std::string uri_;

        // 去掉?后面的参数的uri
        std::string shortUri_;

        //uri参数
        std::map<std::string, std::string> uriParams_;

        std::map<std::string, std::string> cookies_;

        // 暂存JSON中的键值对
        std::string key_;
        std::string value_;

        // json/form参数
        std::map<std::string, std::string> body_;

        std::string& getParam(const std::string&);

        std::string& getCookie(const std::string& cookie);

    };
}

#endif //payhttp_REQUEST_HPP
