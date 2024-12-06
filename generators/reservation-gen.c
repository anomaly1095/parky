#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "reservation.h"

#define PATH_RESERVATION_DATA "reservation.dat"

// Helper function to generate a random float in a range
float generate_random_float(float min, float max) {
    return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

// Helper function to generate a random timestamp
time_t generate_random_timestamp(int year, int month) {
    struct tm time_info = {0};
    time_info.tm_year = year - 1900;    // tm_year is years since 1900
    time_info.tm_mon = month - 1;      // tm_mon is 0-based
    time_info.tm_mday = rand() % 28 + 1; // Random day of the month (1-28)
    time_info.tm_hour = rand() % 24;
    time_info.tm_min = rand() % 60;
    time_info.tm_sec = rand() % 60;
    return mktime(&time_info);
}

void generate_reservation_data(int num_reservations) {
    FILE *file = fopen(PATH_RESERVATION_DATA, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    srand((unsigned int)time(NULL)); // Seed the random number generator

    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1; // tm_mon is 0-based (0 = January)

    for (int i = 0; i < num_reservations; ++i) {
        reservation_t reservation;

        // Fill in the reservation data
        reservation.id = rand() % 1000000; // Random ID
        reservation.id_parking = rand() % 100; // Random parking ID
        reservation.id_service = rand() % 50;  // Random service ID
        reservation.id_citizen = rand() % 1000; // Random citizen ID
        reservation.cost = generate_random_float(10.0f, 100.0f); // Random cost
        snprintf(reservation.description, sizeof(reservation.description), "Reservation #%llu", reservation.id);

        // Decide if the reservation is for the current month or a random other month
        if (rand() % 2 == 0) {
            // Current month
            reservation.reservation_datetime = generate_random_timestamp(current_year, current_month);
        } else {
            // Random other month (in the same year or adjacent year)
            int random_month = rand() % 12 + 1;
            int random_year = current_year + (rand() % 2 ? 1 : -1); // Randomly next or previous year
            reservation.reservation_datetime = generate_random_timestamp(random_year, random_month);
        }

        // Generate start and end times for the reservation
        reservation.start_datetime = reservation.reservation_datetime;
        reservation.end_datetime = reservation.start_datetime + (rand() % 3600 + 1800); // Random duration (30-90 min)

        // Write the reservation to the binary file
        fwrite(&reservation, sizeof(reservation_t), 1, file);
    }

    fclose(file);
    printf("Generated %d reservations in '%s'\n", num_reservations, PATH_RESERVATION_DATA);
}

int main() {
    int num_reservations;

    printf("Enter the number of reservations to generate: ");
    scanf("%d", &num_reservations);

    generate_reservation_data(num_reservations);

    return 0;
}

