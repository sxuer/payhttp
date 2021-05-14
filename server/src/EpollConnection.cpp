#include <array>
#include <memory>
#include <thread>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>

#include "server/include/Response.hpp"
#include "server/include/Request.hpp"
#include "server/include/RequestHandler.hpp"
#include "server/include/RequestParser.hpp"

#include "server/include/EpollConnection.hpp"
#include "server/include/EpollConnectionManager.hpp"

namespace payhttp {


    EpollConnection::EpollConnection(int cfd, int epfd, RequestHandler& handler)
            : cfd_(cfd),
              epfd_(epfd),
              requestHandler_(handler),
              buffer_{} {
    }

    void EpollConnection::start() {
        isHeaderParsed_ = false;
        doRead();
    }

    void EpollConnection::stop() const {
        int flag = 0;
        // 从epoll上取下
        if (epoll_ctl(epfd_, EPOLL_CTL_DEL, cfd_, nullptr) == -1) {
            perror("close() cfd from epoll tree error, after 3 seconds try again...");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            flag = epoll_ctl(epfd_, EPOLL_CTL_DEL, cfd_, nullptr);
            std::cout << "close() cfd from epoll tree return value: " << flag << std::endl;
        }
        // 关闭文件描述符
        if (close(cfd_) == -1) {
            perror("close() cfd error, after 3 seconds try again...");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            flag = close(cfd_);
            std::cout << "close() cfd return value: " << flag << std::endl;
        }
    }

    void EpollConnection::doRead() {
        int errorCode = 0;
        // 从cfd中读取
        int bytesTransferred = readCfdToBuffer(buffer_, &errorCode);

        if (errorCode == 0) {
            // 分包逻辑处理
            // 获得buffer中的所有数据，读取长度为所记录的bytes_transferred
            std::string this_data(buffer_.data(), bytesTransferred);
            receiveBuffer_.append(this_data);

            //http协议的头部结束标识
            int header_end = static_cast<int>(receiveBuffer_.find("\r\n\r\n"));
            // 如果没有读到请求头末尾，则继续读
            if (header_end < 0) {
                doRead();
                return;
            }

            RequestParser::resultType result;
            // 如果header信息还未被解析过，则进行解析
            if (!isHeaderParsed_) {
                std::tie(result, std::ignore) = requestParser_.parse(
                        request_, receiveBuffer_.begin(), receiveBuffer_.end());
            }

            // 如果header信息已经解析成功，开始读取请求体
            if (isHeaderParsed_ || result == RequestParser::good) {
                // 重置标志位
                isHeaderParsed_ = true;

                // 找到客户端数据包实际长度信息
                std::string length_str;
                for (auto& header : request_.headers_) {
                    if (header.name_ == "Content-Length") {
                        length_str = header.value_;
                        break;
                    }
                }

                // 记录客户端数据包实际长度
                int content_length = 0;
                if (!length_str.empty()) {
                    content_length = atoi(length_str.c_str());
                }

                if (receiveBuffer_.size() < (content_length + header_end + 4)) {
                    doRead();
                    return;
                }

                // 解析cookie
                requestParser_.parseCookie(request_);

                // 解析参数
                requestParser_.parseParam(request_, receiveBuffer_);

                // 核心逻辑，利用request信息构造response_对象，随后在write中写回客户端
                requestHandler_.requestDispatcher(request_, response_);

                doWrite();

            } else if (result == RequestParser::bad) {
                response_ = Response::stockResponse(statusCode::bad_request);
                doWrite();
            } else {
                doRead();
            }
        } else {
            perror("ERROR: doRead()");
        }
    }


    void EpollConnection::doWrite() {
        int errorCode = 0;
        // 把response_写到cfd中
        writeToCfd(response_.toString(), &errorCode);

        // 写完回调，做错误处理
        if (errorCode != 0) {
            perror("doWrite()");
        }
    }

    int EpollConnection::readCfdToBuffer(std::array<char, 8192>& buffer, int* errorCode) const {
        // 初始化无错误
        *errorCode = 0;

        int size = buffer.size();
        // 已阅读字节数
        int i = 0;

        ssize_t flag;
        char c = '\0';
        while (i < size - 1) {
            // 读取1个字节
            flag = recv(cfd_, &c, 1, 0);
            buffer[i] = c;
            // 读取成功
            if (flag > 0) {
                // 后移指针
                ++i;
                // 检查是否是末尾
                if (c == '\r') {
                    // 预读1字节
                    flag = recv(cfd_, &c, 1, MSG_PEEK);
                    // 如果是末尾，就真的读一字节
                    if (flag > 0 && c == '\n') {
                        recv(cfd_, &c, 1, 0);
                        // 指针后移，一行读取完毕，结束
                        buffer[i] = '\n';
                        ++i;
                        break;
                    }
                }
            }
        }
        return i;
    }

    size_t EpollConnection::writeToCfd(const std::string& buffer, int* errorCode) const {
        // 初始化无错误
        *errorCode = 0;

        // 应该写出的字节数
        size_t buffersLen = 0;
        // 实际写出的字节数
        size_t bytesTransferred = 0;

        ssize_t flag;

        size_t bufferSize = buffer.size();
        flag = send(cfd_, buffer.c_str(), bufferSize, 0);
        if (flag == -1) {
            *errorCode = errno;
            return bytesTransferred;
        }
        buffersLen += bufferSize;
        bytesTransferred += flag;
        // 写出错误
        if (buffersLen != bytesTransferred) {
            *errorCode = -1;
        }
        return bytesTransferred;
    }

}