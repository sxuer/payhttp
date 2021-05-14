//
// Created by sxuer on 2021/5/9.
//

#include <vector>
#include "BaseController.hpp"
#include "web/src/service/UserService.hpp"
#include "web/src/service/TransactionService.hpp"
#include "web/core/RCode.hpp"

typedef payhttp::Response Response;
typedef payhttp::Request Request;

namespace web {

    std::map<std::string, Handler> BaseController::requestMap;

    void BaseController::app(const std::string& uri, Handler handler) {
        requestMap.insert(std::make_pair(uri, handler));
    }

    bool BaseController::handle() {

        // 登录
        app("/login", [](Request& request, payhttp::Response& response) {
            std::string username = request.getParam("username");
            std::string password = request.getParam("password");

            if (username.empty() || password.empty()) {
                return Response::html(response, payhttp::statusCode::bad_request, "用户名或密码为空");
            }

            User& loginUser = UserService::selectUserByUsername(username);
            if (loginUser == User::nullUser) {
                return Response::html(response, payhttp::statusCode::bad_request, "用户不存在");
            }
            if (!UserService::checkUsernameByPassword(username, password)) {
                return Response::html(response, payhttp::statusCode::bad_request, "密码错误");
            }

            // 登录成功，通知客户端保存cookie
            response.setHeader("Set-Cookie", "uid=" + std::to_string(loginUser.getId()));

            // 登录成功，跳转到首页
            response.setHeader("Location", "/home.html");
            Response::json(response, payhttp::statusCode::moved_temporarily,
                           {{"username", loginUser.getUsername()},
                            {"password", loginUser.getPassword()},
                            {"balance",  std::to_string(loginUser.getBalance())}
                           });
        });

        // 获取登录用户信息  根据用户id获取信息
        app("/user", [](Request& request, Response& response) {
            std::string uid = request.getCookie("uid");

            int id = std::stoi(uid);

            User& user = UserService::selectUserById(id);
            if (user == User::nullUser) {
                return Response::json(response, payhttp::statusCode::ok, {{"msg",  "登录失效"},
                                                                          {"code", "1001"}});
            }
            std::string& search = request.getParam("uid");

            if (search.empty()) {
                // 查询当前登录用户
                return Response::json(response, payhttp::statusCode::ok,
                                      {{"uid",      std::to_string(user.getId())},
                                       {"username", user.getUsername()},
                                       {"balance",  std::to_string(user.getBalance())}
                                      });
            } else {
                User& searchUser = UserService::selectUserById(stoi(search));
                if (searchUser != User::nullUser) {
                    return Response::json(response, payhttp::statusCode::ok,
                                          {{"uid",      std::to_string(searchUser.getId())},
                                           {"username", searchUser.getUsername()},
                                           {"balance",  std::to_string(searchUser.getBalance())}
                                          });
                }
            }

        });

        // 注册
        app("/register", [](Request& request, Response& response) {
            std::string username = request.getParam("username");
            std::string password = request.getParam("password");
            if (username.empty() || password.empty()) {
                return Response::html(response, payhttp::statusCode::bad_request, "用户名或密码为空");
            }
            if (UserService::selectUserByUsername(username) != User::nullUser) {
                return Response::html(response, payhttp::statusCode::bad_request, "用户名已存在");
            }

            int uid = UserService::insertUser(username, password);

            // TODO 并发访问测试
//            std::this_thread::sleep_for(std::chrono::seconds(10));

            // 注册成功，通知客户端保存cookie
            response.setHeader("Set-Cookie", "uid=" + std::to_string(uid));

            // 注册成功，跳转到首页
            response.setHeader("Location", "/home.html");
            Response::html(response, payhttp::statusCode::moved_temporarily, "注册成功");
        });

        // 注销
        app("/logout", [](Request& request, Response& response) {
            // TODO maxAge=0 立即删除cookie
            response.setHeader("Set-Cookie", "uid=" + std::to_string(-1));
            response.setHeader("Location", "/index.html");
            Response::html(response, payhttp::statusCode::moved_temporarily, "注销成功");
        });

        // 充值
        app("/recharge", [](Request& request, Response& response) {
            std::string uid = request.getCookie("uid");
            // 充值金额
            std::string total = request.getParam("total");
            if (uid.empty()) {
                return Response::html(response, payhttp::statusCode::bad_request, "登录信息失效，请重新登录");
            } else if (total.empty()) {
                return Response::html(response, payhttp::statusCode::bad_request, "转账金额有误，请重试");
            }
            // 添加金额
            UserService::transterBalance(stoi(uid), -1, stoi(uid), stol(total), " ");

            Response::json(response, payhttp::statusCode::ok,
                           {{"msg",   "充值成功"},
                            {"total", total}});
        });

        // 转账
        app("/transfer", [](Request& request, Response& response) {
            // 获取当前用户信息
            const std::string& uid = request.getCookie("uid");
            int id = stoi(uid);
            // 检查合法性
            if (!UserService::checkUserById(id)) {
                return Response::json(response, payhttp::statusCode::ok, {{"msg",  "登录失效"},
                                                                          {"code", std::to_string(RCode::LOGIN_FAIL)}});
            }
            // 获取转账信息
            // 对方账号   转账金额
            const std::string& relate = request.getParam("relate");
            const std::string& totals = request.getParam("total");
            if (relate.empty() || totals.empty()) {
                return Response::json(response, payhttp::statusCode::bad_request, {{"msg",  "数据非法"},
                                                                                   {"code", std::to_string(
                                                                                           RCode::PARAM_ERR)}});
            }
            if (relate == uid) {
                return Response::json(response, payhttp::statusCode::bad_request, {{"msg",  "不能给自己转账"},
                                                                                   {"code", std::to_string(
                                                                                           RCode::PARAM_ERR)}});
            }
            // 检查转出账户
            if (!UserService::checkUserById(stoi(relate))) {
                return Response::json(response, payhttp::statusCode::ok, {{"msg",  "转出对象不存在"},
                                                                          {"code", std::to_string(RCode::PARAM_ERR)}});
            }
            // 获得备注
            std::string& remark = request.getParam("remark");
            if (remark.empty()) {
                remark.push_back(' ');
            }
            // 如果字符串为空，这里会抛异常
            int targetId = std::stoi(relate);
            long total = std::stol(totals);

            // 开始转账
            bool res = UserService::transterBalance(id, id, targetId, total, remark);

            if (res) {
                return Response::json(response, payhttp::statusCode::ok, {{"msg", "操作成功"}});
            } else {
                return Response::json(response, payhttp::statusCode::ok, {{"msg",  "操作失败,余额不足"},
                                                                          {"code", std::to_string(
                                                                                  RCode::BALANCE_ERR)}});
            }
        });

        // 查看当前用户交易记录
        app("/transaction", [](Request& request, Response& response) {
            std::string uid = request.getCookie("uid");

            int id = std::stoi(uid);
            if (!UserService::checkUserById(id)) {
                return Response::json(response, payhttp::statusCode::ok, {{"msg",  "登录失效"},
                                                                          {"code", std::to_string(RCode::LOGIN_FAIL)}});
            }

            // 获取交易记录并返回
            std::vector<Transaction> trans = TransactionService::selectByUserId(id);

            return payhttp::Response::json(response, payhttp::statusCode::ok, trans);
        });

        // 查看其他所有用户
        app("/otheruser", [](Request& request, Response& response) {
            std::string uid = request.getCookie("uid");

            int id = std::stoi(uid);
            if (!UserService::checkUserById(id)) {
                return Response::json(response, payhttp::statusCode::ok, {{"msg",  "登录失效"},
                                                                          {"code", std::to_string(RCode::LOGIN_FAIL)}});
            }

            std::vector<User> users = UserService::selectUserExId(id);

            return payhttp::Response::json(response, payhttp::statusCode::ok, users);
        });

        return true;
    }
}

