#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MAX_NAME_LENGTH         100U
#define MAX_PHONE_LENGTH         20U
#define MAX_EMAIL_LENGTH        100U
#define MAX_ADDRESS_LENGTH      200U
#define MAX_PASSWORD_LENGTH      64U
#define MAX_VEHICLE_NUM_LENGTH   16U

typedef enum Availability {
    available,
    non_available
} availability_t;

typedef struct Agent {
    __uint64_t id;
    char *first_name;
    char *last_name;
    char *address;
    char *phone_num;
    time_t hiring_datetime;
    __uint64_t id_parking;
    availability_t availability;
} agent_t;

#define NUM_AGENTS 10  // Number of agent instances to generate

// Function to generate a random string for names
char* generate_random_name() {
    const char* names[] = {"John", "Jane", "Michael", "Sarah", "David", "Emma", "Chris", "Sophia", "James", "Olivia"};
    int num_names = sizeof(names) / sizeof(names[0]);
    char *name = malloc(MAX_NAME_LENGTH);
    snprintf(name, MAX_NAME_LENGTH, "%s", names[rand() % num_names]);
    return name;
}

// Function to generate a random address
char* generate_random_address() {
    const char* streets[] = {"Main St", "2nd Ave", "Park Blvd", "Broadway", "Elm St"};
    int num_streets = sizeof(streets) / sizeof(streets[0]);
    char *address = malloc(MAX_ADDRESS_LENGTH);
    snprintf(address, MAX_ADDRESS_LENGTH, "%d %s", rand() % 9999 + 1, streets[rand() % num_streets]);
    return address;
}

// Function to generate a random phone number
char* generate_random_phone() {
    char *phone = malloc(MAX_PHONE_LENGTH);
    snprintf(phone, MAX_PHONE_LENGTH, "(%03d) %03d-%04d", rand() % 900 + 100, rand() % 900 + 100, rand() % 10000);
    return phone;
}

// Function to generate a random hiring datetime
time_t generate_random_hiring_datetime() {
    time_t now = time(NULL);
    return now - (rand() % (365 * 24 * 60 * 60 * 5)); // Random time within the last 5 years
}

// Function to generate an agent instance
agent_t generate_agent_instance(__uint64_t id) {
    agent_t agent;
    agent.id = id;

    agent.first_name = generate_random_name();
    agent.last_name = generate_random_name();
    agent.address = generate_random_address();
    agent.phone_num = generate_random_phone();

    agent.hiring_datetime = generate_random_hiring_datetime();
    agent.id_parking = rand() % 1000; // Random parking ID
    agent.availability = rand() % 2 ? available : non_available;

    return agent;
}

// Function to write agent instances to a file
void write_agent_data(const char *filename, agent_t *agents, int count) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        fwrite(&agents[i].id, sizeof(agents[i].id), 1, file);

        size_t length;

        // Write first name
        length = strlen(agents[i].first_name) + 1;
        fwrite(&length, sizeof(size_t), 1, file);
        fwrite(agents[i].first_name, sizeof(char), length, file);

        // Write last name
        length = strlen(agents[i].last_name) + 1;
        fwrite(&length, sizeof(size_t), 1, file);
        fwrite(agents[i].last_name, sizeof(char), length, file);

        // Write address
        length = strlen(agents[i].address) + 1;
        fwrite(&length, sizeof(size_t), 1, file);
        fwrite(agents[i].address, sizeof(char), length, file);

        // Write phone number
        length = strlen(agents[i].phone_num) + 1;
        fwrite(&length, sizeof(size_t), 1, file);
        fwrite(agents[i].phone_num, sizeof(char), length, file);

        // Write other fields
        fwrite(&agents[i].hiring_datetime, sizeof(agents[i].hiring_datetime), 1, file);
        fwrite(&agents[i].id_parking, sizeof(agents[i].id_parking), 1, file);
        fwrite(&agents[i].availability, sizeof(agents[i].availability), 1, file);
    }

    fclose(file);
}

int main() {
    srand((unsigned int)time(NULL)); // Seed the random number generator

    agent_t agents[NUM_AGENTS];

    // Generate agent instances
    for (__uint64_t i = 0; i < NUM_AGENTS; i++) {
        agents[i] = generate_agent_instance(i + 1);
    }

    // Write agent data to a file
    write_agent_data("agents.dat", agents, NUM_AGENTS);

    // Free allocated memory for dynamic strings
    for (int i = 0; i < NUM_AGENTS; i++) {
        free(agents[i].first_name);
        free(agents[i].last_name);
        free(agents[i].address);
        free(agents[i].phone_num);
    }

    printf("Agent data written to agents.dat successfully!\n");
    return 0;
}
