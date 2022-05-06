//
// Created by sifi on 18.04.22.
//

#include "../headers/hashing.h"

char *md5_to_string(unsigned char *md) {

    char *hash = (char *) malloc(MD5_DIGEST_LENGTH);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf((char *) (hash + i), "%02x", md[i]);
    }

    return hash;
}