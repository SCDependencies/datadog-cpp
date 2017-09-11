//
// Created by ogo on 9/8/17.
//

#include "c_wrapper.h"
#include "trace_dogstatsd.hpp"

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