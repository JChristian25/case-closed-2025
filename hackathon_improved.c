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
} book;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @FUNCTION PROTOTYPES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int search ();
void store (book* books, int* bookCount);
void displayAll (book* books, int bookCount);
void displaySingle (book* books, int index);
void returnBook (book* books, int index);
void checkOutBook (book* books, int index);
int searchByTitle (book* books, int bookCount);
int searchByAuthor (book* books, int bookCount);
int searchByISBN (book* books, int bookCount);
char* getAvailability(enum bookStatus status);
void generateISBN(book* books, int index);
void clearScreen();
void displayHeader();
void displayMainMenu();
void waitForKeypress();
// void typewriter(const char *text, int delay_ms);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @MAIN FUNCTION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main ()
{
    // Declare array that will hold books
    struct Book books[100];

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
               store(books, &bookCount);
               waitForKeypress();
               break;
            case '2':
                clearScreen();
                displayHeader();
                if (bookCount > 0) {
                    displayAll(books, bookCount);
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
                int index = -1;  // Initialize to invalid index

                searchType = getchar();
                while (getchar() != '\n');

                switch(searchType) {
                    case '1':
                        clearScreen();
                        displayHeader();
                        index = searchByTitle(books, bookCount);
                        if (index > -1) displaySingle(books, index);
                        break;
                    case '2':
                        clearScreen();
                        displayHeader();
                        index = searchByAuthor(books, bookCount);
                        if (index > -1) displaySingle(books, index);
                        break;
                    case '3': 
                        clearScreen();
                        displayHeader();
                        index = searchByISBN(books, bookCount);
                        if (index > -1) displaySingle(books, index);
                        break;
                    default:
                        printf(RED"Invalid choice. Please try again.\n"RESET);
                        waitForKeypress();
                        break;
                }

                if (index > -1)
                {
                    int option;
                    printf(CYAN"<============= Options: ================>\n"RESET);
                    printf(YELLOW"~~ 1 - Return Book\t2 - Checkout\n~~ 3 - Back to Menu\n|=> "RESET);
                    scanf("%d", &option);
                    while (getchar() != '\n'); // Clear input buffer
                    
                    switch(option)
                    {
                        case 1:
                            returnBook(books, index);
                            break;
                        case 2:
                            checkOutBook(books, index);
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
                exit(0);
                break;
            default:
                printf(RED"Invalid choice. Please try again.\n"RESET);
                waitForKeypress();
                break;
        }
    }
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

// Generate a unique isbn per book added
void generateISBN(book* books, int index)
{
    srand(time(NULL) + index); // Better seeding based on time and index
    
    // Initialize isbn string
    memset(books[index].isbn, 0, sizeof(books[index].isbn));
    
    // Generate ISBN format: XXXX-XXXX-XXXX-XXXX (all digits)
    int j = 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 9 || i == 14) {
            books[index].isbn[i] = '-';
        } else {
            books[index].isbn[i] = '0' + (rand() % 10);  // Generate digits 0-9
        }
    }
    books[index].isbn[19] = '\0'; // Ensure null termination
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @SEARCH FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Search book by book title
int searchByTitle(book* books, int bookCount)
{
    int index = -1;
    char titleToSearch[50];  // Match size with book structure

    printf(CYAN"\n<=======================================>\n"
           "||             SEARCH BY TITLE            ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter book title: "RESET);
    scanf(" %49[^\n]", titleToSearch);  // Prevent buffer overflow
    while (getchar() != '\n');  // Clear input buffer

    printf(YELLOW"\nSearching..."RESET);
    Sleep(500); // Add a small delay for better UX

    for (int i = 0; i < bookCount; i++)
    {
        char bookTitle[50];
        char searchTitle[50];
        
        // Create temporary copies for the strlwr function
        strcpy(bookTitle, books[i].title);
        strcpy(searchTitle, titleToSearch);
        
        if (strcmp(strlwr(bookTitle), strlwr(searchTitle)) == 0)
        {
            printf(GREEN"\nBook is found!\n"RESET);
            index = i;
            return index;
        }
    }

    printf(RED"\nBook is not found.\n"RESET);
    return -1;
}

// Search book by author
int searchByAuthor(book* books, int bookCount)
{
    int index = -1;
    char authorToSearch[50];  // Match size with book structure

    printf(CYAN"\n<=======================================>\n"
           "||             SEARCH BY AUTHOR           ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter author name: "RESET);
    scanf(" %49[^\n]", authorToSearch);  // Prevent buffer overflow
    while (getchar() != '\n');  // Clear input buffer

    printf(YELLOW"\nSearching..."RESET);
    Sleep(500); // Add a small delay for better UX

    for (int i = 0; i < bookCount; i++)
    {
        char bookAuthor[50];
        char searchAuthor[50];
        
        // Create temporary copies for the strlwr function
        strcpy(bookAuthor, books[i].author);
        strcpy(searchAuthor, authorToSearch);
        
        if (strcmp(strlwr(bookAuthor), strlwr(searchAuthor)) == 0)
        {
            printf(GREEN"\nBook is found!\n"RESET);
            index = i;
            return index;
        }
    }

    printf(RED"\nBook is not found.\n"RESET);
    return -1;
}

// Search book by book isbn
int searchByISBN(book* books, int bookCount)
{
    int index = -1;
    char isbnToSearch[20];  // Match size with book structure

    printf(CYAN"\n<=======================================>\n"
           "||              SEARCH BY ISBN             ||\n"
           "<=======================================>\n"RESET);
    printf(CYAN"Enter ISBN: "RESET);
    scanf(" %19[^\n]", isbnToSearch);  // Prevent buffer overflow
    while (getchar() != '\n');  // Clear input buffer

    printf(YELLOW"\nSearching..."RESET);
    Sleep(500); // Add a small delay for better UX

    for (int i = 0; i < bookCount; i++)
    {
        char bookISBN[20];
        char searchISBN[20];
        
        // Create temporary copies for the strlwr function
        strcpy(bookISBN, books[i].isbn);
        strcpy(searchISBN, isbnToSearch);
        
        if (strcmp(strlwr(bookISBN), strlwr(searchISBN)) == 0)
        {
            printf(GREEN"\nBook is found!\n"RESET);
            index = i;
            return index;
        }
    }

    printf(RED"\nBook is not found.\n"RESET);
    return -1;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @STORE FUNCTION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void store(book* books, int* bookCount) {
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
    
    if (*bookCount + count > 100) {
        printf(RED"\nNot enough space. Maximum capacity is 100 books.\n"RESET);
        return;
    }

    int startCount = *bookCount;
    
    for (int i = startCount; i < (startCount + count); i++)
    {
        clearScreen();
        displayHeader();
        printf(CYAN"\n<=======================================>\n"
               "||               ADD BOOKS                ||\n"
               "<=======================================>\n"RESET);
        printf(YELLOW"\nBook #%d of %d:\n"RESET, (i - startCount) + 1, count);
        
        printf(YELLOW"Book Title: "RESET);
        scanf(" %49[^\n]", books[i].title);  // Prevent buffer overflow
        while (getchar() != '\n');  // Clear input buffer
        
        printf(YELLOW"Author: "RESET);
        scanf(" %49[^\n]", books[i].author);  // Prevent buffer overflow
        while (getchar() != '\n');  // Clear input buffer

        generateISBN(books, i);
        books[i].status = AVAILABLE;
    }
    
    *bookCount += count;
    printf(GREEN"\nSuccessfully added %d books. Total books: %d\n"RESET, count, *bookCount);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @DISPLAY FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void displayAll(book* books, int bookCount) {
    printf(CYAN"\n<=======================================>\n"
           "||              ALL BOOKS                 ||\n"
           "<=======================================>\n\n"RESET);
    
    for (int i = 0; i < bookCount; i++) {
        printf(YELLOW"<=======================================>\n"RESET);
        printf(CYAN"~~> Book #%d\n"RESET, i+1);
        printf(CYAN"~~> Title: "RESET);
        printf(GREEN"%s\n"RESET, books[i].title);
        printf(CYAN"~~> Author:  "RESET);
        printf(GREEN"%s\n"RESET, books[i].author);
        printf(CYAN"~~> ISBN: "RESET);
        printf(GREEN"%s\n"RESET, books[i].isbn);
        printf(CYAN"~~> AVAILABILITY: "RESET);
        printf("%s\n", books[i].status == AVAILABLE ? GREEN"Available"RESET : RED"Checked Out"RESET);
    }
}

void displaySingle(book* books, int index) {
    printf(YELLOW"\n<=======================================>\n"
           "||              BOOK DETAILS              ||\n"
           "<=======================================>\n\n"RESET);
    
    printf(CYAN"~~> Title: "RESET);
    printf(GREEN"%s\n"RESET, books[index].title);
    printf(CYAN"~~> Author: "RESET);
    printf(GREEN"%s\n"RESET, books[index].author);
    printf(CYAN"~~> ISBN: "RESET);
    printf(GREEN"%s\n"RESET, books[index].isbn);
    printf(CYAN"~~> AVAILABILITY: "RESET);
    printf("%s\n", books[index].status == AVAILABLE ? GREEN"Available"RESET : RED"Checked Out"RESET);
    
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

void returnBook(book* books, int index) 
{
    if (books[index].status == CHECKED_OUT) {
        books[index].status = AVAILABLE;
        printf(GREEN"\nBook has been returned successfully.\n"RESET);
    } else {
        printf(YELLOW"\nBook is already available.\n"RESET);
    }
}

void checkOutBook(book* books, int index) 
{
    if (books[index].status == AVAILABLE) {
        books[index].status = CHECKED_OUT;
        printf(GREEN"\nBook has been checked out successfully.\n"RESET);
    } else {
        printf(RED"\nBook is already checked out.\n"RESET);
    }
}
