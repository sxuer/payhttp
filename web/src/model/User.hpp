//
// Created by sxuer on 2021/5/9.
//

#ifndef PAYHTTP_USER_HPP
#define PAYHTTP_USER_HPP

#include <ostream>

namespace web {
    class User {
        int id_;
        std::string username_;
        std::string password_;

        // 余额 单位：分
        long balance_;

    public:
        static User nullUser;

        User();

        User(std::string& username, std::string& password);

        User(std::string& username, std::string& password, int id);

        User(std::string& username, std::string& password, int id, long balance);

        int getId() const;

        void setId(int id);

        const std::string& getUsername() const;

        void setUsername(const std::string& username);

        const std::string& getPassword() const;

        void setPassword(const std::string& password);

        bool operator==(const User& rhs) const;

        bool operator!=(const User& rhs) const;

        long getBalance() const;

        void setBalance(long balance);

        friend std::ostream& operator<<(std::ostream& os, const User& user);
    };
}

#endif //PAYHTTP_USER_HPP
