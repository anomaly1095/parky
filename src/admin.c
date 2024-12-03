#include "admin.h"

admin_t admin = {
  .id = 0,
  .first_name = "Admin",
  .last_name = "Admin",
  .email = "Admin@esprit.tn",
  .phone = "+21699999999",
  .password = "0000",
};

bool admin_signin(const char *email, const char *password) {
  if ((strcmp(admin.email, email) == 0) && (strcmp(admin.password, password) == 0))
    return true; // Successful sign-in
  return false; // Sign-in failed
}