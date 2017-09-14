//
// Created by ogo on 9/8/17.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void* dd_allocate(char const*  host,
                  unsigned int port,
                  char const* tags_string);

void    dd_free(void* connection);
int     dd_gauge(void* connection, char const* name, int value);


#ifdef __cplusplus
}
#endif
