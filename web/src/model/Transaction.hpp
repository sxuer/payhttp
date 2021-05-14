//
// Created by sxuer on 2021/5/11.
//

#ifndef PAYHTTP_TRANSACTION_HPP
#define PAYHTTP_TRANSACTION_HPP

#include <ctime>
#include <string>
#include <ostream>

namespace web {

    class Transaction {
        // 交易编号
        int id_;

        // 所属用户
        int userId_;

        // 0转入 1转出
        int type_;

        // 交易时间
        time_t createTime;

        // 交易金额
        long total_;

        /**
         * 另一方账号，如果是转入，则为转入来源
         * 如果是转出，则为转出目标账号id
         */
        int relate_;

        std::string remark_;

    public:

        Transaction() = default;

        Transaction(int id, int userId, int type, time_t createTime, long total,
                    int relate, std::string remark);

        const std::string& getRemark() const;

        void setRemark(const std::string& remark);

        enum Type {
            RECHARGE = 0, // 充值
            COMSUME = 1,  // 消费
            IN = 2,// 转出
            OUT = 3// 转入
        };

        enum TransactionType {
            in = -1,
            out = -2
        };

        // 转入\转出为第三方的时候，relate_的取值
        enum relateType {
            aliPay = -1,
            wxPay = -2
        };

        int getId() const;

        void setId(int id);

        int getUserId() const;

        void setUserId(int userId);

        int getType() const;

        void setType(int type);

        time_t getCreateTime() const;

        void setCreateTime(time_t createTime);

        long getTotal() const;

        void setTotal(long total);

        int getRelate() const;

        void setRelate(int relate);

        friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);
    };
}


#endif //PAYHTTP_TRANSACTION_HPP
