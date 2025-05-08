#include <stdio.h>
#include <string.h>
#include <math.h>

// Function to calculate LRC (Longitudinal Redundancy Check)
void calculateLRC(char data[][8], int rows, int cols, char lrc[]) {
    for (int j = 0; j < cols; j++) {
        int parity = 0;
        for (int i = 0; i < rows; i++) {
            parity ^= (data[i][j] - '0');
        }
        lrc[j] = parity + '0';
    }
    lrc[cols] = '\0';
}

// Function to calculate VRC (Vertical Redundancy Check)
void calculateVRC(char data[], int len, char *vrc) {
    int parity = 0;
    for (int i = 0; i < len; i++) {
        parity ^= (data[i] - '0');
    }
    *vrc = parity + '0';
}

// Function to compute CRC (Cyclic Redundancy Check)
void computeCRC(char data[], char poly[], char crc[]) {
    int dataLen = strlen(data), polyLen = strlen(poly);
    char temp[32];
    strcpy(temp, data);
    for (int i = 0; i < polyLen - 1; i++) {
        strcat(temp, "0");
    }
    for (int i = 0; i < dataLen; i++) {
        if (temp[i] == '1') {
            for (int j = 0; j < polyLen; j++) {
                temp[i + j] = (temp[i + j] == poly[j]) ? '0' : '1';
            }
        }
    }
    strncpy(crc, temp + dataLen, polyLen - 1);
    crc[polyLen - 1] = '\0';
}

// Function to generate Hamming Code
void generateHamming(char data[], char hamming[]) {
    int m = strlen(data), r = 0, i, j, k;
    while ((1 << r) < (m + r + 1)) r++;
    int n = m + r;
    for (i = 0, j = 0, k = 1; k <= n; k++) {
        if ((k & (k - 1)) == 0) {
            hamming[k - 1] = '0';
        } else {
            hamming[k - 1] = data[j++];
        }
    }
    for (i = 0; i < r; i++) {
        int parity = 0;
        for (j = 1; j <= n; j++) {
            if (j & (1 << i)) {
                parity ^= (hamming[j - 1] - '0');
            }
        }
        hamming[(1 << i) - 1] = parity + '0';
    }
    hamming[n] = '\0';
}

// Function to detect and correct error in Hamming Code
int detectAndCorrectHamming(char hamming[]) {
    int n = strlen(hamming), r = 0, i, errorPos = 0;
    while ((1 << r) < n) r++;
    for (i = 0; i < r; i++) {
        int parity = 0;
        for (int j = 1; j <= n; j++) {
            if (j & (1 << i)) {
                parity ^= (hamming[j - 1] - '0');
            }
        }
        if (parity) errorPos += (1 << i);
    }
    if (errorPos) {
        hamming[errorPos - 1] = (hamming[errorPos - 1] == '0') ? '1' : '0';
        return errorPos;
    }
    return 0;
}

int main() {
    int choice;
    do {
        printf("\nMenu:\n1. LRC & VRC\n2. CRC\n3. Hamming Code\n4. Exit\nChoose an option: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int rows, cols;
            printf("Enter number of rows and columns: ");
            scanf("%d %d", &rows, &cols);
            char data[rows][cols+1], lrc[cols+1], vrc;
            for (int i = 0; i < rows; i++) {
                printf("Enter row %d: ", i + 1);
                scanf("%s", data[i]);
            }
            calculateLRC(data, rows, cols, lrc);
            printf("LRC: %s\n", lrc);
            calculateVRC(data[0], cols, &vrc);
            printf("VRC for first row: %c\n", vrc);
        } else if (choice == 2) {
            char data[32], poly[16], crc[16];
            printf("Enter data: ");
            scanf("%s", data);
            printf("Enter polynomial: ");
            scanf("%s", poly);
            computeCRC(data, poly, crc);
            printf("CRC: %s\n", crc);
        } else if (choice == 3) {
            char data[16], hamming[32];
            printf("Enter data bits: ");
            scanf("%s", data);
            generateHamming(data, hamming);
            printf("Hamming Code: %s\n", hamming);
            printf("Enter received Hamming Code: ");
            scanf("%s", hamming);
            int errorPos = detectAndCorrectHamming(hamming);
            if (errorPos) {
                printf("Error found at position %d, corrected Hamming Code: %s\n", errorPos, hamming);
            } else {
                printf("No errors detected.\n");
            }
        }
    } while (choice != 4);
    return 0;
}
