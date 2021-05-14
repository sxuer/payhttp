//
// Created by sxuer on 2021/5/11.
//

#ifndef PAYHTTP_GLOBALAUTOIDS_HPP
#define PAYHTTP_GLOBALAUTOIDS_HPP

#include <map>
#include <atomic>
#include <memory>

namespace web {

    // 全局自增主键
    class GlobalAutoIds {
    public:
        static std::map<std::string, std::shared_ptr<std::atomic<int>>> ids;

        static int getId(const std::string& tableName);

        static void init(const std::string& tableName,int num);

        GlobalAutoIds() = delete;
    };
}

#endif //PAYHTTP_GLOBALAUTOIDS_HPP

