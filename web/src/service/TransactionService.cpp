//
// Created by sxuer on 2021/5/12.
//

#include "TransactionService.hpp"
#include "web/src/dao/TransactionDao.hpp"

namespace web {

    std::vector<Transaction> TransactionService::selectByUserId(int uid) {
        std::vector<Transaction> res;
        res.clear();
        if (TransactionDao::index_.count(uid) == 1) {
            // 遍历所有cid，然后从database中取出放入到vector中
            for (int& cid : TransactionDao::index_[uid]) {
                if (TransactionDao::transDatabase_.count(cid) == 1) {
                    // 要求Transaction实现空构造，因为
                    res.emplace_back(TransactionDao::transDatabase_[cid]);
                }
            }
        }
        return res;
    }

    void TransactionService::saveRecord(const Transaction& trans) {
        // 保存索引
        TransactionDao::index_[trans.getUserId()].push_back(trans.getId());
        // 保存数据
        TransactionDao::transDatabase_[trans.getId()] = trans;
    }
}