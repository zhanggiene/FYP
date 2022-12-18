//
// Created by Zhang Zhuyan on 17/12/22.
//
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
TEST_CASE( "vectors can be sized and resized", "[vector2]" ) {

    std::vector<int> v( 5 );

    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );

    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );

        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
}