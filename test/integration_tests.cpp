//
// Created by Zhang Zhuyan on 17/12/22.
//


#include "catch2/catch.hpp"
#include "ApprovalTests.hpp"
#include "Point.h"
TEST_CASE("HelloApprovals")
{
    ApprovalTests::Approvals::verify("Hello 1");
    // Point a(1.0,1.0, 1.0,1.0,1.0, 1.0,1.0,1.0, 1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0);
}