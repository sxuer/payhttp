//
// Created by sxuer on 2021/5/10.
//

#ifndef PAYHTTP_USERDAO_HPP
#define PAYHTTP_USERDAO_HPP

#include <web/src/model/User.hpp>
#include <map>

namespace web {
    class UserDao {
    public:
        UserDao() = delete;

        // 存储用户信息 id_:<usrname:pwd>
        static std::map<int, User> userDatabase_;

        // 启动时加载数据
        static bool loadDatabase(std::map<int, User>&);

        // 结束时保存数据
        static void saveDatabase();
    };
}


#endif //PAYHTTP_USERDAO_HPP
