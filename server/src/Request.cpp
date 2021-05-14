#include "server/include/Request.hpp"

namespace payhttp {

    std::string& Request::getParam(const std::string& param) {
        std::string& res = body_[param];
        // 优先从body中获取
        if (res.empty()) {
            res = uriParams_[param];
        }
        return res;
    }

    std::string& Request::getCookie(const std::string& cookie) {
        return cookies_[cookie];
    }
}