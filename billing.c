#include <stdio.h>
#include "billing.h"


// Setting up and calculating fees
double calculate_usage_fee(UsageRecord *record) {
    // Define standard pricing tiers
    double cpu_hourly_rate = 0.05;
    double ram_hourly_rate = 0.02;
    double network_mb_rate = 0.01;

    // Calculate core resource fees
    double cpu_cost = record->cpu_cores * cpu_hourly_rate * record->hours_used;
    double ram_cost = record->ram_gb * ram_hourly_rate * record->hours_used;
    double network_cost = record->outbound_mb * network_mb_rate;

    // Total up the bill for this incoming metric session
    double total_fee = cpu_cost + ram_cost + network_cost;

    return total_fee;
}


// Outputs structured customer profile text using clear formatting flags
void generate_invoice_json(Account *account, UsageRecord *records, int count, const char *filepath) {
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        perror("Failed to create invoice file");
        return;
    }

    // Print JSON opening braces
    fprintf(fp, "{\n");
    fprintf(fp, "  \"account_id\": %u,\n", account->account_id);
    fprintf(fp, "  \"customer_name\": \"%s\",\n", account->customer_name);
    fprintf(fp, "  \"total_balance_due_usd\": %.2f,\n", account->balance_due);
    
    // Nested array block to show individual raw records processed
    fprintf(fp, "  \"metrics_processed_count\": %d,\n", count);
    fprintf(fp, "  \"status\": \"Generated Successfully\"\n");
    fprintf(fp, "}\n");

    fclose(fp);
}