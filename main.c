#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "billing.h"

int main() {
    printf("=== Initializing Cloud Billing Database Mock Tests ===\n");
    
    // Setup clean mock database profile tests
    Account client1 = {1001, "Acme MegaCorp", 0.0, 1};
    db_create_account(&client1);
    
    Account check;
    if (db_read_account(1001, &check) == 0) {
        printf("Successfully Created & Read Account! Customer: %s\n", check.customer_name);
    }

    // Launching Server Ingestion Thread Loop
    // For manual local testing, you can hit this socket engine using client simulations
    start_ingestion_server(8080);

    return 0;
}