//
// Created by Zhang Zhuyan on 17/12/22.
//

#ifndef MYCODE_CATCH_H
#define MYCODE_CATCH_H
// Catch includes
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

template<typename T>
auto approx(T&& number) -> Catch::Approx
{
    const double val = static_cast<double>(number);
    return Catch::Approx(val).margin(1e-12);
}

#define CHECK_APPROX(a, b) CHECK(a == approx(b))
#endif //MYCODE_CATCH_H
