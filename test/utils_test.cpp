#include "catch2/catch.hpp"

#include <fstream>
#include <iostream>

#include "utils.h"

TEST_CASE("Split string")
{
    const std::string str("_foo_bar baz_qux");

    REQUIRE(split(str, '_').size() == 3);
    REQUIRE(split(str, '_')[0] == "foo");
    REQUIRE(split(str, '_')[1] == "bar baz");
    REQUIRE(split(str, '_')[2] == "qux");

    REQUIRE(split(str, ' ').size() == 2);
    REQUIRE(split(str, ' ')[0] == "_foo_bar");
    REQUIRE(split(str, ' ')[1] == "baz_qux");
}

TEST_CASE("Title case string")
{
    REQUIRE(titleCase("foo") == "Foo");
    REQUIRE(titleCase("foo_bar") == "Foo Bar");
    REQUIRE(titleCase("foo_bar_BAZ") == "Foo Bar Baz");
    REQUIRE(titleCase("foo-bar", '-') == "Foo Bar");
}

TEST_CASE("Trim string")
{
    REQUIRE(trim(" foo bar") == "foo bar");
    REQUIRE(trim("		 foo bar ") == "foo bar");
    REQUIRE(trim("\tfoo bar", "\t") == "foo bar");
    REQUIRE(trim("  foo bar", "\t") == "  foo bar");
    REQUIRE(trim("abcd", "ad") == "bc");
}

TEST_CASE("Read from file")
{
    const std::string filepath("/tmp/testingfile");
    const std::string filecontent = "row 1\nrow 2";

    std::ofstream outfile(filepath);
    outfile << filecontent;
    outfile.close();

    REQUIRE(readFile(filepath) == filecontent);
}
