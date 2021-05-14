//
// Created by sxuer on 2021/5/12.
//

#include "Json.hpp"

namespace web {

    void Json::map2Json(const std::map<std::string, std::string>& map, std::string& json) {
        json.clear();
        json += "{";
        for (const auto& item : map) {
            json += "\"";
            json += item.first;
            json += "\":\"";
            json += item.second;
            json += "\",";
        }
        // 去掉最后一个逗号
        json = json.substr(0, json.rfind(','));
        json += "}";
    }

    void Json::json2Map(const std::string& json, std::map<std::string, std::string>& map) {
        throw "json2Map is not impl";
    }


    void Json::vector2Json(const std::vector<web::Transaction>& vector, std::string& json) {
        json.clear();
        if (vector.empty()) {
            json += "{ \"msg\":\"操作成功\", \"data\":[]}";
        } else {
            json += "{ \"msg\":\"操作成功\", \"data\":[";
            for (const auto& trans : vector) {
                json += "{";

                json += "\"";
                json += "id";
                json += "\":\"";
                json += std::to_string(trans.getId());
                json += "\",";

                json += "\"";
                json += "relate";
                json += "\":\"";
                json += std::to_string(trans.getRelate());
                json += "\",";

                json += "\"";
                json += "total";
                json += "\":\"";
                json += std::to_string(trans.getTotal());
                json += "\",";

                json += "\"";
                json += "uid";
                json += "\":\"";
                json += std::to_string(trans.getUserId());
                json += "\",";

                json += "\"";
                json += "type";
                json += "\":\"";
                json += std::to_string(trans.getType());
                json += "\",";

                json += "\"";
                json += "remark";
                json += "\":\"";
                json += trans.getRemark();
                json += "\",";

                json += "\"";
                json += "createTime";
                json += "\":\"";
                json += std::to_string(trans.getCreateTime());

                json += "\"";
                json += "}";
                json += ",";
            }
            // 去掉最后一个逗号
            json = json.substr(0, json.rfind(','));
            json += "]}";
        }
    }

    void Json::vector2Json(const std::vector<web::User>& vector, std::string& json) {
        json.clear();
        if (vector.empty()) {
            json += "{ \"msg\":\"操作成功\", \"data\":[]}";
        } else {
            json += "{ \"msg\":\"操作成功\", \"data\":[";
            for (const auto& user : vector) {
                json += "{";

                json += "\"";
                json += "uid";
                json += "\":\"";
                json += std::to_string(user.getId());
                json += "\",";

                json += "\"";
                json += "balance";
                json += "\":\"";
                json += std::to_string(user.getBalance());
                json += "\",";

                json += "\"";
                json += "username";
                json += "\":\"";
                json += user.getUsername();
                json += "\"";

                json += "}";
                json += ",";
            }
            // 去掉最后一个逗号
            json = json.substr(0, json.rfind(','));
            json += "]}";
        }
    }
}

