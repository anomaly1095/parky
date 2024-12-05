#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "citizen.h"
#include "parking.h"
#include "reservation.h"
#include "service.h"
#include "agent.h"
#include "admin.h"

/**
 * 
 * ---------------------------CITIZEN/LOGIN SECTION---------------------------
 * 
 */

// Callback function for the signup button
void signup_clicked(GtkButton *button, gpointer user_data) {
	// Fetch widgets (entries and radio buttons) using lookup_widget or similar methods
	GtkWidget *first_name_entry       = lookup_widget(signup_window, "signup_firstname_entry");
	GtkWidget *last_name_entry        = lookup_widget(signup_window, "signup_lastname_entry");
	GtkWidget *phone_entry            = lookup_widget(signup_window, "signup_phone_entry");
	GtkWidget *email_entry            = lookup_widget(signup_window, "signup_email_entry");
	GtkWidget *address_entry          = lookup_widget(signup_window, "signup_address_entry");
	GtkWidget *password_entry         = lookup_widget(signup_window, "signup_password_entry");
	GtkWidget *password_confirm_entry = lookup_widget(signup_window, "signup_password_confirm_entry");
	GtkWidget *male_radio             = lookup_widget(signup_window, "signup_male_radio");
	GtkWidget *female_radio           = lookup_widget(signup_window, "signup_female_radio");

	// Fetch text from entry fields
	const gchar *first_name       = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
	const gchar *last_name        = gtk_entry_get_text(GTK_ENTRY(last_name_entry));
	const gchar *phone            = gtk_entry_get_text(GTK_ENTRY(phone_entry));
	const gchar *email            = gtk_entry_get_text(GTK_ENTRY(email_entry));
	const gchar *address          = gtk_entry_get_text(GTK_ENTRY(address_entry));
	const gchar *password         = gtk_entry_get_text(GTK_ENTRY(password_entry));
	const gchar *password_confirm = gtk_entry_get_text(GTK_ENTRY(password_confirm_entry));

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
  
  // fetch the id list from the file
	fetch_id_counts();
  
	// Create a new citizen struct
	citizen_create(id_list.id_citizen, first_name, last_name, phone, email, address, gender, password);
  
  // increment number of citizens
	id_list.id_citizen++;

	// Save the citizen data
	citizen_save();
  update_id_counts();
	// Show success message
	GtkWidget *success_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Signup successful!");
	gtk_dialog_run(GTK_DIALOG(success_dialog));
	gtk_widget_destroy(success_dialog);

	// Close signup window (assuming you have a reference to the window)
	gtk_widget_destroy(signup_window);

  signin_window = create_signin_window ();
  gtk_widget_show(signin_window);

}


void
signup_go_signin_clicked (GtkButton *button, gpointer user_data)
{
  // Hide the signup window
	gtk_widget_destroy(signup_window);
  // Show the signin window
  signin_window = create_signin_window ();
  gtk_widget_show(signin_window);
}


void
signin_clicked(GtkButton *button, gpointer user_data) {
  // Retrieve the widgets using lookup_widget
  GtkWidget *signin_email_entry   = lookup_widget(signin_window, "signin_email_entry");
  GtkWidget *signin_password_entry= lookup_widget(signin_window, "signin_password_entry");
  
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

    // Close the sign-in window
    gtk_widget_destroy(signin_window);
    
    citizen_window = create_citizen_window();
    
    // if signed in as citizen fill the fields and display the citizen window
    citizen_account_populate();
    
    gtk_widget_show(citizen_window);
    
  } else if (admin_signin(email, password)){
    // Successful sign-in
    g_print("Welcome, %s %s!\n", admin.first_name, admin.last_name);

    // Close the sign-in window
    gtk_widget_destroy(signin_window);

    // if signed in as admin fill the fields and display the admin window
    // fill citizen 
    admin_window = create_admin_window();
    gtk_widget_show(admin_window);
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
signin_go_signup_clicked (GtkButton *button, gpointer user_data) {
  // destroy the signin window
	gtk_widget_destroy(signin_window);
  // Create and Show the signup window
  signup_window = create_signup_window ();
  gtk_widget_show(signup_window);
}


void citizen_delete_clicked(GtkButton *button, gpointer user_data) {
  // Create the confirmation dialog
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(citizen_window))),
                                             GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_QUESTION,
                                             GTK_BUTTONS_YES_NO,
                                             "Are you sure you want to delete your account?");

  gint response = gtk_dialog_run(GTK_DIALOG(dialog));

  // Handle the user's response
  if (response == GTK_RESPONSE_YES) {
    // Proceed with the deletion
    citizen_delete();

    // Show a success message dialog after deletion
    GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(citizen_window))),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_CLOSE,
                                                       "Citizen account deleted successfully.");
    gtk_dialog_run(GTK_DIALOG(success_dialog));
    gtk_widget_destroy(success_dialog);

    // Optionally, destroy the citizen window and show the signin window
    gtk_widget_destroy(citizen_window);
    signin_window = create_signin_window();
    gtk_widget_show(signin_window);
  }

  // Destroy the confirmation dialog
  gtk_widget_destroy(dialog);
}



void
citizen_modify_clicked(GtkButton *button, gpointer user_data)
{
  guint year, month, day;
  // Lookup the widgets
  GtkWidget *first_name_entry   = lookup_widget(citizen_window, "citizen_modify_firstname_entry");
  GtkWidget *last_name_entry    = lookup_widget(citizen_window, "citizen_modify_lastname_entry");
  GtkWidget *phone_entry        = lookup_widget(citizen_window, "citizen_modify_phone_entry");
  GtkWidget *email_entry        = lookup_widget(citizen_window, "citizen_modify_email_entry");
  GtkWidget *address_entry       = lookup_widget(citizen_window, "citizen_modify_address_entry");
  GtkWidget *password_entry     = lookup_widget(citizen_window, "citizen_modify_password_entry");
  GtkWidget *car_num_entry      = lookup_widget(citizen_window, "citizen_modify_car_entry");
  GtkWidget *male_radio         = lookup_widget(citizen_window, "citizen_modify_male_radio");
  GtkWidget *female_radio       = lookup_widget(citizen_window, "citizen_modify_female_radio");
  GtkWidget *datebirth_calendar = lookup_widget(citizen_window, "citizen_modify_datebirth_calendar");

  // Retrieve text from GtkEntry widgets
  const gchar *first_name = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
  const gchar *last_name  = gtk_entry_get_text(GTK_ENTRY(last_name_entry));
  const gchar *phone      = gtk_entry_get_text(GTK_ENTRY(phone_entry));
  const gchar *email      = gtk_entry_get_text(GTK_ENTRY(email_entry));
  const gchar *address     = gtk_entry_get_text(GTK_ENTRY(address_entry));
  const gchar *password   = gtk_entry_get_text(GTK_ENTRY(password_entry));
  const gchar *car_num    = gtk_entry_get_text(GTK_ENTRY(car_num_entry));
  gboolean is_male        = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(male_radio));
  gboolean is_female      = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(female_radio));

  gtk_calendar_get_date(GTK_CALENDAR(datebirth_calendar), &year, &month, &day);
  month += 1; // GTK 2 calendar months are 0-based

  // Create a struct tm and populate it
  struct tm birth_time = {0};
  birth_time.tm_year = year - 1900; // tm_year is years since 1900
  birth_time.tm_mon = month - 1;    // tm_mon is 0-based
  birth_time.tm_mday = day;
  time_t birth_timestamp = mktime(&birth_time);

  // update connected citizen with new values
  citizen_modify(first_name, last_name, phone, email, password, car_num, address, birth_time, is_male, is_female);
  // save the struct in the file
  citizen_save();
}



void
citizen_modify_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data){
  citizen_modify_populate(); // put back the old values in the entries
}


/**
 * 
 * ---------------------------PARKING SECTION---------------------------
 * 
 */

void
parking_create_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
parking_create_confirm_clicked         (GtkButton       *button,
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

/**
 * 
 * ---------------------------SERVICE SECTION---------------------------
 * 
 */


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



/**
 * 
 * ---------------------------AGENT SECTION---------------------------
 * 
 */


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
agent_delete_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
agent_reservation_search_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}

/**
 * 
 * ---------------------------RESERVATION SECTION---------------------------
 * 
 */


void
reservation_display_find_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_display_refresh_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_delete_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_create_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_calcul_refresh_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_calcul_find_by_id_res_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
reservation_calcul_find_by_id_park_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

}

