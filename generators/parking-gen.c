#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


typedef enum Availability{
available,
non_available
}availability_t;

typedef struct Parking{
__uint64_t id;
char *location;
unsigned int capacity;
availability_t availability;
float fees;
__uint64_t id_agent;
}parking_t;

#define NUM_PARKINGS 10  // Number of parking instances to generate

// Function to create a random string for location
char* generate_random_location() {
    const char* cities[] = {"New York", "Los Angeles", "Chicago", "Houston", "Phoenix"};
    int num_cities = sizeof(cities) / sizeof(cities[0]);
    char* location = malloc(50);
    snprintf(location, 50, "%s - %d", cities[rand() % num_cities], rand() % 100 + 1);
    return location;
}

// Function to generate a parking instance
parking_t generate_parking_instance(__uint64_t id) {
    parking_t parking;
    parking.id = id;
    parking.location = generate_random_location();
    parking.capacity = rand() % 500 + 50; // Random capacity between 50 and 550
    parking.availability = rand() % 2 ? available : non_available;
    parking.fees = (float)(rand() % 2000) / 100.0; // Random fees between 0 and 20.00
    parking.id_agent = rand() % 100000;  // Random agent ID
    return parking;
}

// Function to write parking instances to a file
void write_parking_data(const char* filename, parking_t* parkings, int count) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        fwrite(&parkings[i].id, sizeof(parkings[i].id), 1, file);

        // Write the length of the location string first
        size_t location_len = strlen(parkings[i].location) + 1; // Include null terminator
        fwrite(&location_len, sizeof(size_t), 1, file);
        fwrite(parkings[i].location, sizeof(char), location_len, file);

        fwrite(&parkings[i].capacity, sizeof(parkings[i].capacity), 1, file);
        fwrite(&parkings[i].availability, sizeof(parkings[i].availability), 1, file);
        fwrite(&parkings[i].fees, sizeof(parkings[i].fees), 1, file);
        fwrite(&parkings[i].id_agent, sizeof(parkings[i].id_agent), 1, file);
    }

    fclose(file);
}

int main() {
    srand((unsigned int)time(NULL)); // Seed the random number generator

    parking_t parkings[NUM_PARKINGS];

    // Generate parking instances
    for (__uint64_t i = 0; i < NUM_PARKINGS; i++) {
        parkings[i] = generate_parking_instance(i + 1);
    }

    // Write parking data to a file
    write_parking_data("parking.dat", parkings, NUM_PARKINGS);

    // Free allocated memory for location strings
    for (int i = 0; i < NUM_PARKINGS; i++) {
        free(parkings[i].location);
    }

    printf("Parking data written to parking.dat successfully!\n");
    return 0;
}
