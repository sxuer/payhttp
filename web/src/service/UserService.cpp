#include "UserService.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include "web/src/dao/UserDao.hpp"
#include "web/core/GlobalAutoIds.hpp"
#include "web/src/service/TransactionService.hpp"

namespace web {

    User& UserService::selectUserByUsername(const std::string& username) {
        for (const auto& user : UserDao::userDatabase_) {
            const std::string& uname = user.second.getUsername();
            if (uname == username) {
                return const_cast<User&>(user.second);
            }
        }
        return User::nullUser;
    }

    bool UserService::checkUsernameByPassword(const std::string& username, const std::string& password) {
        User& user = selectUserByUsername(username);
        if (user != User::nullUser) {
            return password == user.getPassword();
        }
        return false;
    }

    User& UserService::selectUserById(const int id) {
        // 不能直接使用UserService::userDatabase_[id]; 如果不存在，会返回一个随机对象
        return UserDao::userDatabase_.count(id) == 0 ? User::nullUser : UserDao::userDatabase_[id];
    }

    int UserService::insertUser(std::string& username, std::string& password) {
        User user(username, password, GlobalAutoIds::getId("user"), 0);
        UserDao::userDatabase_.insert(std::make_pair(user.getId(), user));
        return user.getId();
    }

    bool UserService::transterBalance(int uid, int from, int to, long total, const std::string& remark) {
        User& fromUser = selectUserById(from);
        User& toUser = selectUserById(to);

        int type = 0;

        // 转账对端，默认为系统
        int relate = -1;

        // 系统充值
        if (from == -1) {
            type = Transaction::Type::RECHARGE;
            // TODO 并发
            if (toUser != User::nullUser) {
                toUser.setBalance(toUser.getBalance() + total);
            }
        } else if (to == -1) { // 消费（虽然暂时么有这个功能）
            type = Transaction::Type::COMSUME;
            // TODO 并发
            if (fromUser != User::nullUser) {
                fromUser.setBalance(fromUser.getBalance() - total);
            }
        } else {  // 用户间的转账
            if (fromUser.getBalance() < total) {
                return false;
            }
            relate = uid == from ? to : from;
            // 当前用户为from，则为转出
            type = uid == from ? Transaction::Type::OUT : Transaction::Type::IN;
            if (fromUser != User::nullUser && toUser != User::nullUser) {
                // TODO 并发
                fromUser.setBalance(fromUser.getBalance() - total);
                toUser.setBalance(toUser.getBalance() + total);
            }
        }
        // 保存记录
        // 保存记录 充值
        const Transaction& transaction =
                Transaction(GlobalAutoIds::getId("trans"), uid, type, std::time(nullptr), total, relate, remark);
        TransactionService::saveRecord(transaction);
        return true;
    }

    bool UserService::checkUserById(const std::string& sid) {
        int id = std::stoi(sid);
        return checkUserById(id);
    }

    bool UserService::checkUserById(int id) {
        return selectUserById(id) != User::nullUser;
    }

    std::vector<User> UserService::selectUserExId(int uid) {
        std::vector<User> res;
        for (const auto& user : UserDao::userDatabase_) {
            if (user.first != uid) {
                res.emplace_back(user.second);
            }
        }
        return res;
    }
}