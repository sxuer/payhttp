#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "server/include/Request.hpp"
#include "server/include/Response.hpp"
#include "server/include/RequestHandler.hpp"
#include "web/src/controller/BaseController.hpp"

namespace payhttp {
    RequestHandler::RequestHandler(std::string docRoot)
            : docRoot_(std::move(docRoot)) {
    }

    void RequestHandler::requestDispatcher(Request& req, Response& rep) {
        // url解码
        std::string requestPath;
        if (!urlDecode(req.uri_, requestPath)) {
            rep = Response::stockResponse(statusCode::bad_request);
            return;
        }

        req.uri_ = requestPath;

        // 要求访问路径必须是绝对路径（'/'开头），且不包含..  std::string::npos,不存在的下标
        if (requestPath.empty() || requestPath[0] != '/'
            || requestPath.find("..") != std::string::npos) {
            rep = Response::stockResponse(statusCode::bad_request);
            return;
        }

        //解析uri参数
        int index = (int) req.uri_.find_first_of('?');
        if (index >= 0) {
            // 无uri参数的uri
            req.shortUri_ = req.uri_.substr(0, index);

            /**
             * 拿到?后面的字符串，然后根据uri参数的规则解析
             */
            std::string param_str = req.uri_.substr(index + 1, req.uri_.size());

            std::vector<std::string> split_result;
            boost::split(split_result, param_str, boost::is_any_of("&"));

            for (auto& i : split_result) {
                std::vector<std::string> split_result_temp;
                boost::split(split_result_temp, i, boost::is_any_of("="));
                if (split_result_temp.size() >= 2) {
                    // 原地构造 emplace_back
                    req.uriParams_.insert(make_pair(split_result_temp.at(0), split_result_temp.at(1)));
                }
            }
        } else {
            req.shortUri_ = req.uri_;
        }
        // 解析uri参数结束

        // 如果以'/'结尾，就加上index.html
        if (requestPath[requestPath.size() - 1] == '/') {
            requestPath += "index.html";
        }

        // 获得文件扩展名  xxx/a.c  找到最后一个.和/
        std::size_t lastSlashPos = requestPath.find_last_of('/');
        std::size_t lastDotPos = requestPath.find_last_of('.');
        std::string extension;
        if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos) {
            // 截取.之后的所有字符
            extension = requestPath.substr(lastDotPos + 1);
        }

        // 如果不是直接访问文件，则调用注册的控制器处理方法
        if (extension.empty()) {
            web::Handler h = web::BaseController::requestMap[req.shortUri_];
            if (h != nullptr) {
                h(const_cast<Request&>(req), rep);
            }
            // 如果rep状态不为零，说明已经处理过了，直接返回
            if (rep.status_ != 0) {
                return;
            }
        }

        // 打开文件，将数据加到响应体中
        std::string fullPath = docRoot_ + requestPath;
        std::ifstream is(fullPath.c_str(), std::ios::in | std::ios::binary);
        if (!is) {
            return Response::html(rep, statusCode::not_found, "页面不存在");
        }

        rep.status_ = statusCode::ok;
        char buf[512];
        rep.content_.clear();
        while (is.read(buf, sizeof(buf)).gcount() > 0)
            rep.content_.append(buf, is.gcount());
        rep.headers_["Content-Length"] = std::to_string(rep.content_.size());
        rep.headers_["Content-Type"] = mimeTypes::extensionToType(extension);
    }

/**
 * @param in 待解析url
 * @param out 传出参数，解析结果
 * @return 解析是否成功
 */
    bool RequestHandler::urlDecode(const std::string& in, std::string& out) {
        out.clear();
        // 补充内存至 >= in.size()
        out.reserve(in.size());
        // 字符串处理
        for (std::size_t i = 0; i < in.size(); ++i) {
            // 中文处理
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value = 0;
                    // 取出%后面的两位值
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out += static_cast<char>(value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }
}