//
// Created by sxuer on 2021/5/12.
//

#ifndef PAYHTTP_TRANSACTIONDAO_HPP
#define PAYHTTP_TRANSACTIONDAO_HPP

#include "web/src/model/Transaction.hpp"
#include "web/src/service/TransactionService.hpp"
#include <map>
#include <string>

namespace web {
    class TransactionDao {
    public:
        TransactionDao() = delete;

        // 存储交易信息
        static std::map<int, Transaction> transDatabase_;
        // 索引表  uid:cid  一个uid对应多个cid
        static std::map<int, std::vector<int>> index_;

        // 启动时加载数据
        static bool loadDatabase(std::map<int, Transaction>&);

        // 结束时保存数据
        static void saveDatabase();
    };
}


#endif //PAYHTTP_TRANSACTIONDAO_HPP
