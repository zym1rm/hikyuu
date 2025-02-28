/*
 * MA.h
 *
 *  Created on: 2013-2-14
 *      Author: fasiondog
 */

#pragma once
#ifndef MA_H_
#define MA_H_

#include "../Indicator.h"

namespace hku {

/**
 * 简单移动平均
 * @param data 待计算的数据
 * @param n 计算均值的周期窗口，必须为大于0的整数
 * @ingroup Indicator
 */
Indicator HKU_API MA(int n = 22);
Indicator HKU_API MA(const IndParam& n);

inline Indicator HKU_API MA(const Indicator& ind, int n = 22) {
    return MA(n)(ind);
}

inline Indicator HKU_API MA(const Indicator& ind, const IndParam& n) {
    return MA(n)(ind);
}

inline Indicator MA(const Indicator& ind, const Indicator& n) {
    return MA(IndParam(n))(ind);
}

}  // namespace hku

#endif /* MA_H_ */
