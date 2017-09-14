//
// Created by ogo on 9/8/17.
//

#include "datadog-c.h"
#include "datadog-cpp.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "datadog-c.h"

void* dd_allocate(char const* host,
                  unsigned int port,
                  char const* tags_string)
{
    std::istringstream iss(tags_string);
    std::vector<std::string> tags;
    copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(tags));

    datadog::Dogstatsd* instance = new datadog::Dogstatsd(host, port, false, tags);

    return static_cast<void*>(instance);
}

void dd_free(void* connection)
{
    datadog::Dogstatsd* instance = static_cast<datadog::Dogstatsd*>(connection);
    delete instance;
}

int dd_gauge(void* connection, char const* name, int value)
{
    datadog::Dogstatsd* instance = static_cast<datadog::Dogstatsd*>(connection);
    return instance->gauge(name, value, std::vector<std::string>(), 1);
}

