#include <stdio.h>

// Stores information for one seat

struct seat {
    int id_number;
    int assignment_status;
    char passenger_lastname[50];
    char passenger_firstname[50];
};

struct seat outbound_seats[24];
struct seat inbound_seats[24];

void outbound_menu(void);
void inbound_menu(void);
void load_data(void);
void save_data(void);

// Set all seats to empty at startup
void initialize_seats(void) {
    for (int i = 0; i < 24; i++) {
        outbound_seats[i].id_number = i + 1;
        outbound_seats[i].assignment_status = 0;

        inbound_seats[i].id_number = i + 1;
        inbound_seats[i].assignment_status = 0;
    }
}

// Check that a saved name contains valid characters
int valid_name(char name[50]) {

    int i = 0;

    while (i < 50 && name[i] != '\0') {

        unsigned char ch = name[i];

        if (ch < 32 || ch > 126) {
            return 0;
        }

        i++;
    }

    if (i == 50) {
        return 0;
    }

    return 1;
}

// Load saved seat data from the file
void load_data(void) {

    // Open the saved data file
    FILE *file = fopen("flight_data.bin", "rb");

    if (file == NULL) {
        // Stop if the file cannot be opened

        initialize_seats();

        printf("Error: File could not be loaded.\n");

        return;
    }

    // Find the file size before reading it
    if (fseek(file, 0, SEEK_END) != 0) {

        printf("Error: Unable to determine file size.");

        fclose(file);

        initialize_seats();

        return;
    }

    long file_size = ftell(file);

    // Make sure the file has exactly 48 seats
    if (file_size != sizeof(struct seat) * 48) {

        printf("Error: Invalid flight data file size.");

        fclose(file);

        initialize_seats();

        return;
    }

    // Go back to the beginning of the file
    if (fseek(file, 0, SEEK_SET) != 0) {

        printf("Error: Unable to reset file position.");

        fclose(file);

        initialize_seats();

        return;
    }

    // Read the outbound seats
    size_t result = fread(outbound_seats, sizeof(struct seat), 24, file);

    if (result != 24) {

        printf("Error: Unable to load outbound flight data.");

        fclose(file);

        initialize_seats();

        return;
    }

    // Read the inbound seats
    result = fread(inbound_seats, sizeof(struct seat), 24, file);

    // Check the loaded data for invalid values

    if (result != 24) {

        printf("Error: Unable to load inbound flight data.");

        fclose(file);

        initialize_seats();

        return;
    }

    for (int i = 0; i < 24; i++) {

        if (outbound_seats[i].id_number != i + 1 ||
            inbound_seats[i].id_number != i + 1) {

            printf("Error: Invalid seat number in flight data.");

            fclose(file);

            initialize_seats();

            return;
        }

        if ((outbound_seats[i].assignment_status != 0 &&
             outbound_seats[i].assignment_status != 1) ||
            (inbound_seats[i].assignment_status != 0 &&
             inbound_seats[i].assignment_status != 1)) {

            printf("Error: Invalid assignment status in flight data.");

            fclose(file);

            initialize_seats();

            return;
        }

        if (outbound_seats[i].assignment_status == 1) {

            if (!valid_name(outbound_seats[i].passenger_firstname) ||
                !valid_name(outbound_seats[i].passenger_lastname)) {

                printf("Error: Invalid passenger name in flight data.");

                fclose(file);

                initialize_seats();

                return;
            }
        }

        if (inbound_seats[i].assignment_status == 1) {

            if (!valid_name(inbound_seats[i].passenger_firstname) ||
                !valid_name(inbound_seats[i].passenger_lastname)) {

                printf("Error: Invalid passenger name in flight data.");

                fclose(file);

                initialize_seats();

                return;
            }
        }
    }

    // Close the file
    if (fclose(file) != 0) {

        printf("Error: Unable to close file.");
    }
}

// Save the current seat data to the file
void save_data(void) {

    // Write to a temporary file first
    FILE *file = fopen("flight_data.tmp", "wb");

    if (file == NULL) {
        // Stop if the file cannot be opened

        printf("Error: File could not be opened for saving.\n");

        return;
    }

    // Save the outbound seats
    size_t result = fwrite(outbound_seats, sizeof(struct seat), 24, file);

    if (result != 24) {

        printf("Error: Unable to save outbound flight data.");

        fclose(file);

        return;
    }

    // Save the inbound seats
    result = fwrite(inbound_seats, sizeof(struct seat), 24, file);

    if (result != 24) {

        printf("Error: Unable to save inbound flight data.");

        fclose(file);

        return;
    }

    // Make sure all data is written
    if (fflush(file) != 0) {

        printf("Error: Unable to flush flight data.");

        fclose(file);

        return;
    }

    // Close the file
    if (fclose(file) != 0) {

        printf("Error: Unable to close temporary flight data.");

        return;
    }

    // Replace the old file with the new file
    if (rename("flight_data.tmp", "flight_data.bin") != 0) {

        printf("Error: Unable to replace flight data file.");

        return;
    }
}

// Show the main menu
void first_level_menu(void) {
    char choice;

    while (1) {

        printf("First-Level Menu: \n");
        printf("a) Outbound Flight \n");
        printf("b) Inbound Flight \n");
        printf("c) Quit \n");

        printf("Choose an option: ");
        if (scanf(" %c", &choice) != 1) {
            return;
        }

        // Handle the selected menu option
        switch(choice) {
            case 'a':
                outbound_menu();
                break;

            case 'b':
                inbound_menu();
                break;

            case 'c':
                save_data();
                printf("Goodbye!\n");
                return;

            default:
                printf("Invalid selection. Please choose a, b, or c.\n");
                break;
        }
    }
}

// Handle the outbound flight menu
void outbound_menu(void) {
    char choice;

    while (1) {

        printf("a) Show number of empty seats \n");
        printf("b) Show list of empty seats \n");
        printf("c) Show alphabetical list of seats \n");
        printf("d) Assign a customer to a seat assignment \n");
        printf("e) Delete a seat assignment \n");
        printf("f) Return to Main Menu \n");

        printf("Choose an option: ");
        if (scanf(" %c", &choice) != 1) {
            return;
        }

        // Handle the selected menu option
        switch(choice) {
            case 'a': {
                // Count empty seats
                int empty_seats = 0;

            for (int i = 0; i < 24; i++) {
                    if (outbound_seats[i].assignment_status == 0) {
                        empty_seats++;
                    }
                }
            
                printf("Number of empty seats: %d\n", empty_seats);

                break;
            }

            case 'b': {
                // Show every empty seat
                printf("List of empty seats:\n");

            for (int i = 0; i < 24; i++) {
                    if (outbound_seats[i].assignment_status == 0) {
                        printf("%d\n", outbound_seats[i].id_number);
                    }
                }

                break;
            }

            case 'c': {
                // Sort seat indexes by passenger last name
                int order[24];
                int temp;

                // Put the seat indexes into the array
            for (int i = 0; i < 24; i++) {
                    order[i] = i;
                }

                // Sort the indexes alphabetically by last name
                for (int i = 0; i < 23; i++) {
                    for (int j = i + 1; j < 24; j++) {

                        int k = 0;

                        while (outbound_seats[order[i]].passenger_lastname[k] != '\0' &&
                            outbound_seats[order[j]].passenger_lastname[k] != '\0' &&
                            outbound_seats[order[i]].passenger_lastname[k] ==
                            outbound_seats[order[j]].passenger_lastname[k]) {
                            k++;
                        }

                        if (outbound_seats[order[i]].passenger_lastname[k] >
                            outbound_seats[order[j]].passenger_lastname[k]) {

                            temp = order[i];
                            order[i] = order[j];
                            order[j] = temp;
                        }
                    }
                }

                printf("Alphabetical list of seats:\n");

            for (int i = 0; i < 24; i++) {
                    int seat = order[i];

                    if (outbound_seats[seat].assignment_status == 1) {
                        printf("%s, %s - Seat %d\n",
                            outbound_seats[seat].passenger_lastname,
                            outbound_seats[seat].passenger_firstname,
                            outbound_seats[seat].id_number);
                    }
                }

                break;
            }
            
            case 'd': {
                // Assign a passenger to a seat
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                // Make sure the seat number is valid
                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                // Do not overwrite an occupied seat
                if (outbound_seats[seat - 1].assignment_status == 1) {
                    printf("That seat is occupied.");
                    break;
                }

                // Get the passenger first name
                printf("Enter the passenger's first name (-1 to cancel): ");
                scanf(" %49[^\n]", outbound_seats[seat - 1].passenger_firstname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (outbound_seats[seat - 1].passenger_firstname[0] == '-' &&
                    outbound_seats[seat - 1].passenger_firstname[1] == '1' &&
                    outbound_seats[seat - 1].passenger_firstname[2] == '\0') {

                    printf("Entry aborted.\n");
                    break;
                }

                // Get the passenger last name
                printf("Enter the passenger's last name: ");
                scanf(" %49[^\n]", outbound_seats[seat - 1].passenger_lastname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (outbound_seats[seat - 1].passenger_lastname[0] == '-' &&
                    outbound_seats[seat - 1].passenger_lastname[1] == '1' &&
                    outbound_seats[seat - 1].passenger_lastname[2] == '\0') {

                    printf("Entry aborted.\n");
                    break;
                }

                // Mark the seat as occupied
                outbound_seats[seat - 1].assignment_status = 1;

                printf("Seat assigned successfully!\n");

                break;
            }

            case 'e': {
                // Delete a passenger from a seat
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                // Make sure the seat number is valid
                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                if (outbound_seats[seat - 1].assignment_status == 0) {
                    printf("That seat is already empty.");
                    break;
                }

                // Mark the seat as empty
                outbound_seats[seat - 1].assignment_status = 0;
                outbound_seats[seat - 1].passenger_firstname[0] = '\0';
                outbound_seats[seat - 1].passenger_lastname[0] = '\0';

                printf("Seat assignment deleted.");

                break;
            }

            case 'f': {
                return;
            }

            default: {
                printf("Invalid selection. Please choose an option a-f.");
                break;
            }
        }
    }
}

// Handle the inbound flight menu
void inbound_menu(void) {
    char choice;

    while (1) {

        printf("a) Show number of empty seats \n");
        printf("b) Show list of empty seats \n");
        printf("c) Show alphabetical list of seats \n");
        printf("d) Assign a customer to a seat assignment \n");
        printf("e) Delete a seat assignment \n");
        printf("f) Return to Main Menu \n");

        printf("Choose an option: ");
        if (scanf(" %c", &choice) != 1) {
            return;
        }

        // Handle the selected menu option
        switch(choice) {
            case 'a': {
                // Count empty seats
                int empty_seats = 0;

            for (int i = 0; i < 24; i++) {
                    if (inbound_seats[i].assignment_status == 0) {
                        empty_seats++;
                    }
                }
            
                printf("Number of empty seats: %d\n", empty_seats);

                break;
            }

            case 'b': {
                // Show every empty seat
                printf("List of empty seats:\n");

            for (int i = 0; i < 24; i++) {
                    if (inbound_seats[i].assignment_status == 0) {
                        printf("%d\n", inbound_seats[i].id_number);
                    }
                }

                break;
            }

            case 'c': {
                // Sort seat indexes by passenger last name
                int order[24];
                int temp;

                // Put the seat indexes into the array
            for (int i = 0; i < 24; i++) {
                    order[i] = i;
                }

                // Sort the indexes alphabetically by last name
                for (int i = 0; i < 23; i++) {
                    for (int j = i + 1; j < 24; j++) {

                        int k = 0;

                        while (inbound_seats[order[i]].passenger_lastname[k] != '\0' &&
                            inbound_seats[order[j]].passenger_lastname[k] != '\0' &&
                            inbound_seats[order[i]].passenger_lastname[k] ==
                            inbound_seats[order[j]].passenger_lastname[k]) {
                            k++;
                        }

                        if (inbound_seats[order[i]].passenger_lastname[k] >
                            inbound_seats[order[j]].passenger_lastname[k]) {

                            temp = order[i];
                            order[i] = order[j];
                            order[j] = temp;
                        }
                    }
                }

                printf("Alphabetical list of seats:\n");

            for (int i = 0; i < 24; i++) {
                    int seat = order[i];

                    if (inbound_seats[seat].assignment_status == 1) {
                        printf("%s, %s - Seat %d\n",
                            inbound_seats[seat].passenger_lastname,
                            inbound_seats[seat].passenger_firstname,
                            inbound_seats[seat].id_number);
                    }
                }

                break;
            }
            
            case 'd': {
                // Assign a passenger to a seat
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                // Make sure the seat number is valid
                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                // Do not overwrite an occupied seat
                if (inbound_seats[seat - 1].assignment_status == 1) {
                    printf("That seat is occupied.");
                    break;
                }

                // Get the passenger first name
                printf("Enter the passenger's first name (-1 to cancel): ");
                scanf(" %49[^\n]", inbound_seats[seat - 1].passenger_firstname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (inbound_seats[seat - 1].passenger_firstname[0] == '-' &&
                    inbound_seats[seat - 1].passenger_firstname[1] == '1' &&
                    inbound_seats[seat - 1].passenger_firstname[2] == '\0') {

                    printf("Entry aborted.\n");
                    break;
                }

                // Get the passenger last name
                printf("Enter the passenger's last name: ");
                scanf(" %49[^\n]", inbound_seats[seat - 1].passenger_lastname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (inbound_seats[seat - 1].passenger_lastname[0] == '-' &&
                    inbound_seats[seat - 1].passenger_lastname[1] == '1' &&
                    inbound_seats[seat - 1].passenger_lastname[2] == '\0') {

                    printf("Entry aborted.\n");
                    break;
                }

                // Mark the seat as occupied
                inbound_seats[seat - 1].assignment_status = 1;

                printf("Seat assigned successfully!\n");

                break;
            }

            case 'e': {
                // Delete a passenger from a seat
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    // Clear the rest of the input line
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                // Make sure the seat number is valid
                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                if (inbound_seats[seat - 1].assignment_status == 0) {
                    printf("That seat is already empty.");
                    break;
                }

                // Mark the seat as empty
                inbound_seats[seat - 1].assignment_status = 0;
                inbound_seats[seat - 1].passenger_firstname[0] = '\0';
                inbound_seats[seat - 1].passenger_lastname[0] = '\0';

                printf("Seat assignment deleted.");

                break;
            }

            case 'f': {
                return;
            }

            default: {
                printf("Invalid selection. Please choose an option a-f.");
                break;
            }
        }
    }
}

int main() {
    // Load saved data before showing the menus
    load_data();

    // Start the main menu
    first_level_menu();

    return 0;
}