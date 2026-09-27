#include <stdio.h>

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

void initialize_seats(void) {
    for (int i = 0; i < 24; i++) {
        outbound_seats[i].id_number = i + 1;
        outbound_seats[i].assignment_status = 0;

        inbound_seats[i].id_number = i + 1;
        inbound_seats[i].assignment_status = 0;
    }
}

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

        switch(choice) {
            case 'a':
                outbound_menu();
                break;

            case 'b':
                inbound_menu();
                break;

            case 'c':
                printf("Goodbye!\n");
                return;

            default:
                printf("Invalid selection. Please choose a, b, or c.\n");
                break;
        }
    }
}

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

        switch(choice) {
            case 'a': {
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
                printf("List of empty seats:\n");

                for (int i = 0; i < 24; i++) {
                    if (outbound_seats[i].assignment_status == 0) {
                        printf("%d\n", outbound_seats[i].id_number);
                    }
                }

                break;
            }

            case 'c': {
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
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                if (outbound_seats[seat - 1].assignment_status == 1) {
                    printf("That seat is occupied.");
                    break;
                }

                printf("Enter the passenger's first name (-1 to cancel): ");
                scanf(" %49[^\n]", outbound_seats[seat - 1].passenger_firstname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

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

                printf("Enter the passenger's last name: ");
                scanf(" %49[^\n]", outbound_seats[seat - 1].passenger_lastname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

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

                outbound_seats[seat - 1].assignment_status = 1;

                printf("Seat assigned successfully!\n");

                break;
            }

            case 'e': {
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                if (outbound_seats[seat - 1].assignment_status == 0) {
                    printf("That seat is already empty.");
                    break;
                }

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

        switch(choice) {
            case 'a': {
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
                printf("List of empty seats:\n");

                for (int i = 0; i < 24; i++) {
                    if (inbound_seats[i].assignment_status == 0) {
                        printf("%d\n", inbound_seats[i].id_number);
                    }
                }

                break;
            }

            case 'c': {
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
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                if (inbound_seats[seat - 1].assignment_status == 1) {
                    printf("That seat is occupied.");
                    break;
                }

                printf("Enter the passenger's first name (-1 to cancel): ");
                scanf(" %49[^\n]", inbound_seats[seat - 1].passenger_firstname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

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

                printf("Enter the passenger's last name: ");
                scanf(" %49[^\n]", inbound_seats[seat - 1].passenger_lastname);

                if (getchar() != '\n') {
                    printf("Name is too long. Please enter 49 characters or fewer.\n");

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

                inbound_seats[seat - 1].assignment_status = 1;

                printf("Seat assigned successfully!\n");

                break;
            }

            case 'e': {
                int seat;

                printf("Enter a seat number (-1 to cancel): ");
                if (scanf("%d", &seat) != 1) {
                    printf("Invalid selection.\n");

                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    break;
                }

                if (seat == -1) {
                    printf("Entry aborted.\n");
                    break;
                }

                if (seat < 1 || seat > 24) {
                    printf("Invalid selection.\n");
                    break;
                }

                if (inbound_seats[seat - 1].assignment_status == 0) {
                    printf("That seat is already empty.");
                    break;
                }

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
    initialize_seats();

    first_level_menu();

    return 0;
}