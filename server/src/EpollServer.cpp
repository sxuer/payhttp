//
// Created by sxuer on 2021/5/10.
//

#include <thread>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sys/timeb.h>
#include <web/src/dao/UserDao.hpp>
#include <web/src/dao/TransactionDao.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <csignal>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include "server/include/EpollServer.hpp"

// epoll监听个数
#define MAXSIZE 512

// 大于
#define GT 0
// 小于
#define LT -1
// 等于
#define EQ 1
// 不等于
#define NEQ 2

using namespace std;

/**
 * 如果(target) cmp (negative) 成立，抛出str错误
 */
void returnCheckCmp(int target, int cmp, int negative, const string& str) {
    int res;
    if (cmp == LT) {
        res = target < negative;
    } else if (cmp == EQ) {
        res = target == negative;
    } else if (cmp == GT) {
        res = target > negative;
    } else {
        res = target != negative;
    }
    if (res) {
        perror(str.c_str());
        exit(1);
    }
}


/**
 * 如果target==negative，抛出str错误
 */
void returnCheck(int target, int negative, const string& str) {
    returnCheckCmp(target, EQ, negative, str);
}


void quit(int eno) {
    //////////////////////关闭所有客户端连接//////////////////////////////
    std::cout << "\nclose all cilent fd..." << std::endl;
    payhttp::EpollConnectionManager::stopAll();

    //////////////////////////保存数据///////////////////////////////////
    std::cout << "\nsave data..." << std::endl;
    std::cout << "save users..." << std::endl;
    web::UserDao::saveDatabase();
    std::cout << "save transactions..." << std::endl;
    web::TransactionDao::saveDatabase();

    //////////////////////////回收epoll//////////////////////////////////
    std::cout << "\nshutdown epoll..." << std::endl;

    int epfd = payhttp::EpollServer::getEpfd();
    int sfd = payhttp::EpollServer::getSfd();

    int flag = 0;
    // 从epoll上取下
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, sfd, nullptr) == -1) {
        std::cout << "sfd: " << sfd << std::endl;
        perror("close() sfd_ from epoll tree error, after 3 seconds try again...");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        flag = epoll_ctl(epfd, EPOLL_CTL_DEL, sfd, nullptr);
        std::cout << "close() sfd_ from epoll tree return value: " << flag << std::endl;
    }
    // 关闭文件描述符
    if (close(sfd) == -1) {
        perror("close() sfd_ error, after 3 seconds try again...");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        flag = close(sfd);
        std::cout << "close() sfd_ return value: " << flag << std::endl;
    }

    // 关闭epoll
    if (close(epfd) == -1) {
        std::cout << "epfd: " << epfd << std::endl;
        perror("close() epfd_ error, after 3 seconds try again...");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        flag = close(epfd);
        std::cout << "close() epfd_ return value: " << flag << std::endl;
    }

    // 回收统计线程
//    if (payhttp::EpollServer::sumer_.joinable())
//        payhttp::EpollServer::sumer_.join();

    std::cout << "\nCollection Finished!" << std::endl;
    exit(eno);
}

void quitRegister() {
    signal(SIGINT, quit);
    signal(SIGTERM, quit);
#if defined(SIGQUIT)
    signal(SIGQUIT, quit);
#endif // defined(SIGQUIT)
}

namespace payhttp {
    int EpollServer::sfd_;
    int EpollServer::epfd_;
    std::string EpollServer::docRoot_;
//    std::thread EpollServer::sumer_([]() {
//        // 定时统计连接个数
//        while (true) {
//            std::cout << "EpollConnection nums:" << EpollConnectionManager::getEpollConnectionSize()
//                      << std::endl;
//            std::this_thread::sleep_for(std::chrono::seconds(10));
//        }
//    });

    EpollServer::EpollServer(const std::string& address,
                             const std::string& port,
                             const std::string& docRoot) {
        quitRegister();

        EpollServer::docRoot_ = docRoot;

        // 创建红黑树
        epfd_ = epoll_create(MAXSIZE);
        returnCheck(epfd_, -1, "epoll_create error");

        // 创建服务端socket文件描述符sfd，添加至监听树
        sfd_ = initListenFd(stoi(port), epfd_);
        while (1) {
            // 传出参数，保存客户端事件
            struct epoll_event all_events[MAXSIZE];

            // 监听节点事件，timeout=-1表示阻塞式
            int target = epoll_wait(epfd_, all_events, MAXSIZE, -1);

            // epoll返回值检查，仅仅是中断则继续执行
            if (target < 0) {
                if (errno == EINTR) continue;
                else returnCheck(target, -1, "epoll_wait error");
            }

            for (int i = 0; i < target; ++i) {
                // 对all_events[i]的操作可能会改变其值，因此用一个tmpev操作，避免修改原数据
                struct epoll_event* tmpev = &all_events[i];
                // 只监听读事件,不是读事件，跳过
                if (!(tmpev->events & EPOLLIN)) {
                    continue;
                }
                // 如果是服务端socket，进行accept
                if (tmpev->data.fd == sfd_) {
                    doAccept();
                } else { // 否则就是客户端socket，读取数据
                    std::thread([&]() {

//                        pthread_setspecific(1, new Request);
//                        pthread_setspecific(2, new Response);

                        doRead(tmpev->data.fd);
                    }).detach();
                }
            }
        }
    }


// 初始化server fd，并挂载到epfd上
    int EpollServer::initListenFd(int port, int epfd) {
        // 创建TCP socket
        int sfd = socket(AF_INET, SOCK_STREAM, 0);
        returnCheck(sfd, -1, "socket error");

        // 创建服务器地址结构
        struct sockaddr_in server_addr;
        // 初始化
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        // ip地址自动分配
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        // 设置端口复用，避免服务器重启再打开的时候因为time wait提示端口占用
        int opt = 1;
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // 绑定地址结构到socket上
        returnCheck(
                bind(sfd, (const struct sockaddr*) &server_addr, sizeof(server_addr)), 1,
                "bind error");

        // 设置监听上限
        returnCheck(
                listen(sfd, MAXSIZE), -1,
                "listen error");

        // sfd添加到epoll树上
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sfd;

        returnCheck(
                epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev), -1,
                "epoll_ctl error");

        return sfd;
    }

    void EpollServer::doAccept() {
        // 客户端数据，传出参数
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int cfd = accept(sfd_, (struct sockaddr*) &client_addr, &client_len);

        returnCheck(cfd, -1, "accept error");

        char client_ip[64] = {0};
        std::cout << "Client IP: " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof client_ip)
                  << ", Port: " << ntohs(client_addr.sin_port) << ", cfd: " << cfd << "\n" << std::endl;

        // 设置cfd非阻塞，采用ET模式，一次请求的数据只触发一次epoll_wait
        int flag = fcntl(cfd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(cfd, F_SETFL, flag);

        // 挂载epoll
        struct epoll_event ev;
        ev.data.fd = cfd;
        // ET模式
        ev.events = EPOLLIN | EPOLLET;

        // 挂到epoll树上
        returnCheck(
                epoll_ctl(epfd_, EPOLL_CTL_ADD, cfd, &ev), -1,
                "epoll_ctl error");
    }

    void EpollServer::doRead(int cfd) {
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        RequestHandler handler(EpollServer::docRoot_);
        const shared_ptr<EpollConnection>& client = std::make_shared<EpollConnection>(cfd, epfd_, handler);
        // 启动读-写处理
        payhttp::EpollConnectionManager::start(client);
        // 处理完后关闭cfd
        payhttp::EpollConnectionManager::stop(client);
    }

    int EpollServer::getSfd() {
        return sfd_;
    }

    int EpollServer::getEpfd() {
        return epfd_;
    }

}