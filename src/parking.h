
#pragma once

#ifndef PARKING_H
  #define PARKING_H 1
  #include "common.h"
  #include "agent.h"
  
  typedef enum Availability{
    available,
    non_available
  }availability_t;

  typedef struct Parking{
    __uint64_t id;
    char *location;
    unsigned int capacity;
    availability_t availability;
    float fees;
    __uint64_t id_agent;
  }parking_t;

#endif // !PARKING_H