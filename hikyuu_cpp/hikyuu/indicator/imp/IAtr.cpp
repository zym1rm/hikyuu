/*
 * IAtr.cpp
 *
 *  Created on: 2016年5月4日
 *      Author: Administrator
 */

#include "IAtr.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::IAtr)
#endif

namespace hku {

IAtr::IAtr() : IndicatorImp("ATR", 1) {
    setParam<int>("n", 14);
}

IAtr::~IAtr() {}

bool IAtr::check() {
    return getParam<int>("n") >= 1;
}

void IAtr::_calculate(const Indicator& indicator) {
    size_t total = indicator.size();
    m_discard = indicator.discard();
    if (m_discard >= total) {
        m_discard = total;
        return;
    }

    int n = getParam<int>("n");
    size_t startPos = discard();
    price_t ema = indicator[startPos];
    _set(ema, startPos);

    price_t multiplier = 2.0 / (n + 1);
    for (size_t i = startPos + 1; i < total; ++i) {
        ema = (indicator[i] - ema) * multiplier + ema;
        _set(ema, i);
    }
}

Indicator HKU_API ATR(int n) {
    IndicatorImpPtr p = make_shared<IAtr>();
    p->setParam<int>("n", n);
    return Indicator(p);
}

Indicator HKU_API ATR(const Indicator& data, int n) {
    return ATR(n)(data);
}

} /* namespace hku */
