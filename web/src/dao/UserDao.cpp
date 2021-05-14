//
// Created by sxuer on 2021/5/10.
//

#include "UserDao.hpp"
#include "BaseDao.hpp"
#include <web/core/GlobalAutoIds.hpp>
#include <iostream>
#include <fstream>

namespace web {

    using namespace std;

    std::map<int, User> UserDao::userDatabase_;

    bool UserDao::loadDatabase(std::map<int, User>& userDatas) {
        // 重要，不然可能存在一条不规则的初始化数据
        userDatas.clear();
        // 记录数据条数
        int i = 0;

        fstream in(BaseDao::databaseRoot + "/user.dat", ios::in);
        int id;
        std::string username;
        std::string password;
        long balance;
        if (in.is_open()) {
            while (!in.eof()) {
                in >> id >> username >> password >> balance;
                userDatas.insert(make_pair(id, web::User(username, password, id, balance)));
                ++i;
            }
        }
        // 初始化全局自增ID
        GlobalAutoIds::init("user", i);
        return true;
    }

    /**
     * TODO  密码加密
     * @param userDatas 
     */
    void UserDao::saveDatabase() {
        fstream out(BaseDao::databaseRoot + "/user.dat", ios::out);
        if (out.is_open()) {
            for (const auto& item : userDatabase_) {
                std::cout << item.second << endl;
                out << item.first << '\t';
                out << item.second.getUsername() << '\t';
                out << item.second.getPassword() << '\t';
                out << item.second.getBalance() << '\t';
                out << endl;
            }
            out.close();
        }
    }

}