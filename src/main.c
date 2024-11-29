#include "interface.h" // Provides create_signup_window and create_signin_window
#include "support.h"   // Provides initialization functions and helpers
#include "common.h"

GtkWidget *signin_window, *signup_window, *admin_window, *citizen_window, *service_display_dialog;

int main(int argc, char *argv[]) {

  // Initialize GTK
  gtk_init(&argc, &argv);

  // Create the sign-in window
  signin_window = create_signin_window();
  signup_window = create_signup_window();
  admin_window= create_signup_window();
  citizen_window = create_signup_window();
  service_display_dialog = create_signup_window();

  
  // Check if the window was created successfully
  if (!signin_window) {
    g_error("Failed to create the sign-in window.");
    return FAILURE;
  }
  // Check if the window was created successfully
  if (!signup_window) {
    g_error("Failed to create the sign-up window.");
    return FAILURE;
  }
  // Check if the window was created successfully
  if (!admin_window) {
    g_error("Failed to create the admin window.");
    return FAILURE;
  }
  // Check if the window was created successfully
  if (!citizen_window) {
    g_error("Failed to create the citizen window.");
    return FAILURE;
  }
  // Check if the window was created successfully
  if (!service_display_dialog) {
    g_error("Failed to create the service dialog window.");
    return FAILURE;
  }

  // Show the sign-in window
  gtk_widget_show(signin_window); // initially displayed
  // all the rest are initially hidden and should be displayed in callback functions
  gtk_widget_hide(signup_window);
  gtk_widget_hide(admin_window);
  gtk_widget_hide(citizen_window);
  gtk_widget_hide(service_display_dialog);

  // Enter the GTK main event loop
  gtk_main();

  return SUCCESS;
}
