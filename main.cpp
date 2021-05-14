#include <iostream>
#include <web/src/dao/UserDao.hpp>
#include <web/src/dao/TransactionDao.hpp>
#include <web/src/dao/BaseDao.hpp>
#include <web/src/controller/BaseController.hpp>
#include <server/include/EpollServer.hpp>

void init(const std::string& databaseRoot) {
    // 加载数据
    try {
        web::BaseDao::databaseRoot = databaseRoot;
        web::UserDao::loadDatabase(web::UserDao::userDatabase_);
        web::TransactionDao::loadDatabase(web::TransactionDao::transDatabase_);
    } catch (std::exception& e) {
        std::cerr << "load database Error! Please check your Database. (or rm -r <your database>)" << std::endl;
        exit(1);
    }
    // 加载控制器
    try {
        web::BaseController::handle();
    } catch (std::exception& e) {
        std::cerr << "load Web Controller Error! Sorry about that, please try to debug. " << std::endl;
        exit(1);
    }

}


void epoll_http(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            std::cerr << "Usage:: <address> <port> <doc_root> <database_root>" << std::endl;
            return;
        }
        init(argv[4]);
        payhttp::EpollServer server(argv[1], argv[2], argv[3]);

    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}


int main(int argc, char* argv[]) {
    epoll_http(argc, argv);

    return 0;
}
