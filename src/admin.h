


#ifndef ADMIN_H
  #define ADMIN_H 1
  #include "common.h"

  typedef struct Admin{
    __uint64_t id;
    char first_name[32];
    char last_name[32];
    char email[64];
    char phone[16];
    char password[64];
  }admin_t;
  admin_t connected_admin;
#endif