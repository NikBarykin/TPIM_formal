#pragma once

#include <string>
#include <sstream>
#include <gtest/gtest.h>

#include "nfa.h"


inline void ExpectDumps(NFA nfa, const std::string& expected_dump) {
    std::ostringstream output;
    nfa.dumpTo(output);
    EXPECT_EQ(output.str(), expected_dump);
}
