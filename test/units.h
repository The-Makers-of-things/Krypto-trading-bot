#ifndef K_UNITS_H_
#define K_UNITS_H_

namespace K {
  TEST_CASE("mMarketLevels") {
    SECTION("mLevel") {
      mLevel level;
      SECTION("defaults") {
        REQUIRE_NOTHROW(level = mLevel());
      }
      SECTION("assigned") {
        REQUIRE_NOTHROW(level = mLevel(
          1234.56,
          0.12345678
        ));
        SECTION("values") {
          REQUIRE(level.price == 1234.56);
          REQUIRE(level.size == 0.12345678);
        }
        SECTION("not empty") {
          REQUIRE_FALSE(level.empty());
        }
        SECTION("to json") {
          REQUIRE(((json)level).dump() == "{\"price\":1234.56,\"size\":0.12345678}");
        }
        SECTION("clear") {
          REQUIRE_NOTHROW(level.clear());
          SECTION("values") {
            REQUIRE_FALSE(level.price);
            REQUIRE_FALSE(level.size);
          }
          SECTION("empty") {
            REQUIRE(level.empty());
          }
          SECTION("to json") {
            REQUIRE(((json)level).dump() == "{\"price\":0.0}");
          }
        }
      }
    }
    SECTION("mLevels") {
      mLevels levels;
      SECTION("defaults") {
        REQUIRE_NOTHROW(levels = mLevels());
      }
      SECTION("assigned") {
        REQUIRE_NOTHROW(levels = mLevels(
          { mLevel(1234.56, 0.12345678) },
          { mLevel(1234.57, 0.12345679) }
        ));
        SECTION("values") {
          REQUIRE(levels.spread() == Approx(0.01));
        }
        SECTION("not empty") {
          REQUIRE_FALSE(levels.empty());
        }
        SECTION("to json") {
          REQUIRE(((json)levels).dump() == "{\"asks\":[{\"price\":1234.57,\"size\":0.12345679}],\"bids\":[{\"price\":1234.56,\"size\":0.12345678}]}");
        }
        SECTION("clear") {
          REQUIRE_NOTHROW(levels.clear());
          SECTION("values") {
            REQUIRE_FALSE(levels.spread());
          }
          SECTION("empty") {
            REQUIRE(levels.empty());
          }
          SECTION("to json") {
            REQUIRE(((json)levels).dump() == "{\"asks\":[],\"bids\":[]}");
          }
        }
      }
    }
    SECTION("mMarketLevels") {
      mMarketLevels levels;
      SECTION("defaults") {
        REQUIRE_NOTHROW(levels = mMarketLevels());
      }
      SECTION("assigned") {
        REQUIRE_NOTHROW(levels.send_reset_filter(mLevels(
          { mLevel(1234.50, 0.12345678) },
          { mLevel(1234.60, 0.12345678) }
        ), 0.01));
        SECTION("values") {
          REQUIRE_NOTHROW(levels.calcFairValue(0.01));
          REQUIRE(levels.fairValue == 1234.55);
        }
      }
    }
  }
}

#endif