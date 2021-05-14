//
// Created by sxuer on 2021/5/9.
//

#ifndef payhttp_EpollConnectionMANAGER_HPP
#define payhttp_EpollConnectionMANAGER_HPP


#include <set>
#include <mutex>
#include "EpollConnection.hpp"

/**
 * 连接管理器：用于创建链接、停止连接
 */

namespace payhttp {
    class EpollConnection;

    class EpollConnectionManager {
    private:
        static std::set<std::shared_ptr<EpollConnection>> epollConnections_;
        static std::mutex EpollConnectionsMutex_;

    public:

        EpollConnectionManager(const EpollConnectionManager&) = delete;

        EpollConnectionManager& operator=(const EpollConnectionManager&) = delete;

        EpollConnectionManager() = delete;

        // 新增一个连接，并start
        static void start(const std::shared_ptr<EpollConnection>& c);

        // 停止一个连接
        static void stop(const std::shared_ptr<EpollConnection>& c);

        static void stopAll();

        static unsigned long getEpollConnectionSize();
    };
}
#endif //payhttp_EpollConnectionMANAGER_HPP
