#include "User.hpp"
#include <string>
namespace web {

    User User::nullUser;

    User::User() : balance_(0) {};

    User::User(std::string& username, std::string& password) : username_(username), password_(password) {}

    User::User(std::string& username, std::string& password, int id) : username_(username), password_(password),
                                                                        id_(id) {}

    User::User(std::string& username, std::string& password, int id, long balance) : username_(username),
                                                                                      password_(password),
                                                                                      id_(id), balance_(balance) {}


    long User::getBalance() const {
        return balance_;
    }

    int User::getId() const {
        return id_;
    }

    void User::setId(int id) {
        User::id_ = id;
    }

    const std::string& User::getUsername() const {
        return username_;
    }

    void User::setUsername(const std::string& username) {
        User::username_ = username;
    }

    const std::string& User::getPassword() const {
        return password_;
    }

    void User::setPassword(const std::string& password) {
        User::password_ = password;
    }

    bool User::operator==(const User& rhs) const {
        return id_ == rhs.id_ &&
               username_ == rhs.username_ &&
               password_ == rhs.password_;
    }

    bool User::operator!=(const User& rhs) const {
        return !(rhs == *this);
    }

    void User::setBalance(long balance) {
        balance_ = balance;
    }

    std::ostream& operator<<(std::ostream& os, const User& user) {
        os << "id: " << user.id_ << " username: " << user.username_ << " password: " << user.password_
           << " balance: " << user.balance_;
        return os;
    }
}
