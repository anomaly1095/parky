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


/// @brief fills citizen modify fields with details from connected citizen
/// @param window CITIZEN WINDOW WIDGET
void citizen_modify_populate_fields(GtkWidget *window) {
  // Lookup the notebook widget (assuming you have a GtkNotebook)
  GtkWidget *notebook = lookup_widget(window, "citizen_modify_notebook");

  // Lookup the widgets inside the first page of the notebook (e.g., citizen modify tab)
  GtkWidget *first_name_entry     = lookup_widget(window, "citizen_modify_firstname_entry");
  GtkWidget *last_name_entry     = lookup_widget(window, "citizen_modify_lastname_entry");
  GtkWidget *phone_entry         = lookup_widget(window, "citizen_modify_phone_entry");
  GtkWidget *email_entry         = lookup_widget(window, "citizen_modify_email_entry");
  GtkWidget *address_combobox    = lookup_widget(window, "citizen_modify_address_comboboxentry");
  GtkWidget *street_entry        = lookup_widget(window, "citizen_modify_street_entry");
  GtkWidget *password_entry      = lookup_widget(window, "citizen_modify_password_entry");
  GtkWidget *car_num_entry       = lookup_widget(window, "citizen_modify_car_entry");
  GtkWidget *male_radio          = lookup_widget(window, "citizen_modify_male_radio");
  GtkWidget *female_radio        = lookup_widget(window, "citizen_modify_female_radio");
  GtkWidget *datebirth_calendar  = lookup_widget(window, "citizen_modify_datebirth_calendar");

  // Set the values of the widgets to match the connected_citizen data

  // Set GtkEntry fields
  gtk_entry_set_text(GTK_ENTRY(first_name_entry), (char *)connected_citizen.first_name);
  gtk_entry_set_text(GTK_ENTRY(last_name_entry), (char *)connected_citizen.last_name);
  gtk_entry_set_text(GTK_ENTRY(phone_entry), (char *)connected_citizen.phone);
  gtk_entry_set_text(GTK_ENTRY(email_entry), (char *)connected_citizen.email);
  gtk_entry_set_text(GTK_ENTRY(street_entry), (char *)connected_citizen.address); // Assuming street is part of the address field
  gtk_entry_set_text(GTK_ENTRY(password_entry), (char *)connected_citizen.password);
  gtk_entry_set_text(GTK_ENTRY(car_num_entry), (char *)connected_citizen.vehicle_num);

  // Set the active text in the ComboBox for address (if using combined address and street)
  gtk_combo_box_set_active_text(GTK_COMBO_BOX(address_combobox), (char *)connected_citizen.address);

  // Set the gender radio buttons based on connected_citizen.gender
  if (connected_citizen.gender == male) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(male_radio), TRUE);
  } else if (connected_citizen.gender == female) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(female_radio), TRUE);
  }

  // Set the calendar widget's date
  struct tm *birth_tm = localtime(&connected_citizen.birth_date);
  gtk_calendar_select_month(GTK_CALENDAR(datebirth_calendar), birth_tm->tm_mon, birth_tm->tm_year);
  gtk_calendar_select_day(GTK_CALENDAR(datebirth_calendar), birth_tm->tm_mday);
}

/// @brief fills citizen details firlds from connected citizen
/// @param window CITIZEN WINDOW WIDGET
void citizen_details_populate_fields(GtkWidget *window) {
  // Lookup the widgets for the citizen details window
  GtkWidget *first_name_entry       = lookup_widget(window, "citizen_details_firstname_entry");
  GtkWidget *last_name_entry       = lookup_widget(window, "citizen_details_lastname_entry");
  GtkWidget *address_entry         = lookup_widget(window, "citizen_details_address_entry");
  GtkWidget *phone_entry           = lookup_widget(window, "citizen_details_phone_entry");
  GtkWidget *id_entry              = lookup_widget(window, "citizen_details_id_entry");
  GtkWidget *reservations_entry    = lookup_widget(window, "citizen_details_reservations_entry");
  GtkWidget *monthlybill_entry     = lookup_widget(window, "citizen_details_monthlybill_entry");
  GtkWidget *password_entry        = lookup_widget(window, "citizen_details_password_entry");  // Optional for display
  GtkWidget *registration_entry    = lookup_widget(window, "citizen_details_registration_entry");
  GtkWidget *email_entry           = lookup_widget(window, "citizen_details_email_entry");
  GtkWidget *datebirth_entry       = lookup_widget(window, "citizen_details_datebirth_entry");
  GtkWidget *male_radio            = lookup_widget(window, "citizen_details_male_radio");
  GtkWidget *female_radio          = lookup_widget(window, "citizen_details_female_radio");

  // Set the values of the widgets to match the connected_citizen data

  // Set GtkEntry fields
  gtk_entry_set_text(GTK_ENTRY(first_name_entry), (char *)connected_citizen.first_name);
  gtk_entry_set_text(GTK_ENTRY(last_name_entry), (char *)connected_citizen.last_name);
  gtk_entry_set_text(GTK_ENTRY(address_entry), (char *)connected_citizen.address);
  gtk_entry_set_text(GTK_ENTRY(phone_entry), (char *)connected_citizen.phone);
  gtk_entry_set_text(GTK_ENTRY(id_entry), g_strdup_printf("%llu", connected_citizen.id)); // For the id, format it as uint64_t
  gtk_entry_set_text(GTK_ENTRY(reservations_entry), g_strdup_printf("%d", connected_citizen.total_reservations));
  gtk_entry_set_text(GTK_ENTRY(monthlybill_entry), g_strdup_printf("%.2f", connected_citizen.monthly_bill));
  gtk_entry_set_text(GTK_ENTRY(email_entry), (char *)connected_citizen.email);

  // Display registration datetime (optional; format as desired)
  char reg_datetime_str[64];
  struct tm *reg_tm = localtime(&connected_citizen.registration_datetime);
  strftime(reg_datetime_str, sizeof(reg_datetime_str), "%Y-%m-%d %H:%M:%S", reg_tm);
  gtk_entry_set_text(GTK_ENTRY(registration_entry), reg_datetime_str);

  // Display date of birth (optional; format as desired)
  char birth_datetime_str[64];
  struct tm *birth_tm = localtime(&connected_citizen.birth_date);
  strftime(birth_datetime_str, sizeof(birth_datetime_str), "%Y-%m-%d", birth_tm);
  gtk_entry_set_text(GTK_ENTRY(datebirth_entry), birth_datetime_str);

  // Set the gender radio buttons based on connected_citizen.gender
  if (connected_citizen.gender == male)
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(male_radio), true);
  else if (connected_citizen.gender == female)
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(female_radio), true);
}
