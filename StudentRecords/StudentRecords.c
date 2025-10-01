#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define FILENAME "Sample-CMS.txt"


void freeMemory();


typedef struct Student {
    int id;
    char name[50];
    char programme[50];
    float mark;
    struct Student* next;
} Student;


// Function Prototypes
void freeMemory();
void loadRecords();
void saveRecords();
void showAllRecords();
void insertRecord();
void queryRecord();
void updateRecord();
void deleteRecord();
int isAlpha(const char* str);
int isDigit(const char* str);


Student* head = NULL;


void loadRecords() {
    freeMemory();
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("File not found. Creating a new one.\n");
        saveRecords(); // Create an empty file if none exists
        return;
    }

    char line[200];
    int inDataSection = 0; // Flag to indicate when we reach the actual data section

    while (fgets(line, sizeof(line), file)) {
        // Skip metadata and header lines
        if (!inDataSection) {
            if (strstr(line, "ID")) { // Look for the column header to detect the data section
                inDataSection = 1;
            }
            continue; // Skip until we find the header line
        }

        // Parse the student record
        Student* newStudent = malloc(sizeof(Student));
        if (sscanf(line, "%d %49[^\t] %49[^\t] %f", &newStudent->id, newStudent->name, newStudent->programme, &newStudent->mark) != 4) {
            free(newStudent);
            continue; // Skip malformed lines
        }

        // Trim any trailing whitespace from the name and programme
        newStudent->name[strcspn(newStudent->name, "\n")] = 0;
        newStudent->programme[strcspn(newStudent->programme, "\n")] = 0;

        // Insert the new student at the beginning of the linked list
        newStudent->next = head;
        head = newStudent;
    }

    fclose(file);
    printf("CMS: The database file “%s” is successfully opened.\n", FILENAME);
}



void saveRecords() {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error opening file for saving.\n");
        return;
    }

    // Save metadata and header
    fprintf(file, "Database Name: StudentRecords\n");
    fprintf(file, "Authors: P12 Group 5\n\n");
    fprintf(file, "Table Name: StudentRecords\n");
    fprintf(file, "ID\tName\tProgramme\tMark\n");

    // Save raw data (unformatted)
    Student* current = head;
    while (current) {
        fprintf(file, "%d\t%s\t%s\t%.2f\n", current->id, current->name, current->programme, current->mark);
        current = current->next;
    }

    fclose(file);
    printf("Records saved successfully.\n");
}




void showAllRecords() {
    if (!head) {
        printf("No records found.\n");
        return;
    }

    printf("\nHere are all records:\n\n");
    printf("%-12s%-30s%-30s%-12s\n", "ID", "Name", "Programme", "Mark");

    Student* current = head;
    while (current) {
        printf("%-12d%-30s%-30s%-12.2f\n", current->id, current->name, current->programme, current->mark);
        current = current->next;
    }
}


int isAlpha(const char* str) {
    while (*str) {
        if (*str == ' ') {
            str++;
            continue;
        }
        if (!isalpha(*str)) return 0;
        str++;
    }
    return 1;
}


int isDigit(const char* str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}


void insertRecord() {
    char idStr[10];

    printf("Enter ID: ");
    scanf("%s", idStr);

    // Validate ID input
    if (!isDigit(idStr)) {
        printf("Invalid ID. Please enter a numeric value.\n");
        return;
    }

    int newId = atoi(idStr);

    // Check if the ID already exists in the linked list
    Student* current = head;
    while (current) {
        if (current->id == newId) {
            printf("Error: Record with ID %d already exists.\n", newId);
            return; // Don't insert the record if ID already exists
        }
        current = current->next;
    }

    // Allocate memory for the new student
    Student* newStudent = malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Set the student's information
    newStudent->id = newId;

    // Input Name
    printf("Enter Name: ");
    getchar(); // Clear newline character left by scanf
    fgets(newStudent->name, sizeof(newStudent->name), stdin);
    newStudent->name[strcspn(newStudent->name, "\n")] = 0; // Remove newline

    // Validate Name input (ensure it is not empty)
    while (strlen(newStudent->name) == 0) {
        printf("Name cannot be empty. Please enter a valid name: ");
        fgets(newStudent->name, sizeof(newStudent->name), stdin);
        newStudent->name[strcspn(newStudent->name, "\n")] = 0; // Remove newline
    }

    // Input Programme
    printf("Enter Programme: ");
    fgets(newStudent->programme, sizeof(newStudent->programme), stdin);
    newStudent->programme[strcspn(newStudent->programme, "\n")] = 0; // Remove newline

    // Validate Programme input (ensure it is not empty)
    while (strlen(newStudent->programme) == 0) {
        printf("Programme cannot be empty. Please enter a valid programme: ");
        fgets(newStudent->programme, sizeof(newStudent->programme), stdin);
        newStudent->programme[strcspn(newStudent->programme, "\n")] = 0; // Remove newline
    }

    // Input Mark
    printf("Enter Mark: ");
    scanf("%f", &newStudent->mark);

    // Insert the new student at the beginning of the linked list
    newStudent->next = head;
    head = newStudent;

    printf("Record successfully added.\n");
}


void queryRecord() {
    char idStr[10];
    printf("Enter ID to query: ");
    scanf("%s", idStr);


    if (!isDigit(idStr)) {
        printf("Invalid ID. Please enter a numeric value.\n");
        return;
    }


    int id = atoi(idStr);


    // Search the linked list for the record
    Student* current = head;
    while (current) {
        if (current->id == id) {
            // Record found, display it in the same format as `showAllRecords`
            printf("\nRecord found:\n");
            printf("%-12s%-30s%-30s%-12s\n", "ID", "Name", "Programme", "Mark");
            printf("%-12d%-30s%-30s%-12.2f\n", current->id, current->name, current->programme, current->mark);
            return;
        }
        current = current->next;
    }


    // If no matching record is found
    printf("Record with ID %d not found.\n", id);
}

void updateRecord() {
    char idStr[10];
    printf("Enter ID to update: ");
    scanf("%s", idStr);

    if (!isDigit(idStr)) {
        printf("Invalid ID. Please enter a numeric value.\n");
        return;
    }

    int id = atoi(idStr);

    // Search the linked list for the record
    Student* current = head;
    while (current) {
        if (current->id == id) {
            // Record found, allow updating
            printf("Record found:\n");
            printf("%-12s%-30s%-30s%-12s\n", "ID", "Name", "Programme", "Mark");
            printf("%-12d%-30s%-30s%-12.2f\n", current->id, current->name, current->programme, current->mark);

            printf("\nChoose which field(s) to update:\n");
            printf("1. Name\n");
            printf("2. Programme\n");
            printf("3. Mark\n");
            printf("4. Update all fields\n");
            printf("5. Cancel update\n");

            int choice;
            printf("Enter your choice (1-5): ");
            scanf("%d", &choice);
            getchar();  // Clear newline left by scanf

            // Perform updates based on user choice
            if (choice == 1 || choice == 4) {
                printf("Enter new Name: ");
                fgets(current->name, sizeof(current->name), stdin);
                current->name[strcspn(current->name, "\n")] = '\0';  // Remove newline
            }

            if (choice == 2 || choice == 4) {
                printf("Enter new Programme: ");
                fgets(current->programme, sizeof(current->programme), stdin);
                current->programme[strcspn(current->programme, "\n")] = '\0';  // Remove newline
            }

            if (choice == 3 || choice == 4) {
                printf("Enter new Mark: ");
                scanf("%f", &current->mark);
            }

            if (choice == 5) {
                printf("Update canceled.\n");
                return;
            }

            printf("Record updated successfully.\n");
            return;
        }
        current = current->next;
    }

    // If no matching record is found
    printf("Record with ID %d not found.\n", id);
}
void deleteRecord() {
    char idStr[10];
    printf("Enter ID to delete: ");
    scanf("%s", idStr);

    if (!isDigit(idStr)) {
        printf("Invalid ID. Please enter a numeric value.\n");
        return;
    }
    int id = atoi(idStr);

    // Check if the record exists in the linked list
    Student* current = head;
    int recordFound = 0;

    while (current) {
        if (current->id == id) {
            recordFound = 1;
            break;
        }
        current = current->next;
    }

    if (!recordFound) {
        printf("Record not found in memory.\n");
        return;
    }

    // Ask for confirmation
    char confirm;
    while (1) {
        printf("Are you sure you want to delete the record with ID %d? (Y/N): ", id);
        scanf(" %c", &confirm);
        if (confirm == 'Y' || confirm == 'y') {
            break;
        }
        else if (confirm == 'N' || confirm == 'n') {
            printf("Deletion cancelled.\n");
            return;
        }
        else {
            printf("Invalid input. Please enter 'Y' or 'N'.\n");
        }
    }

    // Remove from the linked list
    current = head;
    Student* previous = NULL;

    while (current) {
        if (current->id == id) {
            if (previous) {
                previous->next = current->next; // Bypass the current node
            }
            else {
                head = current->next; // Update head if the first node is deleted
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }

    // Remove from the file
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("File not found. Please make sure the file exists.\n");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        printf("Error opening temporary file.\n");
        fclose(file);
        return;
    }

    char line[200];
    int inDataSection = 0;

    while (fgets(line, sizeof(line), file)) {
        if (!inDataSection) {
            if (strstr(line, "ID")) {
                inDataSection = 1;
            }
            fputs(line, tempFile); // Copy metadata and headers to the temp file
            continue;
        }

        int currentId;
        sscanf(line, "%d", &currentId);
        if (currentId == id) {
            continue; // Skip the line with the matching ID
        }
        fputs(line, tempFile); // Copy all other lines to the temp file
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME); // Remove the original file
    rename("temp.txt", FILENAME); // Rename the temp file to the original file name

    printf("Record with ID %d deleted successfully.\n", id);
}




void freeMemory() {
    Student* current = head;
    Student* next;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL; // Reset the head pointer
}


int main() {
    char command[10];

    printf("SIT's policy on copying does not allow the students to copy source code as well as assessment solutions from another \nperson or other places.It is the students responsibility to guarantee that their assessment solutions are their own work\nMeanwhile, the students must also ensure that their work is not accessible by others. Where such plagiarism is detected,both of the assessments involved will receive ZERO mark. We hereby declare that:\n\nWe fully understand and agree to the abovementioned plagiarism policy.\nWe did not copy any code from others or from other places.\nWe did not share our codes with others or upload to any other places for public access and will not do that in the\nfuture.\nWe agree that our project will receive Zero mark if there is any plagiarism detected.\nWe agree that we will not disclose any information or material of the group project to others or upload to any other\nplaces for public access.\n\n");

    while (1) {
        printf("Enter command (OPEN, SHOW, INSERT, QUERY, UPDATE, DELETE, SAVE, EXIT): ");
        scanf("%s", command);
        for (int i = 0; i < strlen(command);i++) {
            command[i] = tolower(command[i]);
        }
        if (strcmp(command, "open") == 0) {
            loadRecords();
            printf("The file: %s is loaded successfully.\n", FILENAME);
        }
        else if (strcmp(command, "show") == 0) {
            showAllRecords();
        }
        else if (strcmp(command, "insert") == 0) {
            insertRecord();
        }
        else if (strcmp(command, "query") == 0) {
            queryRecord();
        }
        else if (strcmp(command, "update") == 0) {
            updateRecord();
        }
        else if (strcmp(command, "delete") == 0) {
            deleteRecord();
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
        else if (strcmp(command, "save") == 0) {
            saveRecords();
        }
        else {
            printf("Invalid command.\n");
        }
    }
    freeMemory(); // Free dynamically allocated memory
    return 0;
}

