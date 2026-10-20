#include <stdio.h>
#include <string.h>
#include "billing.h"

// CREATE: Appends an account to the binary file
int db_create_account(Account *acc) {
    FILE *fp = fopen(DB_FILE, "ab");
    if (!fp) return -1;
    acc->is_active = 1;
    fwrite(acc, sizeof(Account), 1, fp);
    fclose(fp);
    return 0;
}

// READ: Searches sequentially for a matching active record
int db_read_account(uint32_t account_id, Account *acc_out) {
    FILE *fp = fopen(DB_FILE, "rb");
    if (!fp) return -1;
    
    while (fread(acc_out, sizeof(Account), 1, fp) == 1) {
        if (acc_out->account_id == account_id && acc_out->is_active == 1) {
            fclose(fp);
            return 0; // Found
        }
    }
    fclose(fp);
    return -1; // Not found
}

// UPDATE: Modifies fields of an account directly in place
int db_update_account(Account *acc) {
    FILE *fp = fopen(DB_FILE, "rb+");
    if (!fp) return -1;
    
    Account temp;
    long pos;
    while (fread(&temp, sizeof(Account), 1, fp) == 1) {
        if (temp.account_id == acc->account_id && temp.is_active == 1) {
            pos = ftell(fp) - sizeof(Account);
            fseek(fp, pos, SEEK_SET);
            fwrite(acc, sizeof(Account), 1, fp);
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return -1;
}

// DELETE: Performs a safe "Soft Delete" flag modification
int db_delete_account(uint32_t account_id) {
    FILE *fp = fopen(DB_FILE, "rb+");
    if (!fp) return -1;
    
    Account temp;
    long pos;
    while (fread(&temp, sizeof(Account), 1, fp) == 1) {
        if (temp.account_id == account_id && temp.is_active == 1) {
            temp.is_active = 0; // Soft delete
            pos = ftell(fp) - sizeof(Account);
            fseek(fp, pos, SEEK_SET);
            fwrite(&temp, sizeof(Account), 1, fp);
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return -1;
}