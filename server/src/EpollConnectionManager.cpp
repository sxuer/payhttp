#include <set>
#include <mutex>
#include <server/include/EpollConnectionManager.hpp>

/**
 * 连接管理器：用于创建链接、停止连接
 */

namespace payhttp {

    std::set<std::shared_ptr<EpollConnection>> EpollConnectionManager::epollConnections_;
    std::mutex EpollConnectionManager::EpollConnectionsMutex_;

    void EpollConnectionManager::start(const std::shared_ptr<EpollConnection>& c) {
        // 添加到集合中，并启动
        epollConnections_.insert(c);
        c->start();
    }

    void EpollConnectionManager::stop(const std::shared_ptr<EpollConnection>& c) {
        // 从集合中移除，并停止
        epollConnections_.erase(c);
        c->stop();
    }

    void EpollConnectionManager::stopAll() {
        // 停止所有，清空容器
        for (const auto& c: epollConnections_)
            c->stop();
        epollConnections_.clear();
    }

    unsigned long EpollConnectionManager::getEpollConnectionSize() {
        const std::lock_guard<std::mutex> lock(EpollConnectionsMutex_);
        return epollConnections_.size();
    }
}