//**********************************
//Assignment 2
//Author: Andres Martinez
//Date: 2/18/2025
//Class: COP3502, Dr. Ali 
//Purpose:The purpose of this program is to work with linked lists
//***********************************

// Hint: To read the book title, author's first name, and author's last name,  
// you can use:  
// fscanf(file, " \"%[^\"]\" %s %s", bookTitle, lastName, firstName);  

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_NAME 50

//building struct for linked list
typedef struct node_s {
    //need to store data for bookTitle, borrowerFirstN, borrowerLastN
    char bookTitle[MAX_TITLE];
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    //pointer to next nodes
    struct node_s* next;
} node_t;

// Function prototypes
node_t *borrowBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output);
node_t *returnBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output);
void checkBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output);
void displayBorrowedBooks(node_t *head, FILE *output);
void freeList(node_t *head, FILE *output);


int main(void){
    //Pointer for the linked list
    node_t *head = NULL;

    //Open the input and output files
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

    int libOperation;
    //Scan each operation
    while (fscanf(fp, "%d", &libOperation) == 1) {
        //Exit at operation 5
        if (libOperation == 5) {
            fprintf(output, "Thank you for using the Library System!\n");
            break;
        }

        
        char title[MAX_TITLE];
        char lastName[MAX_NAME];
        char firstName[MAX_NAME];

        //Use a switch to handle the 5 operations
        switch (libOperation) {
            case 1:// Borrow a Book
                
                fscanf(fp, " \"%[^\"]\" %s %s", title, lastName, firstName);
                head = borrowBook(head, title, lastName, firstName, output);
                break;

            case 2:// Return a Book
                fscanf(fp, " \"%[^\"]\" %s %s", title, lastName, firstName);
                head = returnBook(head, title, lastName, firstName, output);
                break;

            case 3:// Check to see if Book is Borrowed
                fscanf(fp, " \"%[^\"]\" %s %s", title, lastName, firstName);
                checkBook(head, title, lastName, firstName, output);
                break;

            case 4://Display Borrowed Books
                
                displayBorrowedBooks(head, output);
                break;

            default:
                // If it's an unknown code (not 1–5), decide how to handle it
                // For now, do nothing or skip the line
                break;
        }
    }

    // 5) Free the linked list and close files
    freeList(head, output);
    fclose(fp);
    fclose(output);

    return 0;
}

//borrow book function 
node_t *borrowBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output){
    //Whole purpose of this is to add to end of a linked list

    //allocate memory for node in the linked list
    node_t*  newNode = malloc(sizeof(node_t));

    //error handle for DMA
    if(!newNode){
        printf("Memory allocation failed. \n");
        return NULL;
    }

    //Place data into new node
    strcpy(newNode->bookTitle, title);//Im going to use fscanf in my main to read each line
    strcpy(newNode->lastName,  lastName);
    strcpy(newNode->firstName, firstName);

    newNode->next = NULL; 

    //print data taken
    fprintf(output, "Borrowed \"%s\" by %s %s", newNode->bookTitle,newNode->lastName,newNode->firstName);


    //If the list is emty set head as the newNode
    if(head == NULL){
        return newNode;//New node becomes the head if the list is empty
    } else{
        node_t *value = head;
        //value inserted 
        while(value->next != NULL){
           value = value->next;
    }
    value->next = newNode;
 }
    return head;
}


node_t *returnBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output) {
    //Traverse the list with value
    node_t *value = head;
    node_t *previous = NULL;

    //Traverse the list
    while (value != NULL) {
        //Check found the node to remove
        if (strcmp(value->bookTitle, title) == 0 && strcmp(value->lastName, lastName) == 0 && strcmp(value->firstName, firstName) == 0) {
            
            // We found the node to remove
            if (previous == NULL) {
                //The node to remove is the head
                head = value->next;
            } else {
                //The node to remove is in anywhere in the list
                previous->next = value->next;
            }
            
            //Free the memory allocated for the removed node
            free(value);

            //Print
            fprintf(output, "Returned \"%s\" by %s %s\n", title, lastName, firstName);
            return head; 
        }

        //Move to the next node
        previous = value;
        value = value->next;
    }
    return head;
}

void checkBook(node_t *head, char *title, char *lastName, char *firstName, FILE *output) {

    node_t *value = head;

    //Search the list
    while (value != NULL) {
        //Compare all three: bookTitle, lastName, firstName
        if (strcmp(value->bookTitle, title) == 0 &&
            strcmp(value->lastName, lastName) == 0 &&
            strcmp(value->firstName, firstName) == 0) {
            
            //Print when a match is found
            fprintf(output, "\"%s\" is borrowed by %s, %s\n", value->bookTitle, value->lastName, value->firstName);
            return;//exit
        }
        value = value->next;//move the node
    }

    // If we exit the loop, the book wasn't found in the list
    fprintf(output, "\"%s\" is not currently borrowed by %s, %s\n", title, lastName, firstName);
}

void displayBorrowedBooks(node_t *head, FILE *output){

    

    fprintf(output,"Borrowed Books List: \n");

    //variable to for the loop
    int i = 1;
    //printing list
    node_t *value = head;

    while(value != NULL){
        fprintf(output, "%d. \"%s\ - %s, %s", i + 1, value->bookTitle, value->lastName, value->firstName);

        value = value->next;
        i++;
    }
}


void freeList(node_t *head, FILE *output) {
    node_t *current = head;
    while (current != NULL) {
        node_t *temp = current;
        current = current->next;
        free(temp);
    }
}