#include <doctest/doctest.h>
#include "data/content_database.h"

TEST_CASE("tuning: values load from Data/tuning.json") {
    const ContentDatabase db = load_content_database("Data");
    const TuningDef& t = db.tuning;

    CHECK(t.continues == 5);
    CHECK(t.bombs_start == 3);
    CHECK(t.bombs_max == 10);
    CHECK(t.lives_penalty == doctest::Approx(0.1));
    CHECK(t.multiplier_min == doctest::Approx(0.6));
    CHECK(t.multiplier_max == doctest::Approx(1.0));
    CHECK(t.shop_size == 8);
    CHECK(t.max_bullets == 4096);
    CHECK(t.speed_floor_ratio == doctest::Approx(2.0));
}

TEST_CASE("tuning: bombs_start never exceeds bombs_max") {
    const ContentDatabase db = load_content_database("Data");
    CHECK(db.tuning.bombs_start <= db.tuning.bombs_max);
}

TEST_CASE("tuning: multiplier range is valid") {
    const ContentDatabase db = load_content_database("Data");
    CHECK(db.tuning.multiplier_min > 0.0);
    CHECK(db.tuning.multiplier_min <= db.tuning.multiplier_max);
}