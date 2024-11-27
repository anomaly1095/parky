#include "main.h"




int main(int argc, const char **argv){
  // display signin
  // if admin connect as admin and open admin window
  // if citizen connect as citizen and open citizen window
  return 0;
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
