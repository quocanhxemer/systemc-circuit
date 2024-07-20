#ifndef CSV_READER_H
#define CSV_READER_H

#include "Request.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>

struct csv_file_data {
    size_t lines;
    struct Request* data;
};

struct csv_file_data csv_parse_file(const char* filename);

#endif // !CSV_READER_H
