#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "citizen.h"

// Callback function for the signup button
void signup_clicked(GtkButton *button, gpointer user_data) {
	// Fetch widgets (entries and radio buttons) using lookup_widget or similar methods
	GtkWidget *first_name_entry       = lookup_widget(GTK_WIDGET(button), "signup_firstname_entry");
	GtkWidget *last_name_entry       = lookup_widget(GTK_WIDGET(button), "signup_lastname_entry");
	GtkWidget *phone_entry           = lookup_widget(GTK_WIDGET(button), "signup_phone_entry");
	GtkWidget *email_entry           = lookup_widget(GTK_WIDGET(button), "signup_email_entry");
	GtkWidget *address_entry         = lookup_widget(GTK_WIDGET(button), "signup_address_entry");
	GtkWidget *password_entry        = lookup_widget(GTK_WIDGET(button), "signup_password_entry");
	GtkWidget *password_confirm_entry = lookup_widget(GTK_WIDGET(button), "signup_password_confirm_entry");
	GtkWidget *male_radio            = lookup_widget(GTK_WIDGET(button), "signup_male_radio");
	GtkWidget *female_radio          = lookup_widget(GTK_WIDGET(button), "signup_female_radio");

	// Fetch text from entry fields
	const gchar *first_name       = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
	const gchar *last_name       = gtk_entry_get_text(GTK_ENTRY(last_name_entry));
	const gchar *phone           = gtk_entry_get_text(GTK_ENTRY(phone_entry));
	const gchar *email           = gtk_entry_get_text(GTK_ENTRY(email_entry));
	const gchar *address         = gtk_entry_get_text(GTK_ENTRY(address_entry));
	const gchar *password        = gtk_entry_get_text(GTK_ENTRY(password_entry));
	const gchar *password_confirm = gtk_entry_get_text(GTK_ENTRY(password_confirm_entry));

	// fetch the id list from the file
	void fetch_id_counts();
	// increment number of citizens
	citizen_connected_id = id_list.id_citizen; 
	id_list.id_citizen++;

	// Fetch gender selection
	gender_t gender;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(male_radio)))
		gender = male;  // Assuming you have a GENDER_MALE enum value
	else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(female_radio)))
		gender = female;  // Assuming you have a GENDER_FEMALE enum value
	else
		gender = male;


	// Check if passwords match
	if (!citizen_pwd_is_same(password, password_confirm)) {
		// Error, passwords do not match, show a message to the user
		GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Passwords do not match!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	// Create a new citizen struct
	citizen_t new_citizen = citizen_create(citizen_connected_id, first_name, last_name, phone, email, address, gender, password);

	// Save the citizen data
	citizen_save(new_citizen);

	// Show success message
	GtkWidget *success_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Signup successful!");
	gtk_dialog_run(GTK_DIALOG(success_dialog));
	gtk_widget_destroy(success_dialog);

	// Close signup window (assuming you have a reference to the window)
	GtkWidget *signup_window = lookup_widget(GTK_WIDGET(button), "signup_window");
	gtk_widget_destroy(signup_window);

	// You could also handle redirecting to the signin screen here, if needed
}


void
signin_clicked(GtkButton *button, gpointer user_data) {
  // Retrieve the widgets using lookup_widget
  GtkWidget *signin_email_entry   = lookup_widget(GTK_WIDGET(button), "signin_email_entry");
  GtkWidget *signin_password_entry= lookup_widget(GTK_WIDGET(button), "signin_password_entry");
  GtkWidget *signin_window        = lookup_widget(GTK_WIDGET(button), "signin_window");

  // Ensure widgets were retrieved
  if (!signin_email_entry || !signin_password_entry || !signin_window) {
    g_print("Error: Failed to retrieve necessary widgets for sign-in.\n");
    return;
  }

  // Get the text from the email and password entry widgets
  const gchar *email = gtk_entry_get_text(GTK_ENTRY(signin_email_entry));
  const gchar *password = gtk_entry_get_text(GTK_ENTRY(signin_password_entry));

  // Attempt to sign in and immediately check the result
  if (citizen_signin(email, password)) {
    // Successful sign-in
    g_print("Welcome, %s %s!\n", connected_citizen.first_name, connected_citizen.last_name);

    // Close the sign-in window
    gtk_widget_destroy(signin_window);
    // display citizen window
  } else if (admin_signin(email, password)){
    // Successful sign-in
    g_print("Welcome, %s %s!\n", connected_citizen.first_name, connected_citizen.last_name);

    // Close the sign-in window
    gtk_widget_destroy(signin_window);
    // display admin window
  } else {
    // Failed sign-in
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(signin_window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE,
                                               "Sign-in failed. Please check your email and password.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
}

void
citizen_delete_clicked(GtkButton *button, gpointer user_data) {
  // Call the citizen_delete function to perform the deletion
  citizen_delete();

  // Optionally, you can display a confirmation message or close the window after deletion
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(button),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_INFO,
                                             GTK_BUTTONS_CLOSE,
                                             "Citizen account deleted successfully.");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  // You may also want to log out the user or perform additional actions after deletion
  // e.g., clear the connected_citizen data
  memset(&connected_citizen, 0, sizeof(citizen_t));
}


void
citizen_modify_clicked(GtkButton *button, gpointer user_data)
{
  // Lookup the widgets
  GtkWidget *first_name_entry    = lookup_widget(GTK_WIDGET(button), "citizen_modify_firstname_entry");
  GtkWidget *last_name_entry    = lookup_widget(GTK_WIDGET(button), "citizen_modify_lastname_entry");
  GtkWidget *phone_entry        = lookup_widget(GTK_WIDGET(button), "citizen_modify_phone_entry");
  GtkWidget *email_entry        = lookup_widget(GTK_WIDGET(button), "citizen_modify_email_entry");
  GtkWidget *address_combobox   = lookup_widget(GTK_WIDGET(button), "citizen_modify_address_comboboxentry");
  GtkWidget *street_entry       = lookup_widget(GTK_WIDGET(button), "citizen_modify_street_entry");
  GtkWidget *password_entry     = lookup_widget(GTK_WIDGET(button), "citizen_modify_password_entry");
  GtkWidget *car_num_entry      = lookup_widget(GTK_WIDGET(button), "citizen_modify_car_entry");
  GtkWidget *male_radio         = lookup_widget(GTK_WIDGET(button), "citizen_modify_male_radio");
  GtkWidget *female_radio       = lookup_widget(GTK_WIDGET(button), "citizen_modify_female_radio");
  GtkWidget *datebirth_calendar = lookup_widget(GTK_WIDGET(button), "citizen_modify_datebirth_calendar");

  // Retrieve text from GtkEntry widgets
  const gchar *first_name = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
  const gchar *last_name = gtk_entry_get_text(GTK_ENTRY(last_name_entry));
  const gchar *phone     = gtk_entry_get_text(GTK_ENTRY(phone_entry));
  const gchar *email     = gtk_entry_get_text(GTK_ENTRY(email_entry));
  const gchar *street    = gtk_entry_get_text(GTK_ENTRY(street_entry));
  const gchar *password  = gtk_entry_get_text(GTK_ENTRY(password_entry));
  const gchar *car_num   = gtk_entry_get_text(GTK_ENTRY(car_num_entry));

  // Retrieve text from GtkComboBoxEntry widget
  const gchar *address = gtk_combo_box_get_active_text(GTK_COMBO_BOX(address_combobox));

  // Retrieve active state of GtkRadioButtons
  gboolean is_male = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(male_radio));
  gboolean is_female = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(female_radio));

  // Retrieve date from GtkCalendar widget
  guint year, month, day;
  gtk_calendar_get_date(GTK_CALENDAR(datebirth_calendar), &year, &month, &day);
  month += 1; // GTK 2 calendar months are 0-based

  // Create a struct tm and populate it
  struct tm birth_time = {0};
  birth_time.tm_year = year - 1900; // tm_year is years since 1900
  birth_time.tm_mon = month - 1;    // tm_mon is 0-based
  birth_time.tm_mday = day;

  // Convert struct tm to time_t
  time_t birth_timestamp = mktime(&birth_time);
  // update connected citizen with new values
  citizen_modify(first_name, last_name, phone, email, street, password, car_num, address, birth_time);
  // save the struct in the file
  citizen_save();
}



void
citizen_modify_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  
}


void
parking_create_confirm_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  
}


void
parking_create_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
parking_edit_cancel_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
parking_edit_apply_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
parking_delete_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
parking_delete_confirm_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
parking_display_search_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_display_search_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_create_add_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_update_apply_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_delete_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_post_update_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_post_research_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_add_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_add_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_modify_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_modify_cancel_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_delete_return_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_delete_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
admin_reservation_update_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
admin_reservation_search_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_refresh_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_read_search_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_update_check_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_update_add_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_delete_search_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_delete_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_delete_close_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_id_confirm_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_create_add_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_calcul_find_id_res_clicked (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_id_park_find_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_affect_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_affect_close_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
service_display_reservation_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_list_add_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_list_delete_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
signup_go_signin_button                (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
signin_go_signup_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}

