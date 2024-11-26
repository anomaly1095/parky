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
	GtkWidget *first_name_entry = lookup_widget(GTK_WIDGET(button), "first_name_entry");
	GtkWidget *last_name_entry = lookup_widget(GTK_WIDGET(button), "last_name_entry");
	GtkWidget *phone_entry = lookup_widget(GTK_WIDGET(button), "phone_entry");
	GtkWidget *email_entry = lookup_widget(GTK_WIDGET(button), "email_entry");
	GtkWidget *address_entry = lookup_widget(GTK_WIDGET(button), "address_entry");
	GtkWidget *password_entry = lookup_widget(GTK_WIDGET(button), "password_entry");
	GtkWidget *password_confirm_entry = lookup_widget(GTK_WIDGET(button), "password_confirm_entry");
	GtkWidget *male_radio = lookup_widget(GTK_WIDGET(button), "male_radio");
	GtkWidget *female_radio = lookup_widget(GTK_WIDGET(button), "female_radio");

	// Fetch text from entry fields
	const __int8_t *first_name = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
	const __int8_t *last_name = gtk_entry_get_text(GTK_ENTRY(last_name_entry));
	const __int8_t *phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));
	const __int8_t *email = gtk_entry_get_text(GTK_ENTRY(email_entry));
	const __int8_t *address = gtk_entry_get_text(GTK_ENTRY(address_entry));
	const __int8_t *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
	const __int8_t *password_confirm = gtk_entry_get_text(GTK_ENTRY(password_confirm_entry));

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
  GtkWidget *signin_email_entry = lookup_widget(GTK_WIDGET(button), "signin_email_entry");
  GtkWidget *signin_password_entry = lookup_widget(GTK_WIDGET(button), "signin_password_entry");
  GtkWidget *signin_window = lookup_widget(GTK_WIDGET(button), "signin_window");

  // Ensure widgets were retrieved
  if (!signin_email_entry || !signin_password_entry || !signin_window) {
    g_print("Error: Failed to retrieve necessary widgets for sign-in.\n");
    return;
  }

  // Get the text from the email and password entry widgets
  const __uint8_t *email = gtk_entry_get_text(GTK_ENTRY(signin_email_entry));
  const __uint8_t *password = gtk_entry_get_text(GTK_ENTRY(signin_password_entry));

  // Attempt to sign in and immediately check the result
  if (citizen_signin(email, password)) {
    // Successful sign-in
    g_print("Welcome, %s %s!\n", connected_citizen.first_name, connected_citizen.last_name);

    // Close the sign-in window
    gtk_widget_destroy(signin_window);
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
on_Service_main_search_button_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_Service_Delete_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_B_Ac_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_cancel_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_ADD_A_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_B_S_clicked                         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_details_delete_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_modify_modify_button_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
	
}


void
citizen_modify_cancel_button_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_modify_createcar_button_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_modify_password_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_B_M_clicked                         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_monthlybill_services_button_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_monthlybill_reservations_button_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_monthlybill_parkings_button_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_signup_cancel_button_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
citizen_signup_confirm_button_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{

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
citizen_modify_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    
}


void
citizen_modify_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}

