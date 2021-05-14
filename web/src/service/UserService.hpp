//
// Created by sxuer on 2021/5/9.
//

#ifndef PAYHTTP_USERSERVICE_HPP
#define PAYHTTP_USERSERVICE_HPP

#include <web/src/model/User.hpp>
#include <map>
#include <vector>

namespace web {
    class UserService {
    public:
        UserService() = delete;

        static User& selectUserByUsername(const std::string& username);

        static User& selectUserById(int id);

        static bool checkUsernameByPassword(const std::string& username, const std::string& password);

        // 返回用户id
        static int insertUser(std::string& username, std::string& password);

        /**
         * @param from 金钱来源uid   from=-1表示系统
         * @param to 金钱走向uid   to=-1表示系统
         * @param total 交易金额  from-total  to+total
         */
        static bool transterBalance(int uid, int from, int to, long total, const std::string& remark);

        // 检查用户是否存在
        static bool checkUserById(const std::string& id);

        static bool checkUserById(int id);

        static std::vector<User> selectUserExId(int uid);
    };

}
#endif //PAYHTTP_USERSERVICE_HPP
