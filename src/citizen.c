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
      .last_login_datetime = time(NULL)  // Set to current time
  };

  return new_citizen;
}



void citizen_delete(__uint64_t id) {
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
    if (current_citizen.id == id) {
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
    printf("Citizen with ID %llu deleted.\n", id);
  } else {
    printf("Citizen with ID %llu not found.\n", id);
    fclose(file);
  }
}


citizen_t citizen_fetch(__uint64_t id) {
  FILE *file = fopen(PATH_CITIZEN_DATA, "rb");  // Open file for reading in binary mode
  if (file == NULL) {
    perror("Error opening file");
    citizen_t empty_citizen = {0};  // Return an empty citizen structure on error
    return empty_citizen;
  }

  citizen_t current_citizen;
  size_t citizen_size = sizeof(citizen_t);

  // Iterate through the file, reading one citizen at a time
  while (fread(&current_citizen, citizen_size, 1, file) == 1) {
    if (current_citizen.id == id) {
      fclose(file);
      return current_citizen;  // Return the citizen if the ID matches
    }
  }

  // If no matching citizen is found, return an empty citizen structure
  fclose(file);
  citizen_t empty_citizen = {0};  // Return an empty citizen structure if not found
  return empty_citizen;
}


void citizen_modify(__uint64_t id){

}


void citizen_calc_monthlybill(){

}


void citizen_save(citizen_t *citizen) {
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
    if (current_citizen.id == citizen->id) {
      // Move the file pointer back to the start of this citizen's record
      fseek(file, -citizen_size, SEEK_CUR);

      // Write the updated citizen record in place of the old one
      if (fwrite(citizen, citizen_size, 1, file) != 1) {
        perror("Error writing to file");
      }

      // Close the file after updating
      fclose(file);
      return;
    }
  }

  // If no match is found, you might want to handle that case (e.g., log an error)
  printf("Citizen with ID %d not found.\n", citizen->id);

  // Close the file
  fclose(file);
}