#include <tuple>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "server/include/RequestParser.hpp"

namespace payhttp {
// 默认构造状态：method_start
    RequestParser::RequestParser()
            : state_(method_start) {
    }

// 重置状态
    void RequestParser::reset() {
        state_ = method_start;
    }

// 自动状态机 挨个解析字符，得到一个最终结果
    RequestParser::resultType RequestParser::consume(Request& req, char input) {
        switch (state_) {
            case method_start:
                // 如果不是char或者是非法char
                if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                    return bad;
                    // 否则跳转到method判断
                } else {
                    state_ = method;
                    // 尾部追加char
                    req.method_.push_back(input);
                    return indeterminate;
                }
            case method: // 方法解析完毕会遇到一个' '，开始解析uri
                if (input == ' ') {
                    state_ = uri;
                    return indeterminate;
                } else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                    return bad;
                } else {
                    req.method_.push_back(input);
                    return indeterminate;
                }
            case uri: // uri解析完毕遇到' '，开始解析http版本号
                if (input == ' ') {
                    state_ = http_version_h;
                    return indeterminate;
                } else if (isCtl(input)) {
                    return bad;
                } else {
                    req.uri_.push_back(input);
                    return indeterminate;
                }
            case http_version_h:
                if (input == 'H') {
                    state_ = http_version_t_1;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_t_1:
                if (input == 'T') {
                    state_ = http_version_t_2;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_t_2:
                if (input == 'T') {
                    state_ = http_version_p;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_p:
                if (input == 'P') {
                    state_ = http_version_slash;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_slash:
                if (input == '/') {
                    req.httpVersionMajor_ = 0;
                    req.httpVersionMinor_ = 0;
                    state_ = http_version_major_start;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_major_start:
                if (isDigit(input)) {
                    req.httpVersionMajor_ = req.httpVersionMajor_ * 10 + input - '0';
                    state_ = http_version_major;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_major:
                if (input == '.') {
                    state_ = http_version_minor_start;
                    return indeterminate;
                } else if (isDigit(input)) {
                    req.httpVersionMajor_ = req.httpVersionMajor_ * 10 + input - '0';
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_minor_start:
                if (isDigit(input)) {
                    req.httpVersionMinor_ = req.httpVersionMinor_ * 10 + input - '0';
                    state_ = http_version_minor;
                    return indeterminate;
                } else {
                    return bad;
                }
            case http_version_minor:
                if (input == '\r') {
                    state_ = expecting_newline_1;
                    return indeterminate;
                } else if (isDigit(input)) {
                    req.httpVersionMinor_ = req.httpVersionMinor_ * 10 + input - '0';
                    return indeterminate;
                } else {
                    return bad;
                }
            case expecting_newline_1:
                if (input == '\n') {
                    state_ = header_line_start;
                    return indeterminate;
                } else {
                    return bad;
                }
            case header_line_start:
                if (input == '\r') {
                    state_ = expecting_newline_3;
                    return indeterminate;
                } else if (!req.headers_.empty() && (input == ' ' || input == '\t')) {
                    state_ = header_lws;
                    return indeterminate;
                } else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                    return bad;
                } else {
                    req.headers_.emplace_back(Header());
                    req.headers_.back().name_.push_back(input);
                    state_ = header_name;
                    return indeterminate;
                }
            case header_lws:
                if (input == '\r') {
                    state_ = expecting_newline_2;
                    return indeterminate;
                } else if (input == ' ' || input == '\t') {
                    return indeterminate;
                } else if (isCtl(input)) {
                    return bad;
                } else {
                    state_ = header_value;
                    req.headers_.back().value_.push_back(input);
                    return indeterminate;
                }
            case header_name:
                if (input == ':') {
                    state_ = space_before_header_value;
                    return indeterminate;
                } else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                    return bad;
                } else {
                    req.headers_.back().name_.push_back(input);
                    return indeterminate;
                }
            case space_before_header_value:
                if (input == ' ') {
                    state_ = header_value;
                    return indeterminate;
                } else {
                    return bad;
                }
            case header_value:
                if (input == '\r') {
                    state_ = expecting_newline_2;
                    return indeterminate;
                } else if (isCtl(input)) {
                    return bad;
                } else {
                    req.headers_.back().value_.push_back(input);
                    return indeterminate;
                }
            case expecting_newline_2:
                if (input == '\n') {
                    state_ = header_line_start;
                    return indeterminate;
                } else {
                    return bad;
                }
            case expecting_newline_3:
                return (input == '\n') ? good : bad;
            case json_parse_start:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == '\n') {
                    state_ = json_key_left_dot;
                    return indeterminate;
                } else {
                    return bad;
                }
            case json_key_left_dot:
                if (input == ' ') {
                    return indeterminate;
                }
//                开始key扫描之前，先加入到body中
                if (!req.key_.empty()) {
                    req.body_.insert(make_pair(req.key_, req.value_));
                    req.key_.clear();
                    req.value_.clear();
                }
                if (input == '"') {
                    state_ = json_key;
                    return indeterminate;
                } else {
                    return bad;
                }
            case json_key:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == '"') {
                    state_ = json_key_right_dot;
                    return indeterminate;
                } else {
                    // 接收所有输入值
                    req.key_.push_back(input);
                    return indeterminate;
                }
            case json_key_right_dot:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == '"' || input == ':') {
                    state_ = json_colon;
                    return indeterminate;
                } else {
                    return bad;
                }
            case json_colon:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == ':') {
                    state_ = json_value_start;
                    return indeterminate;
                } else if (input == '"') {
                    state_ = json_value_start;
                    return indeterminate;
                } else {
                    // 接收所有输入
                    state_ = json_value;
                    req.value_.push_back(input);
                    return indeterminate;
                }
            case json_value_start:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == '"') {
                    state_ = json_value_left_dot;
                    return indeterminate;
                } else {
                    // 接收所有输入
                    state_ = json_value;
                    req.value_.push_back(input);
                    return indeterminate;
                }
            case json_value_left_dot:
                if (input == ' ') {
                    return indeterminate;
                }
                state_ = json_value;
                if (input == '"') {
                    return indeterminate;
                } else {
                    return bad;
                }
            case json_value:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == ',') {
                    state_ = json_comma;
                    return indeterminate;
                } else if (input == '"') {
                    state_ = json_value_right_dot;
                    return indeterminate;
                } else if (input == '\n') { // 如果不是逗号，也不是反斜杠，而是换行符，肯定是结束了
                    state_ = json_check_end;
                    return indeterminate;
                } else {
                    req.value_.push_back(input);
                    return indeterminate;
                }
            case json_value_right_dot:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == ',') {
                    state_ = json_comma;
                    return indeterminate;
                }
                if (input == '"') {
                    state_ = json_check_end;
                    return indeterminate;
                } else {
                    return bad;
                }
            case json_check_end:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == '\n') {
                    return indeterminate;
                } else if (input == '}') {
                    req.body_.insert(make_pair(req.key_, req.value_));
                    req.key_.clear();
                    req.value_.clear();
                    return good;
                } else if (input == ',') {
                    state_ = json_comma;
                    return indeterminate;
                } else {
                    return bad;
                }
            case json_comma:
                if (input == ' ') {
                    return indeterminate;
                }
                if (input == '\n') {
                    state_ = json_key_left_dot;
                    return indeterminate;
                } else if (input == '\\') {
                    state_ = json_key_left_dot;
                    return indeterminate;
                } else {
                    return bad;
                }
            default:
                return bad;
        }
    }

    void RequestParser::parseParam(Request& req, std::string& data_) {
        std::string contentType;
        for (auto& header : req.headers_) {
            if (header.name_ == "Content-Type") {
                contentType = header.value_;
                break;
            }
        }

        int index_content_type = (int) contentType.find_first_of(';');
        if (index_content_type > 0) {
            contentType = contentType.substr(0, index_content_type);
        }

        //开始解析post参数
        if (req.method_ == "POST"
            && contentType == "multipart/form-data") {
            const char* find_str = "\nContent-Disposition: form-data; name=\"";
            int posIndex = 0;
            auto it = boost::algorithm::ifind_nth(data_, find_str, posIndex);
            while (!it.empty()) {
                Header tmp;
                auto it_temp = it.end();
                while (it_temp != data_.end()) {
                    if (*it_temp == '\"') {
                        break;
                    }

                    tmp.name_.push_back(*it_temp);
                    ++it_temp;
                }

                ++it_temp;
                while (*it_temp == '\r' || *it_temp == '\n') {
                    ++it_temp;
                }

                while (it_temp != data_.end()) {
                    if (*it_temp == '\r') {
                        break;
                    }

                    tmp.value_.push_back(*it_temp);
                    ++it_temp;
                }

                ++posIndex;
                it = boost::algorithm::ifind_nth(data_, find_str, posIndex);
                std::string s;

                req.body_[tmp.name_] = tmp.value_;
            }
        }
        //解析post参数结束
    }


    bool RequestParser::isChar(int c) {
        return c >= 0 && c <= 127;
    }

    bool RequestParser::isCtl(int c) {
        return (c >= 0 && c <= 31) || (c == 127);
    }

    bool RequestParser::isTspecial(int c) {
        switch (c) {
            case '(':
            case ')':
            case '<':
            case '>':
            case '@':
            case ',':
            case ';':
            case ':':
            case '\\':
            case '"':
            case '/':
            case '[':
            case ']':
            case '?':
            case '=':
            case '{':
            case '}':
            case ' ':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    bool RequestParser::isDigit(int c) {
        return c >= '0' && c <= '9';
    }

    void RequestParser::parseCookie(Request& request) {
        std::string cookies;
        for (auto& header : request.headers_) {
            if (header.name_ == "Cookie") {
                cookies = header.value_;
                break;
            }
        }

        // 拿到分号分割的所有cookie 键值对
        std::vector<std::string> keyValue;
        boost::split(keyValue, cookies, boost::is_any_of(";"));

        // 拆分键值对为map
        for (const auto& item : keyValue) {
            std::vector<std::string> tmp;
            boost::split(tmp, item, boost::is_any_of("="));
            if (tmp.size() > 1) {
                // 去除空格
                tmp[0].erase(0, tmp[0].find_first_not_of(' '));
                tmp[0].erase(tmp[0].find_last_not_of(' ') + 1);
                tmp[1].erase(0, tmp[1].find_first_not_of(' '));
                tmp[1].erase(tmp[1].find_last_not_of(' ') + 1);

                request.cookies_[tmp[0]] = tmp[1];
            }
        }
    }
}