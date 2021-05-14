//
// Created by sxuer on 2021/5/13.
//

#ifndef PAYHTTP_BASEDAO_HPP
#define PAYHTTP_BASEDAO_HPP

#include "string"

namespace web {
    class BaseDao {
    public:
        // 暂时仅用于保存数据库位置
        static std::string databaseRoot;

//        BaseDao() = delete;
    };
}
#endif //PAYHTTP_BASEDAO_HPP
