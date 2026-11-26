#include <doctest/doctest.h>
#include <string>
#include "data/json_loader.h"

TEST_CASE("json: valid file loads and returns the stored value") {
    JsonDocument doc("tests/fixtures/valid.json");
    CHECK(doc.root().get_int("x") == 42);
}

TEST_CASE("json: missing file throws naming the path") {
    try {
        JsonDocument doc("tests/fixtures/does_not_exist.json");
        CHECK(false);  // reached only if no exception was thrown
    } catch (const DataError& e) {
        CHECK(std::string(e.what()).find("does_not_exist.json") != std::string::npos);
    }
}

TEST_CASE("json: wrong-type field throws naming the key") {
    JsonDocument doc("tests/fixtures/wrong_type.json");
    try {
        doc.root().get_int("x");
        CHECK(false);  // reached only if no exception was thrown
    } catch (const DataError& e) {
        CHECK(std::string(e.what()).find("x: expected an integer") != std::string::npos);
    }
}