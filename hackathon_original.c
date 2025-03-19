/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @LIBRARIES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

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
// void typewriter(const char *text, int delay_ms);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @MAIN FUNCTION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main ()
{
    // Declare array that will hold books
    struct Book books[100];

    int size = sizeof(books);
    int bookCount = 0;
    int usrChoice;

    // Main menu loop
    while (true)
    {
        printf(YELLOW"<=======================================>\n<< 1 - Store | 2 - Display | 3 - Search >> \n<=======================================>\n|>> "RESET);
        
        usrChoice = getchar();
        while (getchar() != '\n'); // reject non-numeric inputs

        switch(usrChoice)
        {
            case '1':
               store(books, &bookCount);
               break;
            case '2':
                if (bookCount > 0) displayAll(books, bookCount);
                break;
            case '3':
                printf(CYAN"<=======================================>\n<< Enter mode to search >>\n<=======================================>\n"RESET);
                printf(YELLOW"~~ 1 - By Title\t2 - By Author\n~~ 3 - By ISBN\n<=======================================>\n|=> "RESET);

                int searchType;
                int index;

                searchType = getchar();
                while (getchar() != '\n');

                switch(searchType) {
                    case '1':
                        index = searchByTitle(books, bookCount);
                        if (index > -1) displaySingle(books, index);
                        break;
                    case '2':
                        index = searchByAuthor(books, bookCount);
                        if (index > -1) displaySingle(books, index);
                        break;
                    case '3': 
                        index = searchByISBN(books, bookCount);
                        if (index > -1) displaySingle(books, index);
                        break;
                    default:
                        printf(RED"Invalid choice. Please try again.\n"RESET);
                        break;
                }

                if (index > -1)
                {
                    int option;
                    printf(CYAN"<============= Options: ================>\n"RESET);
                    printf(YELLOW"~~ 1 - Return Book\t2 - Chechout\n~~ 3 - Back to Menu\n|=> "RESET);
                    scanf("%d", &option);
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
                    }
                }
            default:
                break;
        }

    }

}

// Generate a unique isbn per book added
void generateISBN(book* books, int index)
{
    int k = 4;
    srand(time(0)); 
    for (int i = 0; i < 20; i++)
    {
        
        books[index].isbn[i] = rand() % (57 - 48 + 1) + 48;  
    }
    for (int i = 0; i < 4; i++)
    {   
        books[index].isbn[k] = '-';
        k += 4;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @SEARCH FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Search book by book title
int searchByTitle (book* books, int bookCount)
{
    int index;
    char titleToSearch[100];

    printf(CYAN"Enter book title to search: "RESET);
    scanf(" %[^\n]", &titleToSearch);

    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(strlwr(books[i].title), strlwr(titleToSearch)) == 0)
        {
            printf(GREEN"Book is found on index %d\n"RESET, i);
            index = i;
            return index;
        }
    }

    printf(RED"Book is not found.\n"RESET);
    return -1;
}


// Search book by author
int searchByAuthor (book* books, int bookCount)
{
    int index;
    char authorToSearch[100];

    printf(CYAN"<=======================================>\n|~~ Enter book author to search: "RESET);
    scanf(" %[^\n]", &authorToSearch);

    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(strlwr(books[i].author), strlwr(authorToSearch)) == 0)
        {
            printf(GREEN"Book is found on index %d.\n"RESET, i);
            index = i;
            return index;
        }
    }

    printf(RED"Book is not found.\n"RESET);
    return -1;
}

// Search book by book isbn
int searchByISBN (book* books, int bookCount)
{
    int index;
    char isbnToSearch[100];

    printf(CYAN"<=======================================>\n|~~ Enter book ISBN to search: "RESET);
    scanf(" %[^\n]", &isbnToSearch);

    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(strlwr(books[i].isbn), strlwr(isbnToSearch)) == 0)
        {
            printf(GREEN"Book is found on index %d\n"RESET, i);
            index = i;
            return index;
        }
    }

    printf(RED"Book is not found.\n"RESET);
    return -1;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @STORE FUNCTION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void store (book* books, int* bookCount) {
    int count = 0;
    printf(CYAN"<=======================================>\n|~~ Enter the # of books to add: "RESET);
    scanf("%d", &count);

    count += *bookCount;

    for (int i = *bookCount; i < count; i++)
    {
        printf(YELLOW"Book Title: "RESET);
        scanf(" %[^\n]", &books[i].title);
        
        printf(YELLOW"Author: "RESET);
        scanf(" %[^\n]", &books[i].author);

        generateISBN(books, i);

        books[i].status = AVAILABLE;

        *bookCount+=1;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @DISPLAY FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void displayAll (book* books, int bookCount) {
    for (int i = 0; i < bookCount; i++)
    {
        printf(YELLOW"<=======================================>\n"RESET);
        printf(CYAN"~~> Book #%d\n"RESET, i+1);
        printf(CYAN"~~> Title: "RESET);
        printf(GREEN"%s\n"RESET, books[i].title);
        printf(CYAN"~~> Author:  "RESET);
        printf(GREEN"%s\n"RESET, books[i].author);
        printf(CYAN"~~> ISBN: "RESET);
        printf(GREEN"%s\n"RESET, books[i].isbn);
        printf(CYAN"~~> AVAILABILITY: "RESET);
        printf(GREEN"%s\n"RESET,getAvailability(books[i].status));
    }
}

void displaySingle (book* books, int index) {
    printf(YELLOW"<=======================================>\n"RESET);
    printf(CYAN"~~> Title: "RESET);
        printf(GREEN"%s\n"RESET, books[index].title);
        printf(CYAN"~~> Author:  "RESET);
        printf(GREEN"%s\n"RESET, books[index].author);
        printf(CYAN"~~> ISBN: "RESET);
        printf(GREEN"%s\n"RESET, books[index].isbn);
        printf(CYAN"~~> AVAILABILITY: "RESET);
        printf(GREEN"%s\n"RESET,getAvailability(books[index].status));
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
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @CHECKOUT/RETURN FUNCTIONS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void returnBook (book* books, int index) 
{
    books[index].status = AVAILABLE;
}

void checkOutBook (book* books, int index) 
{
    
    books[index].status = CHECKED_OUT;
}
