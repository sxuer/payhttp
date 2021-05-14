//
// Created by sxuer on 2021/5/12.
//

#ifndef PAYHTTP_RCODE_HPP
#define PAYHTTP_RCODE_HPP


namespace web {
    enum RCode {
        SUCCESS=0,
        LOGIN_FAIL=1001,
        BALANCE_ERR=1002,
        PARAM_ERR=1003,
    };
}
#endif //PAYHTTP_RCODE_HPP
