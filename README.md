# Library Management System

## Project Overview
This repository contains different implementations of a Library Management System developed for the "Case Closed" hackathon competition. The system allows users to add, search, display, check out, and return books in a library.

## File Descriptions

### hackathon_original.c
- The original submission for the "Case Closed" hackathon competition
- Uses an array-based implementation to store book information
- Basic functionality includes:
  - Adding books
  - Searching books by title, author, or ISBN
  - Displaying all books or a single book
  - Checking out and returning books

### hackathon_improved.c
- An improved version of the original submission
- Still uses an array-based implementation but with enhanced features:
  - Better user interface with clear screen functionality
  - Improved menu navigation and user experience
  - More robust input handling and error checking
  - Enhanced ISBN generation
  - Maximum library capacity of 100 books

### hackathon_improved_linked-list.c
- The most advanced implementation that uses a linked list data structure
- Features include:
  - Dynamic memory allocation for unlimited book storage
  - No fixed capacity limits
  - Improved memory management with proper memory allocation/deallocation
  - All features from the improved version
  - More robust error handling

## How to Compile and Run

To compile any of the C files, use GCC:

```bash
gcc -o library filename.c
```

Replace `filename.c` with `hackathon_original.c`, `hackathon_improved.c`, or `hackathon_improved_linked-list.c`.

Then run the executable:

```bash
./library
```

## Features

- **Book Management**: Add, display, and search for books
- **Search Functionality**: Search by title, author, or ISBN
- **Check-out System**: Track book availability status
- **Color-coded Interface**: Easy-to-use, color-coded terminal interface

## Implementation Details

### Data Structure
- The original and improved versions use a static array of Book structures
- The linked list implementation uses dynamically allocated nodes

### Book Information
- Title (up to 50 characters)
- Author (up to 50 characters)
- ISBN (automatically generated)
- Status (Available or Checked Out)

## Future Improvements
- Implement binary search for faster search operations
- Add book deletion functionality
- Implement sorting capabilities
- Add data persistence through file storage
- Include more book attributes like genre, publication year, etc.