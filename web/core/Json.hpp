//
// Created by sxuer on 2021/5/12.
//

#ifndef PAYHTTP_JSON_HPP
#define PAYHTTP_JSON_HPP

#include <string>
#include <map>
#include <vector>
#include "web/src/model/Transaction.hpp"
#include "web/src/model/User.hpp"

namespace web {
    class Json {
    public:
        Json() = delete;

        static void map2Json(const std::map<std::string, std::string>& map, std::string& json);

        static void json2Map(const std::string& json, std::map<std::string, std::string>& map);

        static void vector2Json(const std::vector<web::Transaction>& vector, std::string& json);
        static void vector2Json(const std::vector<web::User>& vector, std::string& json);
    };

};


#endif //PAYHTTP_JSON_HPP
