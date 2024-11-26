

#ifndef RESERVATION_H
  #define RESERVATION_H 1
  #include "common.h"
  typedef struct Reservation{
    __uint64_t id;
    time_t reservation_datetime;
    time_t start_datetime;
    time_t end_datetime;
    __uint64_t id_parking;
    __uint64_t id_service;
    __uint64_t id_citizen;
    float cost;
    char description[256];
  }reservation_t;
  
#endif