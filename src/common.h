#pragma once

#ifndef COMMONH_H
  #define COMMON_H 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>
  #include <string.h>
  #include <stdbool.h>
  #include <gtk/gtk.h>

  #define PATH_DATA               "data"
  #define PATH_AGENT_DATA         PATH_DATA "/agent.dat"       // file containing all the agents
  #define PATH_CITIZEN_DATA       PATH_DATA "/citizen.dat"     // file containing all the citizens
  #define PATH_PARKING_DATA       PATH_DATA "/parking.dat"     // file containing all the parkings
  #define PATH_RESERVATION_DATA   PATH_DATA "/reservation.dat" // file containing all the reservations
  #define PATH_SERVICE_DATA       PATH_DATA "/service.dat"     // file containing all the services
  #define PATH_ID_DATA            PATH_DATA "/id.dat"         // file containing all the ids
  #define PATH_CITIZEN_TEMP_DATA  PATH_DATA "/citizen_temp.dat"
  #define FAILURE                 1U
  #define SUCCESS                 0U
  #define MAX_NAME_LENGTH         100U
  #define MAX_PHONE_LENGTH        20U
  #define MAX_EMAIL_LENGTH        100U
  #define MAX_ADDRESS_LENGTH      200U
  #define MAX_PASSWORD_LENGTH     64U
  #define MAX_VEHICLE_NUM_LENGTH  16U

  // struct containing all id's
  struct Ids{
    __uint64_t id_agent;
    __uint64_t id_citizen;
    __uint64_t id_parking;
    __uint64_t id_reservation;
    __uint64_t id_service;
  };
  
  extern struct Ids id_list;

  extern GtkWidget *signup_window;    // Youssef
  extern GtkWidget *signin_window;    // Youssef
  extern GtkWidget *citizen_window;   // Youssef + Salma   
  extern GtkWidget *admin_window;     // Amine + Iheb + Fedi
  extern GtkWidget *lookup_widget(GtkWidget *widget, const gchar *widget_name);

  void fetch_id_counts();
  void update_id_counts();
  void time_to_datetime_string(time_t timestamp, char *datetime_str, size_t max_len);

#endif // !COMMONH_H
