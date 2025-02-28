//**********************************
//Assignment 2
//Author: Andres Martinez
//Date: 2/18/2025
//Class: COP3502, Dr. Ali 
//Purpose: The purpose of this program is to work with linked lists
//**********************************

// Hint: To read the book title, author's first name, and author's last name,  
// you can use:  
// fscanf(file, " \"%[^\"]\" %s %s", bookTitle, lastName, firstName);  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_NAME 50

// Function prototypes
node_t *borrowBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output);
node_t *returnBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output);
void checkBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output);
void displayBorrowedBooks(node_t *head, FILE *output);
void freeList(node_t *head);

// Define the linked list node
typedef struct node_s {
    char bookTitle[MAX_TITLE];
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    struct node_s *next;
} node_t;

int main(void) {
    node_t *head = NULL;

    // Open the input file for reading and output file for writing.
    FILE *fp = fopen("library_records.txt", "r");
    if (!fp) {
        printf("Error: Could not open input file.\n");
        return 1;
    }
    FILE *output = fopen("output.txt", "w");
    if (!output) {
        printf("Error: Could not open output file.\n");
        fclose(fp);
        return 1;
    }

    int operationCode;
    // Read each operation code from the file.
    while (fscanf(fp, "%d", &operationCode) == 1) {
        // If operationCode == 5, quit.
        if (operationCode == 5) {
            fprintf(output, "Thank you for using the Library System!\n");
            break;
        }

        // For operations 1, 2, and 3 we need to read book info.
        char title[MAX_TITLE];
        char lastName[MAX_NAME];
        char firstName[MAX_NAME];

        switch (operationCode) {
            case 1: // Borrow a Book
                fscanf(fp, " \"%[^\"]\" %s %s", title, lastName, firstName);
                head = borrowBook(head, title, lastName, firstName, output);
                break;
            case 2: // Return a Book
                fscanf(fp, " \"%[^\"]\" %s %s", title, lastName, firstName);
                head = returnBook(head, title, lastName, firstName, output);
                break;
            case 3: // Check if a Book is Borrowed
                fscanf(fp, " \"%[^\"]\" %s %s", title, lastName, firstName);
                checkBook(head, title, lastName, firstName, output);
                break;
            case 4: // Display All Borrowed Books
                displayBorrowedBooks(head, output);
                break;
            default:
                // Unknown operation; you can skip or handle as needed.
                break;
        }
    }

    // Clean up: free the linked list and close files.
    freeList(head);
    fclose(fp);
    fclose(output);
    return 0;
}

//------------------------
// Function Definitions
//------------------------

// Adds a new node to the end of the list (Borrow a Book)
node_t *borrowBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output) {
    node_t *newNode = malloc(sizeof(node_t));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return head;
    }
    // Copy the book info into the new node.
    strcpy(newNode->bookTitle, title);
    strcpy(newNode->lastName, lastName);
    strcpy(newNode->firstName, firstName);
    newNode->next = NULL;

    // Print the borrowed message.
    fprintf(output, "Borrowed \"%s\" by %s %s\n", newNode->bookTitle, newNode->lastName, newNode->firstName);

    // Insert newNode at the end of the list.
    if (head == NULL) {
        return newNode;
    } else {
        node_t *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    return head;
}

// Removes a node from the list (Return a Book)
node_t *returnBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output) {
    node_t *current = head, *previous = NULL;
    while (current != NULL) {
        if (strcmp(current->bookTitle, title) == 0 &&
            strcmp(current->lastName, lastName) == 0 &&
            strcmp(current->firstName, firstName) == 0) {
            // Found the matching node.
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            fprintf(output, "Returned \"%s\" by %s %s\n", title, lastName, firstName);
            return head;
        }
        previous = current;
        current = current->next;
    }
    // Not found.
    fprintf(output, "\"%s\" is not currently borrowed by %s %s\n", title, lastName, firstName);
    return head;
}

// Checks if a book is currently borrowed.
void checkBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output) {
    node_t *current = head;
    while (current != NULL) {
        if (strcmp(current->bookTitle, title) == 0 &&
            strcmp(current->lastName, lastName) == 0 &&
            strcmp(current->firstName, firstName) == 0) {
            fprintf(output, "\"%s\" is borrowed by %s %s\n", current->bookTitle, current->lastName, current->firstName);
            return;
        }
        current = current->next;
    }
    fprintf(output, "\"%s\" is not currently borrowed by %s %s\n", title, lastName, firstName);
}

// Displays the list of borrowed books.
void displayBorrowedBooks(node_t *head, FILE *output) {
    fprintf(output, "Borrowed Books List:\n");
    int i = 1;
    node_t *current = head;
    while (current != NULL) {
        fprintf(output, "%d. \"%s\" - %s, %s\n", i, current->bookTitle, current->lastName, current->firstName);
        current = current->next;
        i++;
    }
}

// Frees all nodes in the linked list.
void freeList(node_t *head) {
    node_t *current = head;
    while (current != NULL) {
        node_t *temp = current;
        current = current->next;
        free(temp);
    }
}
