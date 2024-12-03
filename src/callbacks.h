#include <gtk/gtk.h>

/**
 * 
 * ---------------------------CITIZEN/LOGIN SECTION---------------------------
 * 
 */

void
signup_clicked                         (GtkButton       *button,
                                        gpointer         user_data);

void
signup_go_signin_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
signin_clicked                         (GtkButton       *button,
                                        gpointer         user_data);

void
signin_go_signup_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
citizen_delete_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
citizen_modify_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
citizen_modify_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data);

/**
 * 
 * ---------------------------PARKING SECTION---------------------------
 * 
 */

void
parking_create_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
parking_create_confirm_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
parking_edit_cancel_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
parking_edit_apply_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
parking_delete_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
parking_delete_confirm_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
parking_display_search_clicked         (GtkButton       *button,
                                        gpointer         user_data);

/**
 * 
 * ---------------------------SERVICE SECTION---------------------------
 * 
 */

void
service_display_search_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
service_create_add_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
service_update_apply_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
service_delete_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

/**
 * 
 * ---------------------------AGENT SECTION---------------------------
 * 
 */

void
agent_post_research_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
agent_add_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
agent_add_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
agent_modify_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
agent_modify_cancel_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
agent_delete_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
agent_reservation_search_clicked       (GtkButton       *button,
                                        gpointer         user_data);

/**
 * 
 * ---------------------------RESERVATION SECTION---------------------------
 * 
 */


void
reservation_display_find_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
reservation_display_refresh_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
reservation_delete_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
reservation_create_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
reservation_calcul_refresh_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
reservation_calcul_find_by_id_res_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
reservation_calcul_find_by_id_park_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);
