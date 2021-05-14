//
// Created by sxuer on 2021/5/12.
//

#ifndef PAYHTTP_TRANSACTIONSERVICE_HPP
#define PAYHTTP_TRANSACTIONSERVICE_HPP

#include "web/src/model/Transaction.hpp"
#include <vector>
#include <map>

namespace web {
    class TransactionService {
    public:
        TransactionService() = delete;

        static std::vector<Transaction> selectByUserId(int uid);

        static void saveRecord(const Transaction&);
    };
}


#endif //PAYHTTP_TRANSACTIONSERVICE_HPP
