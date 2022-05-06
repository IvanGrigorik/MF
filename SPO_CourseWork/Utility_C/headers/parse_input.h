//
// Created by sifi on 17.04.22.
//

#ifndef UTILITY_C_PARSE_INPUT_H
#define UTILITY_C_PARSE_INPUT_H

#include "structures.h"

#include <getopt.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

char *get_dir(int argc, char *argv[]);

flags_t parse_flags(int argc, char *argv[]);

#endif //UTILITY_C_PARSE_INPUT_H
