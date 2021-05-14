//
// Created by sxuer on 2021/5/12.
//

#include <web/core/GlobalAutoIds.hpp>
#include "TransactionDao.hpp"
#include "BaseDao.hpp"
#include <iostream>
#include <fstream>

namespace web {

    using namespace std;

    std::map<int, Transaction> TransactionDao::transDatabase_;
    std::map<int, std::vector<int>> TransactionDao::index_;

    bool TransactionDao::loadDatabase(std::map<int, Transaction>& transDatas) {
        // 重要，不然可能存在一条不规则的初始化数据
        transDatas.clear();
        index_.clear();

        // 记录数据条数
        int i = 0;

        fstream in(BaseDao::databaseRoot + "/trans.dat", ios::in);

        int id;
        int userId;
        int type;
        time_t createTime;
        long total;
        int relate;
        std::string remark;

        if (in.is_open()) {
            while (!in.eof()) {
                in >> id >> userId >> type >> createTime >> total >> relate >> remark;
                transDatas.insert(make_pair(id, web::Transaction(id, userId, type, createTime, total, relate, remark)));

                // 保存索引
                index_[userId].push_back(id);
                ++i;
            }
        }
        // 初始化全局自增ID
        GlobalAutoIds::init("trans", i);
        return true;
    }

    void TransactionDao::saveDatabase() {
        fstream out(BaseDao::databaseRoot + "/trans.dat", ios::out);
        if (out.is_open()) {
            for (const auto& item : TransactionDao::transDatabase_) {
                std::cout << item.second << endl;

                out << item.first << '\t';
                out << item.second.getUserId() << '\t';
                out << item.second.getType() << '\t';
                out << item.second.getCreateTime() << '\t';
                out << item.second.getTotal() << '\t';
                out << item.second.getRelate() << '\t';
                out << item.second.getRemark() << '\t';
                out << endl;
            }
            out.close();
        }
    }

}