//
// Created by sxuer on 2021/5/9.
//

#ifndef payhttp_EpollConnection_HPP
#define payhttp_EpollConnection_HPP


#include <array>
#include <memory>
#include <set>
#include "Response.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"

namespace payhttp {
    class EpollConnectionManager;

    class EpollConnection : public std::enable_shared_from_this<EpollConnection> {
    private:
        // 当前request对象
        Request request_;

        // 客户端socket fd
        int cfd_;

        int epfd_;

        // 当前连接的处理器
        RequestHandler& requestHandler_;

        // 数据缓冲区
        std::array<char, 8192> buffer_;

        // 解析器
        RequestParser requestParser_;

        // response结果
        payhttp::Response response_;

        // 接收缓存，用于合并可能被分包的tcp数据流
        std::string receiveBuffer_;

        /**
         * 读取过程分为两步
         * 第一步分析请求头，第二步分析请求体
         * is_header_parsed用于记录是否已经解析过header信息
         */
        bool isHeaderParsed_{};

    public:
        EpollConnection(const EpollConnection&) = delete;

        EpollConnection& operator=(const EpollConnection&) = delete;

        explicit EpollConnection(int cfd, int epfd,
                                 RequestHandler& handler);

        // 开始本连接的所有操作
        void start();

        // 停止本连接的所有操作
        void stop() const;

    private:
        void doRead();

        void doWrite();

        int readCfdToBuffer(std::array<char, 8192>& buffer, int* errorCode) const;

        size_t writeToCfd(const std::string&, int* errorCode) const;
    };
}
#endif //payhttp_EpollConnection_HPP
