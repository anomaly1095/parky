#pragma once

#ifndef AGENT_H
  #define AGENT_H 1
  #include "common.h"
  #include "parking.h"
  typedef struct Agent {
    __uint64_t id;
    char *first_name[MAX_NAME_LENGTH];
    char *last_name[MAX_NAME_LENGTH];
    char *address[MAX_ADDRESS_LENGTH];
    char *phone_num[MAX_PHONE_LENGTH];
    time_t hiring_datetime;
    __uint64_t id_parking;
    availability_t availability;
  }agent_t;
#endif // !AGENT_H