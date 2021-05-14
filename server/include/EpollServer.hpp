//
// Created by sxuer on 2021/5/10.
//

#ifndef PAYHTTP_EPOLLSERVER_HPP
#define PAYHTTP_EPOLLSERVER_HPP

#include <string>
#include <thread>
#include "EpollConnection.hpp"
#include <vector>
#include "EpollConnectionManager.hpp"
#include "RequestHandler.hpp"

namespace payhttp {
    class EpollServer {

    private:
        // 服务端socket
        static int sfd_;
        // ep红黑树
        static int epfd_;

        static std::string docRoot_;

    public:
        EpollServer(const EpollServer&) = delete;

        EpollServer& operator=(const EpollServer&) = delete;

        explicit EpollServer(const std::string& address,
                             const std::string& port,
                             const std::string& docRoot);

        static int getSfd();

        static int getEpfd();

        // 统计客户端连接
//        static std::thread sumer_;
    private:
        /// 异步accept
        static void doAccept();

        static void doRead(int);

        // 初始化sfd
        static int initListenFd(int port, int epfd);

    };
}


#endif //PAYHTTP_EPOLLSERVER_HPP
