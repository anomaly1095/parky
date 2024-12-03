#include "citizen.h"


// This session's connected citizen
citizen_t connected_citizen;
__uint64_t citizen_connected_id;


bool citizen_pwd_is_same(const char *password1, const char *password2) {
  // Compare strings using strcmp (returns 0 if the strings are equal)
  if (strcmp(password1, password2) == 0)
      return true;  // Passwords are the same
  else
      return false;  // Passwords are different
}

citizen_t citizen_create(__uint64_t id, const char *first_name, const char *last_name, const char *phone, const char *email, const char *address, gender_t gender, const char *password) {

  citizen_t new_citizen = {
    .id = id,
    .gender = gender,
    .monthly_bill = 0.0f,
    .birth_date = time(NULL),
    .registration_datetime = time(NULL),
    .vehicle_num = "None",
    .last_login_datetime = time(NULL)  // Set to current time
  };

  // Copy the strings to the respective arrays in the new_citizen struct
  strncpy((char *)new_citizen.first_name, (char *)first_name, MAX_NAME_LENGTH - 1);
  strncpy((char *)new_citizen.last_name, (char *)last_name, MAX_NAME_LENGTH - 1);
  strncpy((char *)new_citizen.phone, (char *)phone, MAX_PHONE_LENGTH - 1);
  strncpy((char *)new_citizen.email, (char *)email, MAX_EMAIL_LENGTH - 1);
  strncpy((char *)new_citizen.address, (char *)address, MAX_ADDRESS_LENGTH - 1);
  strncpy((char *)new_citizen.password, (char *)password, MAX_PASSWORD_LENGTH - 1);

  return new_citizen;
}




void citizen_delete() {
  FILE *file = fopen(PATH_CITIZEN_DATA, "ab+");  // Open file for reading and writing in binary mode
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

bool citizen_signin(const char *email, const char *password) {
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
    if ((strcmp((char *)current_citizen.email, (char *)email) == 0) && (strcmp((char *)current_citizen.password, (char *)password) == 0)) {
      fclose(file);
      // Citizen account found, assign to the global variable
      connected_citizen = current_citizen;
      return true;
    }
  }

  // If no match is found
  fclose(file);
  printf("No account found with the given email and password.\n");
  return false;
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
void citizen_modify(const char *first_name, const char *last_name, const char *phone,
                    const char *email, const char *street, const char *password,
                    const char *car_num, const char *address, struct tm birth_time, gboolean is_male, gboolean is_female) {

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
    if (reservation.id_citizen == connected_citizen.id)
      // Increment the monthly bill with the cost of the reservation
      connected_citizen.monthly_bill += reservation.cost;
  }

  // Close the file
  fclose(file);
}

void citizen_get_bills(double bills[31]) {
  // Open the binary file for reading
  FILE *file = fopen(PATH_RESERVATION_DATA, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  // Clear the bills array
  for (int i = 0; i < 31; i++)
    bills[i] = 0.0;

  // Get the current month and year
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  int current_year = current_time->tm_year + 1900; // tm_year is years since 1900
  int current_month = current_time->tm_mon;       // tm_mon is 0-based (0 = January)

  // Temporary variable to hold each reservation
  reservation_t reservation;

  // Iterate through the binary file
  while (fread(&reservation, sizeof(reservation_t), 1, file) == 1) {
    // Check if the reservation belongs to the connected citizen
    if (reservation.id_citizen == connected_citizen.id) {
      // Convert reservation datetime to a struct tm
      struct tm *reservation_time = localtime(&reservation.reservation_datetime);
      int reservation_year = reservation_time->tm_year + 1900;
      int reservation_month = reservation_time->tm_mon;
      int reservation_day = reservation_time->tm_mday;

      // Check if the reservation is in the current month and year
      if (reservation_year == current_year && reservation_month == current_month)
        // Add the reservation cost to the appropriate day
        bills[reservation_day - 1] += reservation.cost;
    }
  }

  // Close the file
  fclose(file);
}

void citizen_save() {
  FILE *file = fopen(PATH_CITIZEN_DATA, "ab+");  // Open file for reading and writing in binary mode
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  citizen_t current_citizen;
  size_t citizen_size = sizeof(citizen_t);

  // Try to read the first citizen
  if (fread(&current_citizen, citizen_size, 1, file))
    // Iterate through the file, reading one citizen at a time
    while (fread(&current_citizen, citizen_size, 1, file)) {
      // Check if the current citizen has the same ID as the one we want to update
      if (current_citizen.id == connected_citizen.id) {
        // Move the file pointer back to the start of this citizen's record
        fseek(file, -citizen_size, SEEK_CUR);

        // Write the updated citizen record in place of the old one
        if (fwrite(&connected_citizen, citizen_size, 1, file) != 1)
          perror("Error writing to file");

        // Close the file after updating
        fclose(file);
        return;
      }
    }
  else {
    // If file is empty or no citizen was read, append the new citizen
    if (fwrite(&connected_citizen, citizen_size, 1, file) != 1)
      perror("Error writing to file");
  }

  // If no match is found, you might want to handle that case (e.g., log an error)
  printf("Citizen with ID %d not found. Adding new citizen.\n", connected_citizen.id);

  // Close the file
  fclose(file);
}


/// @brief fills citizen details firlds from connected citizen
/// @param window CITIZEN WINDOW WIDGET
void citizen_details_populate() {
  // Lookup the widgets for the citizen details window
  GtkWidget *first_name_entry       = lookup_widget(citizen_window, "citizen_details_firstname_entry");
  GtkWidget *last_name_entry       = lookup_widget(citizen_window, "citizen_details_lastname_entry");
  GtkWidget *address_entry         = lookup_widget(citizen_window, "citizen_details_address_entry");
  GtkWidget *phone_entry           = lookup_widget(citizen_window, "citizen_details_phone_entry");
  GtkWidget *id_entry              = lookup_widget(citizen_window, "citizen_details_id_entry");
  GtkWidget *reservations_entry    = lookup_widget(citizen_window, "citizen_details_reservations_entry");
  GtkWidget *monthlybill_entry     = lookup_widget(citizen_window, "citizen_details_monthlybill_entry");
  GtkWidget *registration_entry    = lookup_widget(citizen_window, "citizen_details_registration_entry");
  GtkWidget *email_entry           = lookup_widget(citizen_window, "citizen_details_email_entry");
  GtkWidget *datebirth_entry       = lookup_widget(citizen_window, "citizen_details_datebirth_entry");
  GtkWidget *male_radio            = lookup_widget(citizen_window, "citizen_details_male_radio");
  GtkWidget *female_radio          = lookup_widget(citizen_window, "citizen_details_female_radio");

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


/// @brief fills citizen modify fields with details from connected citizen
/// @param window CITIZEN WINDOW WIDGET
void citizen_modify_populate() {
  // Lookup the widgets inside the first page of the notebook (e.g., citizen modify tab)
  GtkWidget *first_name_entry     = lookup_widget(citizen_window, "citizen_modify_firstname_entry");
  GtkWidget *last_name_entry      = lookup_widget(citizen_window, "citizen_modify_lastname_entry");
  GtkWidget *phone_entry          = lookup_widget(citizen_window, "citizen_modify_phone_entry");
  GtkWidget *email_entry          = lookup_widget(citizen_window, "citizen_modify_email_entry");
  GtkWidget *address_combobox     = lookup_widget(citizen_window, "citizen_modify_address_comboboxentry");
  GtkWidget *street_entry         = lookup_widget(citizen_window, "citizen_modify_street_entry");
  GtkWidget *password_entry       = lookup_widget(citizen_window, "citizen_modify_password_entry");
  GtkWidget *car_num_entry        = lookup_widget(citizen_window, "citizen_modify_car_entry");
  GtkWidget *male_radio           = lookup_widget(citizen_window, "citizen_modify_male_radio");
  GtkWidget *female_radio         = lookup_widget(citizen_window, "citizen_modify_female_radio");
  GtkWidget *datebirth_calendar   = lookup_widget(citizen_window, "citizen_modify_datebirth_calendar");

  // Set the values of the widgets to match the connected_citizen data

  // Set GtkEntry fields
  gtk_entry_set_text(GTK_ENTRY(first_name_entry), (char *)connected_citizen.first_name);
  gtk_entry_set_text(GTK_ENTRY(last_name_entry), (char *)connected_citizen.last_name);
  gtk_entry_set_text(GTK_ENTRY(phone_entry), (char *)connected_citizen.phone);
  gtk_entry_set_text(GTK_ENTRY(email_entry), (char *)connected_citizen.email);
  gtk_entry_set_text(GTK_ENTRY(street_entry), (char *)connected_citizen.address); // Assuming street is part of the address field
  gtk_entry_set_text(GTK_ENTRY(password_entry), (char *)connected_citizen.password);
  gtk_entry_set_text(GTK_ENTRY(car_num_entry), (char *)connected_citizen.vehicle_num);

  // Set the active item in the ComboBox for address (manual method)
  GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(address_combobox));
  GtkTreeIter iter;
  gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

  while (valid) {
    gchar *item_text;
    gtk_tree_model_get(model, &iter, 0, &item_text, -1);  // Get the text in the first column

    // Compare the item with the connected_citizen.address
    if (g_strcmp0(item_text, connected_citizen.address) == 0) {
      // Set the active index when a match is found
      gtk_combo_box_set_active(GTK_COMBO_BOX(address_combobox), gtk_tree_model_iter_n_children(model, NULL));
      g_free(item_text);
      return;
    }

      g_free(item_text);
      valid = gtk_tree_model_iter_next(model, &iter);  // Go to the next item
  }

  // If the item wasn't found, you might want to handle this case (optional)
  g_warning("Address '%s' not found in the combo box model.", connected_citizen.address);

  // Set the gender radio buttons based on connected_citizen.gender
  if (connected_citizen.gender == male)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(male_radio), TRUE);
  else if (connected_citizen.gender == female)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(female_radio), TRUE);

  // Set the calendar widget's date
  struct tm *birth_tm = localtime(&connected_citizen.birth_date);
  gtk_calendar_select_month(GTK_CALENDAR(datebirth_calendar), birth_tm->tm_mon, birth_tm->tm_year);
  gtk_calendar_select_day(GTK_CALENDAR(datebirth_calendar), birth_tm->tm_mday);
}


/**
 * Populate the citizen_monthlybill_curve widget with reservation data.
 *
 * @param curve A pointer to the GtkCurve widget to populate.
 * @param reservations An array of reservation values, where each index
 *                     corresponds to a day of the month (1-based index).
 * @param num_days The number of days in the month (e.g., 28, 30, or 31).
 */
void citizen_monthlybill_curve_populate(GtkCurve *curve, const double *reservations, int num_days) {
  // Ensure the input data is valid
  if (!curve || !reservations || num_days <= 0) {
    g_warning("Invalid parameters passed to populate_monthlybill_curve.");
    return;
  }

  // Normalize the X-axis (days) and Y-axis (reservation values in dinars)
  // X-axis values range from 0.0 to 1.0 for days of the month
  // Y-axis values range from 0.0 to 1.0 for the reservation values
  gfloat curve_points[num_days * 2]; // Use gfloat instead of gdouble for compatibility

  double max_value = 0.0;

  // Find the maximum value in the reservations array for normalization
  for (int i = 0; i < num_days; i++) {
    if (reservations[i] > max_value)
      max_value = reservations[i];
  }

  // Avoid division by zero if all values are zero
  if (max_value == 0.0)
    max_value = 1.0;

  // Populate the points for the curve
  for (int i = 0; i < num_days; i++) {
    float day_normalized = (float)i / (num_days - 1); // Normalize X (0.0 to 1.0)
    float value_normalized = (float)(reservations[i] / max_value); // Normalize Y (0.0 to 1.0)
    curve_points[i * 2] = day_normalized;         // X-coordinate
    curve_points[i * 2 + 1] = value_normalized;   // Y-coordinate
  }

  // Set the range of the curve
  gtk_curve_set_range(curve, 0.0, 1.0, 0.0, 1.0);

  // Apply the points to the curve
  gtk_curve_set_vector(curve, num_days, curve_points);

  // Force the widget to redraw
  gtk_widget_queue_draw(GTK_WIDGET(curve));
}

void citizen_account_populate(){
  double bills[31];
  citizen_details_populate();
  citizen_modify_populate();
  GtkWidget *monthly_bill_curve = lookup_widget(citizen_window, "citizen_monthlybill_curve");
  citizen_get_bills(bills);
  citizen_monthlybill_curve_populate(GTK_CURVE(monthly_bill_curve), bills, 31);
}
