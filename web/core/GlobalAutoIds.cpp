//
// Created by sxuer on 2021/5/11.
//


#include <map>
#include <atomic>
#include <memory>
#include "GlobalAutoIds.hpp"


namespace web {

    std::map<std::string, std::shared_ptr<std::atomic<int>>>  GlobalAutoIds::ids;

    int GlobalAutoIds::getId(const std::string& tableName) {
        // 如果存在，则返回，否则初始化一个id库
        if (ids.count(tableName) == 1) {
            std::shared_ptr<std::atomic<int>>& ptr = ids[tableName];
            ++*ptr;
            return *ptr;
        } else {
            std::shared_ptr<std::atomic<int>> tmp = std::make_shared<std::atomic<int>>(1);
            ids[tableName] = tmp;
        }
        return 1;
    }

    void GlobalAutoIds::init(const std::string& tableName, int num) {
        ids[tableName] = std::make_shared<std::atomic<int>>(num);;
    }
}


