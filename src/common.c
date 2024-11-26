#include "common.h"

void fetch_id_counts() {
  FILE* ids_file = fopen(PATH_ID_DATA, "rb");
  
  // Check if the file opened successfully
  if (ids_file == NULL) {
    perror("Error opening id file for reading");
    exit(EXIT_FAILURE);  // Handle the error as appropriate for your application
  }
  
  // Attempt to read from the file
  if (fread((void*)&id_list, sizeof id_list, 1, ids_file) != 1) {
    perror("Error reading from id file");
    fclose(ids_file);
    exit(EXIT_FAILURE);  // Handle the error as appropriate
  }

  // Close the file and check if it closed successfully
  if (fclose(ids_file) != 0) {
    perror("Error closing id file after reading");
    exit(EXIT_FAILURE);
  }
}

void save_id_counts() {
  FILE* ids_file = fopen(PATH_ID_DATA, "wb");
  
  // Check if the file opened successfully
  if (ids_file == NULL) {
    perror("Error opening id file for writing");
    exit(EXIT_FAILURE);  // Handle the error as appropriate
  }
  
  // Attempt to write to the file
  if (fwrite((const void*)&id_list, sizeof id_list, 1, ids_file) != 1) {
    perror("Error writing to id file");
    fclose(ids_file);
    exit(EXIT_FAILURE);  // Handle the error as appropriate
  }

  // Close the file and check if it closed successfully
  if (fclose(ids_file) != 0) {
    perror("Error closing id file after writing");
    exit(EXIT_FAILURE);
  }
}

void time_to_datetime_string(time_t timestamp, char *datetime_str, size_t max_len) {
    struct tm *tm_info;

    // Convert time_t to struct tm
    tm_info = localtime(&timestamp);

    // Format the datetime into the provided string
    strftime(datetime_str, max_len, "%Y-%m-%d %H:%M:%S", tm_info);
}