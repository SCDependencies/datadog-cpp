//
// Created by ogo on 9/8/17.
//

#include "datadog-c.h"
#include "datadog-cpp.hpp"

static datadog::Dogstatsd& connection()
{
    std::vector<std::string> tags;
    static datadog::Dogstatsd instance(tags);
    return instance;
}


void test_d1()
{
    datadog::Dogstatsd& dog = connection();
}