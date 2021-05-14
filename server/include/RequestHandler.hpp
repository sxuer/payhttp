//
// Created by sxuer on 2021/5/5.
//

#ifndef payhttp_REQUESTHANDLER_HPP
#define payhttp_REQUESTHANDLER_HPP

#include <string>
#include <utility>
#include <fstream>
#include "Request.hpp"
#include "Response.hpp"

namespace payhttp {
    class RequestHandler {
    private:
        // 服务路径
        std::string docRoot_;

    public:
        RequestHandler(const RequestHandler&) = delete;

        RequestHandler& operator=(const RequestHandler&) = delete;

        // 使用文件根目录初始化（服务路径）
        explicit RequestHandler(std::string docRoot);

        void requestDispatcher(Request& req, payhttp::Response& rep);


    private:
        /// url解码
        static bool urlDecode(const std::string& in, std::string& out);
    };
}
#endif //payhttp_REQUESTHANDLER_HPP
