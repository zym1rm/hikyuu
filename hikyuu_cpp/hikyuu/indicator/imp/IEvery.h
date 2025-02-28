/*
 * IEvery.h
 *
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-4-28
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_IEVERY_H_
#define INDICATOR_IMP_IEVERY_H_

#include "../Indicator.h"

namespace hku {

/*
 * 一直存在, EVERY (X,N) 表示条件X在N周期一直存在
 */
class IEvery : public IndicatorImp {
    INDICATOR_IMP_SUPPORT_IND_PARAM(IEvery)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    IEvery();
    virtual ~IEvery();

    virtual void _after_dyn_calculate(const Indicator& ind) override;
};

} /* namespace hku */
#endif /* INDICATOR_IMP_IEVERY_H_ */
