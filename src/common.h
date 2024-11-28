#ifndef COMMONH_H
  #define COMMON_H 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>
  #include <string.h>
  #include <gtk/gtk.h>

  #define PATH_DATA             "../data"
  #define PATH_ADMIN_DATA       PATH_DATA "/admin.bin"       // file containing the admin
  #define PATH_AGENT_DATA       PATH_DATA "/agent.bin"       // file containing all the agents
  #define PATH_CITIZEN_DATA     PATH_DATA "/citizen.bin"     // file containing all the citizens
  #define PATH_COMPLAINT_DATA   PATH_DATA "/complaint.bin"   // file containing all the complaints
  #define PATH_PARKING_DATA     PATH_DATA "/parking.bin"     // file containing all the parkings
  #define PATH_RESERVATION_DATA  PATH_DATA "/reservation.bin" // file containing all the reservations
  #define PATH_REVIEW_DATA      PATH_DATA "/review.bin"      // file containing all the reviews
  #define PATH_SERVICE_DATA     PATH_DATA "/service.bin"     // file containing all the services
  #define PATH_ID_DATA          PATH_DATA "/ids.bin"         // file containing all the ids

  // boolean type for who needs it
  typedef enum Bool{
    false,
    true
  }bool_t;

  // struct containing all id's
  struct Ids{
    __uint64_t id_admin;
    __uint64_t id_agent;
    __uint64_t id_citizen;
    __uint64_t id_complaint;
    __uint64_t id_parking;
    __uint64_t id_reservation;
    __uint64_t id_review;
    __uint64_t id_service;
  };
  
  struct Ids id_list;
  /// @brief fetch all id's from ids.bin
  void fetch_id_counts();
  /// @brief save updated id's to ids.bin
  void update_id_counts();
  /// @brief Converts a time_t timestamp into a human-readable date-time string.
  /// @param timestamp The input timestamp to be converted. It should be a valid `time_t` value
  /// @param datetime_str A character array where the formatted date-time string will be stored `YYYY-MM-DD HH:MM:SS`. 
  /// @param max_len The maximum length of the `datetime_str` buffer minimum should be 20. 
  void time_to_datetime_string(time_t timestamp, char *datetime_str, size_t max_len);

  

#endif // !COMMONH_H
