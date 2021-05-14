//
// Created by sxuer on 2021/5/9.
//

#ifndef PAYHTTP_BASECONTROLLER_HPP
#define PAYHTTP_BASECONTROLLER_HPP

#include<map>
#include "web/src/model/Transaction.hpp"
#include "server/include/Response.hpp"
#include "server/include/RequestHandler.hpp"

namespace web {

    // 控制器中的处理函数
    typedef void (* Handler)(payhttp::Request&, payhttp::Response&);

    class BaseController {
    public:
        static bool handle();

        // 存储控制器的方法处理路径  uri:处理函数指针
        static std::map<std::string, Handler> requestMap;

        static void app(const std::string& uri, Handler handler);
    };
}

#endif //PAYHTTP_BASECONTROLLER_HPP
