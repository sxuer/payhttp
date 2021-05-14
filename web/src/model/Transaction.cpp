//
// Created by sxuer on 2021/5/11.
//

#include "Transaction.hpp"

#include <utility>

namespace web {


    int Transaction::getId() const {
        return id_;
    }

    void Transaction::setId(int id) {
        id_ = id;
    }

    int Transaction::getUserId() const {
        return userId_;
    }

    void Transaction::setUserId(int userId) {
        userId_ = userId;
    }

    int Transaction::getType() const {
        return type_;
    }

    void Transaction::setType(int type) {
        type_ = type;
    }

    time_t Transaction::getCreateTime() const {
        return createTime;
    }

    void Transaction::setCreateTime(time_t createTime) {
        Transaction::createTime = createTime;
    }

    long Transaction::getTotal() const {
        return total_;
    }

    void Transaction::setTotal(long total) {
        total_ = total;
    }

    int Transaction::getRelate() const {
        return relate_;
    }

    void Transaction::setRelate(int relate) {
        relate_ = relate;
    }

    const std::string& Transaction::getRemark() const {
        return remark_;
    }

    void Transaction::setRemark(const std::string& remark) {
        remark_ = remark;
    }

    Transaction::Transaction(int id, int userId, int type, time_t createTime, long total,
                             int relate, std::string remark) : id_(id), userId_(userId), type_(type),
                                                                          createTime(createTime), total_(total),
                                                                          relate_(relate),
                                                                          remark_(std::move(remark)) {}

    std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
        os << "id_: " << transaction.id_ << " userId_: " << transaction.userId_ << " type_: " << transaction.type_
           << " createTime: " << transaction.createTime << " total_: " << transaction.total_ << " relate_: "
           << transaction.relate_ << " remark_: " << transaction.remark_;
        return os;
    }
}