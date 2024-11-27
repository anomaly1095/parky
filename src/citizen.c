#include "citizen.h"

bool_t citizen_pwd_is_same(const char *password1, const char *password2) {
    // Compare strings using strcmp (returns 0 if the strings are equal)
    if (strcmp(password1, password2) == 0)
        return true;  // Passwords are the same
    else
        return false;  // Passwords are different
}

citizen_t citizen_create(__uint64_t id, __int8_t *first_name, __int8_t *last_name, __int8_t *phone, 
                         __int8_t *email, __int8_t *address, gender_t gender, __uint8_t *password) {

  citizen_t new_citizen = {
      .id = id,
      .first_name = first_name,
      .last_name = last_name,
      .phone = phone,
      .email = email,
      .address = address,
      .password = password,
      .gender = gender,
      .monthly_bill = 0.0f,
      .birth_date = time(NULL),
      .registration_datetime = time(NULL),
      .vehicle_num = 12345678,
      .last_login_datetime = time(NULL)  // Set to current time
  };

  return new_citizen;
}



void citizen_delete() {
  FILE *file = fopen(PATH_CITIZEN_DATA, "rb+");  // Open file for reading and writing in binary mode
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  citizen_t current_citizen;
  size_t citizen_size = sizeof(citizen_t);
  int found = 0;

  // Iterate through the file, reading one citizen at a time
  while (fread(&current_citizen, citizen_size, 1, file) == 1) {
    if (current_citizen.id == connected_citizen.id) {
      found = 1;
      break;  // We found the citizen, so we stop reading further
    }
  }

  // If the citizen was found, overwrite the record with empty data
  if (found) {
    // Move the file pointer back to the start of the deleted record
    fseek(file, -citizen_size, SEEK_CUR);

    // Write an empty or "deleted" citizen record to overwrite the existing one
    citizen_t empty_citizen = {0};  // Initialize an empty citizen record (all fields to 0)
    fwrite(&empty_citizen, citizen_size, 1, file);

    // Close the file after writing
    fclose(file);
    printf("Citizen with ID %llu deleted.\n", connected_citizen.id);
  } else {
    printf("Citizen with ID %llu not found.\n", connected_citizen.id);
    fclose(file);
  }
}

bool_t citizen_signin(__int8_t *email, __int8_t *password) {
  FILE *file = fopen(PATH_CITIZEN_DATA, "rb");  // Open file for reading in binary mode
  if (file == NULL) {
    perror("Error opening file");
    return false;
  }

  citizen_t current_citizen;
  size_t citizen_size = sizeof(citizen_t);

  // Iterate through the file, reading one citizen at a time
  while (fread(&current_citizen, citizen_size, 1, file) == 1) {
    // Compare email and password
    if (strcmp((char *)current_citizen.email, (char *)email) == 0 &&
      strcmp((char *)current_citizen.password, (char *)password) == 0) {
      fclose(file);
      // Citizen account found, assign to the global variable
      connected_citizen = current_citizen;
      return true;
    }
  }

  // If no match is found
  fclose(file);
  printf("No account found with the given email and password.\n");
}



void citizen_fetch(__uint64_t id) {
  FILE *file = fopen(PATH_CITIZEN_DATA, "rb");  // Open file for reading in binary mode
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  citizen_t current_citizen;
  size_t citizen_size = sizeof(citizen_t);

  // Iterate through the file, reading one citizen at a time
  while (fread(&current_citizen, citizen_size, 1, file) == 1) {
    if (current_citizen.id == id) {
      fclose(file);
      // citizen account fetched and assigned to the global variable
      connected_citizen = current_citizen;
      return;
    }
  }
  // none found with that id
  fclose(file);
}


// Modify function that fills the connected_citizen struct
void citizen_modify(const __int8_t *first_name, const __int8_t *last_name, const __int8_t *phone,
                    const __int8_t *email, const __int8_t *street, const __int8_t *password,
                    const __int8_t *car_num, const __int8_t *address, struct tm birth_time) {

  // Fill in the connected_citizen struct with the passed values

  // Copy strings into the Citizen struct fields
  strncpy((char *)connected_citizen.first_name, first_name, sizeof(connected_citizen.first_name) - 1);
  connected_citizen.first_name[sizeof(connected_citizen.first_name) - 1] = '\0'; // Null-terminate

  strncpy((char *)connected_citizen.last_name, last_name, sizeof(connected_citizen.last_name) - 1);
  connected_citizen.last_name[sizeof(connected_citizen.last_name) - 1] = '\0'; // Null-terminate

  strncpy((char *)connected_citizen.phone, phone, sizeof(connected_citizen.phone) - 1);
  connected_citizen.phone[sizeof(connected_citizen.phone) - 1] = '\0'; // Null-terminate

  strncpy((char *)connected_citizen.email, email, sizeof(connected_citizen.email) - 1);
  connected_citizen.email[sizeof(connected_citizen.email) - 1] = '\0'; // Null-terminate

  strncpy((char *)connected_citizen.password, password, sizeof(connected_citizen.password) - 1);
  connected_citizen.password[sizeof(connected_citizen.password) - 1] = '\0'; // Null-terminate

  // Use snprintf to combine street and address into the address field
  snprintf((char *)connected_citizen.address, sizeof(connected_citizen.address), "%s, %s", street, address);

  // Set gender based on radio button values
  if (is_male)
      connected_citizen.gender = male;
  else if (is_female)
      connected_citizen.gender = female;

  // Copy car number (vehicle number) into the Citizen struct
  strncpy((char *)connected_citizen.vehicle_num, car_num, sizeof(connected_citizen.vehicle_num) - 1);
  connected_citizen.vehicle_num[sizeof(connected_citizen.vehicle_num) - 1] = '\0'; // Null-terminate

  // Set birth_date
  connected_citizen.birth_date = mktime(&birth_time);

  // Optionally, set registration and login datetime (for example, current time)
  connected_citizen.registration_datetime = time(NULL); // Current time for registration
  connected_citizen.last_login_datetime = time(NULL);  // Current time for last login
}

void citizen_calc_monthlybill() {
  // Open the binary file for reading
  FILE *file = fopen(PATH_RESERVATION_DATA, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  // Reset the monthly bill for the connected citizen
  connected_citizen.monthly_bill = 0.0f;

  // Temporary variable to hold each reservation
  reservation_t reservation;

  // Iterate through the binary file
  while (fread(&reservation, sizeof(reservation_t), 1, file) == 1) {
    // Check if the reservation belongs to the connected citizen
    if (reservation.id_citizen == connected_citizen.id) {
      // Increment the monthly bill with the cost of the reservation
      connected_citizen.monthly_bill += reservation.cost;
    }
  }

  // Close the file
  fclose(file);
}



void citizen_save() {
  FILE *file = fopen(PATH_CITIZEN_DATA, "rb+");  // Open file for reading and writing in binary mode
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  citizen_t current_citizen;
  size_t citizen_size = sizeof(citizen_t);

  // Iterate through the file, reading one citizen at a time
  while (fread(&current_citizen, citizen_size, 1, file) == 1) {
    // Check if the current citizen has the same ID as the one we want to update
    if (current_citizen.id == connected_citizen.id) {
      // Move the file pointer back to the start of this citizen's record
      fseek(file, -citizen_size, SEEK_CUR);

      // Write the updated citizen record in place of the old one
      if (fwrite(&connected_citizen, citizen_size, 1, file) != 1) {
        perror("Error writing to file");
      }

      // Close the file after updating
      fclose(file);
      return;
    }
  }

  // If no match is found, you might want to handle that case (e.g., log an error)
  printf("Citizen with ID %d not found.\n", connected_citizen.id);

  // Close the file
  fclose(file);
}
