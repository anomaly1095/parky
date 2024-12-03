#pragma once

#ifndef CITIZEN_H
  #define CITIZEN_H 1
  #include "common.h"
  #include "reservation.h"

  typedef enum Gender{
    male,
    female
  }gender_t;
  

  typedef struct Citizen{
    __uint64_t  id;
    char        first_name[MAX_NAME_LENGTH];
    char        last_name[MAX_NAME_LENGTH];
    char        email[MAX_EMAIL_LENGTH];
    char        password[MAX_PASSWORD_LENGTH];
    char        address[MAX_ADDRESS_LENGTH];
    gender_t    gender;
    char        phone[MAX_PHONE_LENGTH];
    float       monthly_bill;
    __int16_t   total_reservations;
    char        vehicle_num[16];
    time_t      birth_date;
    time_t      registration_datetime;
    time_t      last_login_datetime;
  }citizen_t; 

  // This session's connected citizen
  extern citizen_t connected_citizen;
  extern __uint64_t citizen_connected_id;
  
  bool    citizen_pwd_is_same               (const char *password1, const char *password2);
citizen_t citizen_create                    (const __uint64_t id, const char *first_name, const char *last_name, const char *phone, const char *email, const char *address, const gender_t gender, const char *password);
  bool    citizen_signin                    (const char *email, const char *password);
  void    citizen_delete                    ();
  void    citizen_fetch                     (__uint64_t id);
  void    citizen_modify                    (const char *first_name, const char *last_name, const char *phone, const char *email, const char *street, const char *password, const char *car_num, const char *address, struct tm birth_time, gboolean is_male, gboolean is_female);
  void    citizen_calc_monthlybill          ();
  void    citizen_get_bills                 (double bills[31]);
  void    citizen_save                      ();
  void    citizen_details_populate          ();
  void    citizen_modify_populate           ();
  void    citizen_monthlybill_curve_populate(GtkCurve *curve, const double *reservations, int num_days);
  void    citizen_account_populate          ();
#endif // !CITIZEN_H