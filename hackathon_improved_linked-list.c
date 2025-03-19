/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @LIBRARIES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h> // Added for Windows-specific functions

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @MACROS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define RED     "\x1b[31m" //RED for error / mismatched input
#define GREEN   "\x1b[32m" //GREEN for information
#define YELLOW  "\x1b[33m" //YELLOW for choices
#define CYAN    "\x1b[36m" //CYAN for default text
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define RESET   "\x1b[0m"

// Enum for status types
enum bookStatus {AVAILABLE = 1, CHECKED_OUT = 0};

// Declare book data structure as 'book'
typedef struct Book {
    char title[50];
    char author[50];
    char isbn[20];
    enum bookStatus status;
    struct Book* next;  // Pointer to the next book in the linked list
} book;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @FUNCTION PROTOTYPES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int search ();
void store (book** head, int* bookCount);
void displayAll (book* head, int bookCount);
void displaySingle (book* node);
void returnBook (book* node);
void checkOutBook (book* node);
book* searchByTitle (book* head, int bookCount);
book* searchByAuthor (book* head, int bookCount);
book* searchByISBN (book* head, int bookCount);
char* getAvailability(enum bookStatus status);
void generateISBN(book* node);
void clearScreen();
void displayHeader();
void displayMainMenu();
void waitForKeypress();
book* createNewBook();
void freeAllBooks(book** head);
// void typewriter(const char *text, int delay_ms);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @MAIN FUNCTION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main ()
{
    // Declare head of the linked list
    book* head = NULL;

    int bookCount = 0;
    int usrChoice;

    // Main menu loop
    while (true)
    {
        clearScreen();
        displayHeader();
        displayMainMenu();
        
        usrChoice = getchar();
        while (getchar() != '\n'); // reject non-numeric inputs

        switch(usrChoice)
        {
            case '1':
               clearScreen();
               displayHeader();
               store(&head, &bookCount);
               waitForKeypress();
               break;
            case '2':
                clearScreen();
                displayHeader();
                if (bookCount > 0) {
                    displayAll(head, bookCount);
                } else {
                    printf(RED"\nNo books to display.\n"RESET);
                }
                waitForKeypress();
                break;
            case '3':
                clearScreen();
                displayHeader();
                if (bookCount <= 0) {
                    printf(RED"\nNo books to search.\n"RESET);
                    waitForKeypress();
                    break;
                }
                
                printf(CYAN"<=======================================>\n<< Enter mode to search >>\n<=======================================>\n"RESET);
                printf(YELLOW"~~ 1 - By Title\t2 - By Author\n~~ 3 - By ISBN\n<=======================================>\n|=> "RESET);

                int searchType;
                book* foundBook = NULL;  // Changed from index to pointer

                searchType = getchar();
                while (getchar() != '\n');

                switch(searchType) {
                    case '1':
                        clearScreen();
                        displayHeader();
                        foundBook = searchByTitle(head, bookCount);
                        if (foundBook != NULL) displaySingle(foundBook);
                        break;
                    case '2':
                        clearScreen();
                        displayHeader();
                        foundBook = searchByAuthor(head, bookCount);
                        if (foundBook != NULL) displaySingle(foundBook);
                        break;
                    case '3': 
                        clearScreen();
                        displayHeader();
                        foundBook = searchByISBN(head, bookCount);
                        if (foundBook != NULL) displaySingle(foundBook);
                        break;
                    default:
                        printf(RED"Invalid choice. Please try again.\n"RESET);
                        waitForKeypress();
                        break;
                }

                if (foundBook != NULL)
                {
                    int option;
                    printf(CYAN"<============= Options: ================>\n"RESET);
                    printf(YELLOW"~~ 1 - Return Book\t2 - Checkout\n~~ 3 - Back to Menu\n|=> "RESET);
                    scanf("%d", &option);
                    while (getchar() != '\n'); // Clear input buffer
                    
                    switch(option)
                    {
                        case 1:
                            returnBook(foundBook);
                            break;
                        case 2:
                            checkOutBook(foundBook);
                            break;
                        case 3:
                            break;
                        default:
                            printf(RED"Invalid option. Going back to main menu.\n"RESET);
                            break;
                    }
                    waitForKeypress();
                }
                break;
            case '4':
                clearScreen();
                printf(GREEN"\nThank you for using the Library Management System!\n\n"RESET);
                freeAllBooks(&head);  // Free allocated memory before exit
                exit(0);
                break;
            default:
                printf(RED"Invalid choice. Please try again.\n"RESET);
                waitForKeypress();
                break;
        }
    }
}

// Function to free all allocated memory for books
void freeAllBooks(book** head) {
    book* current = *head;
    book* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    *head = NULL;
}

// Function to clear the console screen (Windows-specific)
void clearScreen() {
    system("cls");
}

// Display the header/banner of the program
void displayHeader() {
    printf(BLUE"===================================================\n"RESET);
    printf(BLUE"||        "YELLOW"LIBRARY MANAGEMENT SYSTEM       "BLUE"       ||\n"RESET);
    printf(BLUE"===================================================\n\n"RESET);
}

// Display the main menu
void displayMainMenu() {
    printf(YELLOW"<=======================================>\n"
           "||              MAIN MENU               ||\n"
           "<=======================================>\n"
           "|| 1 - Add Books                       ||\n"
           "|| 2 - Display All Books               ||\n"
           "|| 3 - Search Books                    ||\n"
           "|| 4 - Exit                           ||\n"
           "<=======================================>\n"
           "|>> "RESET);
}

// Wait for user to press a key before continuing
void waitForKeypress() {
    printf(YELLOW"\nPress Enter to continue..."RESET);
    getchar();
}

// Create a new book node
book* createNewBook() {
    book* newBook = (book*)malloc(sizeof(book));
    if (newBook == NULL) {
        printf(RED"Memory allocation failed\n"RESET);
        exit(1);
    }
    
    newBook->next = NULL;
    return newBook;
}

// Generate a unique isbn per book added
void generateISBN(book* node)
{
    srand(time(NULL) + (long)node); // Use pointer address for better randomness
    
    // Initialize isbn string
    memset(node->isbn, 0, sizeof(node->isbn));
    
    // Generate ISBN format: XXXX-XXXX-XXXX-XXXX (all digits)
    int j = 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 9 || i == 14) {
            node->isbn[i] = '-';
        } else {
            node->isbn[i] = '0' + (rand() % 10);  // Generate digits 0-9
        }
    }
    node->isbn[19] = '\0'; // Ensure null termination
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @SEARCH FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Search book by book title
book* searchByTitle(book* head, int bookCount)
{
    book* current = head;
    char titleToSearch[50];  // Match size with book structure

    printf(CYAN"\n<=======================================>\n"
           "||             SEARCH BY TITLE            ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter book title: "RESET);
    scanf(" %49[^\n]", titleToSearch);  // Prevent buffer overflow
    while (getchar() != '\n');  // Clear input buffer

    printf(YELLOW"\nSearching..."RESET);
    Sleep(500); // Add a small delay for better UX
    
    int i = 0;
    while (current != NULL) {
        char bookTitle[50];
        char searchTitle[50];
        
        // Create temporary copies for the strlwr function
        strcpy(bookTitle, current->title);
        strcpy(searchTitle, titleToSearch);
        
        if (strcmp(strlwr(bookTitle), strlwr(searchTitle)) == 0) {
            printf(GREEN"\nBook is found!\n"RESET);
            return current;
        }
        current = current->next;
        i++;
    }

    printf(RED"\nBook is not found.\n"RESET);
    return NULL;
}

// Search book by author
book* searchByAuthor(book* head, int bookCount)
{
    book* current = head;
    char authorToSearch[50];  // Match size with book structure

    printf(CYAN"\n<=======================================>\n"
           "||             SEARCH BY AUTHOR           ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter author name: "RESET);
    scanf(" %49[^\n]", authorToSearch);  // Prevent buffer overflow
    while (getchar() != '\n');  // Clear input buffer

    printf(YELLOW"\nSearching..."RESET);
    Sleep(500); // Add a small delay for better UX

    int i = 0;
    while (current != NULL) {
        char bookAuthor[50];
        char searchAuthor[50];
        
        // Create temporary copies for the strlwr function
        strcpy(bookAuthor, current->author);
        strcpy(searchAuthor, authorToSearch);
        
        if (strcmp(strlwr(bookAuthor), strlwr(searchAuthor)) == 0) {
            printf(GREEN"\nBook is found!\n"RESET);
            return current;
        }
        current = current->next;
        i++;
    }

    printf(RED"\nBook is not found.\n"RESET);
    return NULL;
}

// Search book by book isbn
book* searchByISBN(book* head, int bookCount)
{
    book* current = head;
    char isbnToSearch[20];  // Match size with book structure

    printf(CYAN"\n<=======================================>\n"
           "||              SEARCH BY ISBN             ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter ISBN: "RESET);
    scanf(" %19[^\n]", isbnToSearch);  // Prevent buffer overflow
    while (getchar() != '\n');  // Clear input buffer

    printf(YELLOW"\nSearching..."RESET);
    Sleep(500); // Add a small delay for better UX

    int i = 0;
    while (current != NULL) {
        char bookISBN[20];
        char searchISBN[20];
        
        // Create temporary copies for the strlwr function
        strcpy(bookISBN, current->isbn);
        strcpy(searchISBN, isbnToSearch);
        
        if (strcmp(strlwr(bookISBN), strlwr(searchISBN)) == 0) {
            printf(GREEN"\nBook is found!\n"RESET);
            return current;
        }
        current = current->next;
        i++;
    }

    printf(RED"\nBook is not found.\n"RESET);
    return NULL;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @STORE FUNCTION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void store(book** head, int* bookCount) {
    int count = 0;
    printf(CYAN"\n<=======================================>\n"
           "||               ADD BOOKS                ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter the number of books to add: "RESET);
    scanf("%d", &count);
    while (getchar() != '\n');  // Clear input buffer
    
    if (count <= 0) {
        printf(RED"\nInvalid number of books.\n"RESET);
        return;
    }
    
    for (int i = 0; i < count; i++) {
        clearScreen();
        displayHeader();
        printf(CYAN"\n<=======================================>\n"
               "||               ADD BOOKS                ||\n"
               "<=======================================>\n"RESET);
        printf(YELLOW"\nBook #%d of %d:\n"RESET, i + 1, count);
        
        book* newBook = createNewBook();
        
        printf(YELLOW"Book Title: "RESET);
        scanf(" %49[^\n]", newBook->title);  // Prevent buffer overflow
        while (getchar() != '\n');  // Clear input buffer
        
        printf(YELLOW"Author: "RESET);
        scanf(" %49[^\n]", newBook->author);  // Prevent buffer overflow
        while (getchar() != '\n');  // Clear input buffer

        generateISBN(newBook);
        newBook->status = AVAILABLE;
        
        // Add the new book to the beginning of the list for efficiency
        newBook->next = *head;
        *head = newBook;
    }
    
    *bookCount += count;
    printf(GREEN"\nSuccessfully added %d books. Total books: %d\n"RESET, count, *bookCount);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @DISPLAY FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void displayAll(book* head, int bookCount) {
    book* current = head;
    int i = 1;
    
    printf(CYAN"\n<=======================================>\n"
           "||              ALL BOOKS                 ||\n"
           "<=======================================>\n\n"RESET);
    
    while (current != NULL) {
        printf(YELLOW"<=======================================>\n"RESET);
        printf(CYAN"~~> Book #%d\n"RESET, i);
        printf(CYAN"~~> Title: "RESET);
        printf(GREEN"%s\n"RESET, current->title);
        printf(CYAN"~~> Author:  "RESET);
        printf(GREEN"%s\n"RESET, current->author);
        printf(CYAN"~~> ISBN: "RESET);
        printf(GREEN"%s\n"RESET, current->isbn);
        printf(CYAN"~~> AVAILABILITY: "RESET);
        printf("%s\n", current->status == AVAILABLE ? GREEN"Available"RESET : RED"Checked Out"RESET);
        
        current = current->next;
        i++;
    }
}

void displaySingle(book* node) {
    printf(YELLOW"\n<=======================================>\n"
           "||              BOOK DETAILS              ||\n"
           "<=======================================>\n\n"RESET);
    
    printf(CYAN"~~> Title: "RESET);
    printf(GREEN"%s\n"RESET, node->title);
    printf(CYAN"~~> Author: "RESET);
    printf(GREEN"%s\n"RESET, node->author);
    printf(CYAN"~~> ISBN: "RESET);
    printf(GREEN"%s\n"RESET, node->isbn);
    printf(CYAN"~~> AVAILABILITY: "RESET);
    printf("%s\n", node->status == AVAILABLE ? GREEN"Available"RESET : RED"Checked Out"RESET);
    
    printf(YELLOW"<=======================================>\n"RESET);
}

char* getAvailability(enum bookStatus status)
{
    switch(status)
    {
        case AVAILABLE:
            return "Available";
        case CHECKED_OUT:
            return "Checked Out";
        default:
            return "Unknown";  // Add default case for better error handling
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @CHECKOUT/RETURN FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void returnBook(book* node) 
{
    if (node->status == CHECKED_OUT) {
        node->status = AVAILABLE;
        printf(GREEN"\nBook has been returned successfully.\n"RESET);
    } else {
        printf(YELLOW"\nBook is already available.\n"RESET);
    }
}

void checkOutBook(book* node) 
{
    if (node->status == AVAILABLE) {
        node->status = CHECKED_OUT;
        printf(GREEN"\nBook has been checked out successfully.\n"RESET);
    } else {
        printf(RED"\nBook is already checked out.\n"RESET);
    }
}
