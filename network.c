#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "billing.h"

pthread_mutex_t ledger_mutex = PTHREAD_MUTEX_INITIALIZER;

void append_to_ledger(UsageRecord *record) {
    pthread_mutex_lock(&ledger_mutex);
    FILE *fp = fopen(LEDGER_FILE, "ab");
    if (fp) {
        fwrite(record, sizeof(UsageRecord), 1, fp);
        fclose(fp);
    }
    pthread_mutex_unlock(&ledger_mutex);
}


void *client_handler(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);
    
    UsageRecord record;
    int metrics_count = 0;
    
    // Keep reading chunks from the socket network stream directly into our memory layout
    while (recv(client_fd, &record, sizeof(UsageRecord), 0) > 0) {
        metrics_count++;
        append_to_ledger(&record);
        
        // Auto-update live balances on check-in
        Account acc;
        if (db_read_account(record.account_id, &acc) == 0) {
            acc.balance_due += calculate_usage_fee(&record);
            db_update_account(&acc);
            
            // AUTOMATED TASK 2 EXPORT: Automatically generate/refresh the JSON invoice on disk!
            generate_invoice_json(&acc, &record, metrics_count, "invoice.json");
        }
    }
    close(client_fd);
    return NULL;
}


void start_ingestion_server(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return;
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) return;
    if (listen(server_fd, 5) < 0) return;
    
    printf("[SERVER] Concurrent ingestion engine listening on port %d...\n", port);
    
    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);
        if (*client_fd >= 0) {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, client_handler, client_fd);
            pthread_detach(thread_id); // Automatic thread memory clean-up on exit
        } else {
            free(client_fd);
        }
    }
}