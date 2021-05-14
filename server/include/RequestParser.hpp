//
// Created by sxuer on 2021/5/5.
//

#ifndef payhttp_REQUESTPARSER_HPP
#define payhttp_REQUESTPARSER_HPP


#include <tuple>
#include "Request.hpp"
#include <iostream>

namespace payhttp {
    class RequestParser {
    public:
        RequestParser();

        // 重置解析器状态
        void reset();

        // 解析器结果 // 好、坏、尚未定论（继续解析）
        enum resultType {
            good, bad, indeterminate
        };

        void parseParam(Request& req, std::string& data_);

        /**
         * 根据begin逐个char解析，获得一个req对象，并返回解析结果
         * 最终的InputIterator是被消耗过的，指向尚未被消费的起点处
         */
        template<typename InputIterator>
        std::tuple<resultType, InputIterator> parse(Request& req,
                                                    InputIterator begin, InputIterator end) {
            // 挨个解析，直到得到好\坏结果为止
            while (begin != end) {
                resultType result = consume(req, *begin++);
                if (result == bad) {
                    return std::make_tuple(result, begin);
                } else if (result == good) {// 第一次good可能是header解析完毕，可能还有请求体
                    if (*begin++ == '{') {
                        state_ = json_parse_start;
                        continue;
                    }
                    return std::make_tuple(result, begin);
                }
            }
            return std::make_tuple(indeterminate, begin);
        }

        void parseCookie(Request& request);

    private:
        // 利用输入的串，构造request对象
        resultType consume(Request& req, char input);

        static bool isChar(int c);

        static bool isDigit(int c);

        // return (c >= 0 && c <= 31) || (c == 127);
        static bool isCtl(int c);

        /// 一些特殊可用符号  < > {} 等
        static bool isTspecial(int c);

        /// 解析器状态
        enum state {
            method_start,
            method,
            uri,
            http_version_h,
            http_version_t_1,
            http_version_t_2,
            http_version_p,
            http_version_slash,
            http_version_major_start,
            http_version_major,
            http_version_minor_start,
            http_version_minor,
            expecting_newline_1,
            header_line_start,
            header_lws,
            header_name,
            space_before_header_value,
            header_value,
            expecting_newline_2,
            expecting_newline_3,


            // {\n    \"name\":123,\n    \"李四\":\"李四\",\n    \"1\":null,\n    \"wx\":12.8\n}
            json_parse_start, // {
//            json_key_left_slash, // key的左引号前的反斜杠
            json_key_left_dot,  // key的左引号"
            json_key_right_dot,  // key的右引号"
            json_key,
            json_value_start,
            json_value,
            json_value_left_dot,  // key的左引号"
            json_value_right_dot,  // key的右引号"
            json_check_end, // value结束，判断是否是结尾
            json_comma,  // 逗号
            json_colon, // 冒号
            json_save // 保存一对键值对
        } state_;
    };
}

#endif //payhttp_REQUESTPARSER_HPP
