/*
 * test_EVERY.cpp
 *
 *  Created on: 2019-4-28
 *      Author: fasiondog
 */

#include "doctest/doctest.h"
#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/CVAL.h>
#include <hikyuu/indicator/crt/EVERY.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_EVERY test_indicator_EVERY
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_EVERY") {
    Indicator result;

    PriceList a;
    a.push_back(0);
    a.push_back(1);
    a.push_back(0);
    a.push_back(1);
    a.push_back(1);
    a.push_back(0);

    Indicator data = PRICELIST(a);

    /** @arg n=0 */
    result = EVERY(data, 0);
    CHECK_EQ(result.name(), "EVERY");
    CHECK_EQ(result.discard(), data.discard());
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result[5], 0);
    for (int i = 0; i < 5; i++) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg n=1, total>1 */
    result = EVERY(data, 1);
    CHECK_EQ(result.name(), "EVERY");
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result.size(), data.size());
    for (int i = 0; i < data.size(); ++i) {
        CHECK_EQ(result[i], data[i]);
    }

    /** @arg n=1, total=1 */
    result = EVERY(CVAL(1), 1);
    CHECK_EQ(result.size(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 1);

    result = EVERY(CVAL(0), 1);
    CHECK_EQ(result.size(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 0);

    /** @arg n > 1, total = n */
    result = EVERY(data, 6);
    CHECK_EQ(result.name(), "EVERY");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 5);
    CHECK_EQ(result[5], 0);

    /** @arg n > 1, total < n */
    result = EVERY(data, 7);
    CHECK_EQ(result.name(), "EVERY");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 6);

    /** @arg n > 1, total > n */
    result = EVERY(data, 3);
    CHECK_EQ(result.name(), "EVERY");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 2);
    for (int i = 0; i < result.discard(); i++) {
        CHECK_UNARY(std::isnan(result[i]));
    }
    CHECK_EQ(result[2], 0);
    CHECK_EQ(result[3], 0);
    CHECK_EQ(result[4], 0);
    CHECK_EQ(result[5], 0);

    a.push_back(1);
    a.push_back(1);
    a.push_back(1);
    a.push_back(1);

    data = PRICELIST(a);
    result = EVERY(data, 3);
    CHECK_EQ(result.name(), "EVERY");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 2);
    for (int i = 0; i < result.discard(); i++) {
        CHECK_UNARY(std::isnan(result[i]));
    }
    CHECK_EQ(result[2], 0);
    CHECK_EQ(result[3], 0);
    CHECK_EQ(result[4], 0);
    CHECK_EQ(result[5], 0);
    CHECK_EQ(result[6], 0);
    CHECK_EQ(result[7], 0);
    CHECK_EQ(result[8], 1);
    CHECK_EQ(result[9], 1);
}

/** @par 检测点 */
TEST_CASE("test_EVERY_dyn") {
    Stock stock = StockManager::instance().getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-30));
    // KData kdata = stock.getKData(KQuery(0, Null<size_t>(), KQuery::MIN));
    Indicator c = CLOSE(kdata);
    Indicator o = OPEN(kdata);
    Indicator expect = EVERY(c > o, 3);
    Indicator result = EVERY(c > o, CVAL(c, 3));
    CHECK_EQ(expect.discard(), result.discard());
    CHECK_EQ(expect.size(), result.size());
    for (size_t i = 0; i < expect.discard(); i++) {
        CHECK(isnan(result[i]));
    }
    for (size_t i = expect.discard(); i < expect.size(); i++) {
        CHECK_EQ(expect[i], doctest::Approx(result[i]));
    }

    expect = EVERY(c > o, 0);
    result = EVERY(c > o, CVAL(c, 0));
    CHECK_EQ(expect.discard(), result.discard());
    CHECK_EQ(expect.size(), result.size());
    for (size_t i = 0; i < expect.discard(); i++) {
        CHECK_UNARY(isnan(result[i]));
    }
    for (size_t i = expect.discard(); i < expect.size(); i++) {
        CHECK_EQ(expect[i], doctest::Approx(result[i]));
    }
}

//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
TEST_CASE("test_EVERY_export") {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/EVERY.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = EVERY(CLOSE(kdata) > OPEN(kdata));
    {
        std::ofstream ofs(filename);
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(x1);
    }

    Indicator x2;
    {
        std::ifstream ifs(filename);
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(x2);
    }

    CHECK_EQ(x2.name(), "EVERY");
    CHECK_EQ(x1.size(), x2.size());
    CHECK_EQ(x1.discard(), x2.discard());
    CHECK_EQ(x1.getResultNumber(), x2.getResultNumber());
    for (size_t i = x1.discard(); i < x1.size(); ++i) {
        CHECK_EQ(x1[i], doctest::Approx(x2[i]));
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */
