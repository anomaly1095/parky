#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PATH_CITIZEN_DATA "citizen.dat"
#define MAX_NAME_LENGTH         100U
#define MAX_PHONE_LENGTH         20U
#define MAX_EMAIL_LENGTH        100U
#define MAX_ADDRESS_LENGTH      200U
#define MAX_PASSWORD_LENGTH      64U
#define MAX_VEHICLE_NUM_LENGTH   16U

typedef enum Gender {
    male,
    female
} gender_t;

typedef struct Citizen {
    __uint64_t id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    gender_t gender;
    char phone[MAX_PHONE_LENGTH];
    float monthly_bill;
    __int16_t total_reservations;
    char vehicle_num[MAX_VEHICLE_NUM_LENGTH];
    time_t birth_date;
    time_t registration_datetime;
    time_t last_login_datetime;
} citizen_t;

// Helper function to generate a unique email
void generate_unique_email(char *email, int index) {
    snprintf(email, MAX_EMAIL_LENGTH, "user%d@example.com", index);
}

// Function to create citizen records and write to file
void generate_citizen_data(int num_citizens) {
    FILE *file = fopen(PATH_CITIZEN_DATA, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < num_citizens; ++i) {
        citizen_t citizen;

        // Assign default values
        citizen.id = i + 1;
        strcpy(citizen.first_name, "qqq");
        strcpy(citizen.last_name, "qqq");
        generate_unique_email(citizen.email, i + 1);
        strcpy(citizen.password, "qqq");
        strcpy(citizen.address, "qqq");
        citizen.gender = male;
        strcpy(citizen.phone, "qqq");
        citizen.monthly_bill = 0.0f;
        citizen.total_reservations = 0;
        strcpy(citizen.vehicle_num, "qqq");
        citizen.birth_date = time(NULL);
        citizen.registration_datetime = time(NULL);
        citizen.last_login_datetime = time(NULL);

        // Write citizen to file
        fwrite(&citizen, sizeof(citizen_t), 1, file);
    }

    fclose(file);
    printf("Generated %d citizens in '%s'\n", num_citizens, PATH_CITIZEN_DATA);
}

int main() {
    int num_citizens;

    printf("Enter the number of citizens to generate: ");
    scanf("%d", &num_citizens);

    generate_citizen_data(num_citizens);

    return 0;
}
