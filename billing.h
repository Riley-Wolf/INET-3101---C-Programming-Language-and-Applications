#ifndef BILLING_H
#define BILLING_H

#include <stdint.h>

#define MAX_NAME 50
#define DB_FILE "accounts.bin"
#define LEDGER_FILE "ledger.bin"

// Fixed-width Account Struct
typedef struct {
    uint32_t account_id;
    char customer_name[MAX_NAME];
    double balance_due;
    int32_t is_active; // 1 = Active, 0 = Soft Deleted
} Account;

// Usage Record Ingested Over Sockets
typedef struct {
    uint32_t account_id;
    uint32_t cpu_cores;
    uint32_t ram_gb;
    double outbound_mb;
    uint32_t hours_used;
} UsageRecord;

// Database Engine Functions (CRUD)
int db_create_account(Account *acc);
int db_read_account(uint32_t account_id, Account *acc_out);
int db_update_account(Account *acc);
int db_delete_account(uint32_t account_id);

// Network Server Ingestion Engine
void start_ingestion_server(int port);
void append_to_ledger(UsageRecord *record);

// Business Logic Functions (YOUR TASKS GO HERE)
double calculate_usage_fee(UsageRecord *record);
void generate_invoice_json(Account *account, UsageRecord *records, int count, const char *filepath);

#endif