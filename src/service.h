

#pragma once
#ifndef SERVICE_H
  #define SERVICE_H 1
  #include "common.h"
  typedef struct Service{
    __uint64_t id;
    char *name[MAX_NAME_LENGTH];
    char *description[MAX_DESCRIPTION_LENGTH];
    float price;
    time_t duration;
    // equipement?
    // staff ?
  }service_t;
#endif // !SERVICE_H