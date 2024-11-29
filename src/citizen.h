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
    __int8_t    first_name[32];
    __int8_t    last_name[32];
    __int8_t    email[32];
    __int8_t    password[64];
    __int8_t    address[64];
    gender_t    gender;
    __int8_t    phone[16];
    float        monthly_bill;
    __int16_t   total_reservations;
    __int8_t    vehicle_num[16];
    time_t      birth_date;
    time_t      registration_datetime;
    time_t      last_login_datetime;
  }citizen_t; 

  // This session's connected citizen
  citizen_t connected_citizen;


  bool_t citizen_pwd_is_same(__int8_t *password1, __int8_t *password2){
  citizen_t citizen_create(__uint64_t id, __int8_t *first_name, __int8_t *last_name, __int8_t *phone, 
                         __int8_t *email, __int8_t *address, gender_t gender, __uint8_t *password);
  void citizen_delete();
  void citizen_fetch(__uint64_t id);
  void citizen_modify();
  void citizen_calc_monthlybill();
  void citizen_get_bills(double bills[31]);
  void citizen_save();
  
  void citizen_details_populate();
  void citizen_modify_populate();
  void citizen_monthlybill_curve_populate(GtkCurve *curve, const double *reservations, int num_days);
  void citizen_account_populate();
#endif // !CITIZEN_H