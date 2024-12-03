

#pragma once

#ifndef ADMIN_H
  #define ADMIN_H 1
  #include "common.h"

  typedef struct Admin{
    __uint64_t id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
  }admin_t;
  
  extern admin_t admin;
  bool admin_signin(const char *email, const char *password);
#endif // !ADMIN_H