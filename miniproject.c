#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for Flight
typedef struct Flight {
    int departure_time;
    float price;
    char source[50];
    char destination[50];
    struct Flight* left;
    struct Flight* right;
} Flight;


#define NUM_ROWS 10
#define NUM_COLS 6
#define TOTAL_SEATS (NUM_ROWS * NUM_COLS)
#define MAX_PAYMENTS 100


struct Payment {
    int transactionID;
    double amount;
    char paymentMethod[50];
};

typedef enum {
    ECONOMY,
    BUSINESS,
    FIRST
} SeatClass;

typedef struct SeatBooking {
    int seatNumber;
    SeatClass seatClass;
    int isAvailable;
    float price;
    char name[50];
    char phone[15];
    char email[50];
    char actionType[20];  // New field to indicate booking or cancellation
    struct SeatBooking* next; // Linked list for booking history
} SeatBooking;

// Function to create a new flight node
Flight* createFlight(int departure_time, float price, const char* source, const char* destination, int flightno) 
{
    Flight* newFlight = (Flight*)malloc(sizeof(Flight));
    newFlight->departure_time = departure_time;
    newFlight->price = price;
    strcpy(newFlight->source, source);
    strcpy(newFlight->destination, destination);
    newFlight->left = NULL;
    newFlight->right = NULL;
    return newFlight;
}

// Function to insert a flight into the BST based on the chosen criteria (e.g., departure time and source)
Flight* insert(Flight* root, int departure_time, float price, const char* source, const char* destination, int flightno) 
{
    if (root == NULL) 
    {
        return createFlight(departure_time, price, source, destination, flightno);
    }

    // Choose your criteria for insertion (e.g., departure time and source)
    if (departure_time < root->departure_time || (departure_time == root->departure_time && strcmp(source, root->source) < 0)) 
    {
        root->left = insert(root->left, departure_time, price, source, destination, flightno);
    } 
    else 
    {
        root->right = insert(root->right, departure_time, price, source, destination, flightno);
    }

    return root;
}

// Function to search for flights based on "from" and "to" locations
Flight* searchFlights(Flight* root, const char* from, const char* to) 
{
    if (root == NULL) 
    {
        return NULL;
    }

    // Compare "from" and "to" locations
    int fromComparison = strcmp(from, root->source);
    int toComparison = strcmp(to, root->destination);

    // Check if the current flight's source is within the range "from" to "to"
    if (fromComparison <= 0 && toComparison >= 0) 
    {
        return root;
    } 
    else if (fromComparison < 0) 
    {
        return searchFlights(root->left, from, to);
    } 
    else 
    {
        return searchFlights(root->right, from, to);
    }
}

// Function to print flight details
void printFlight(Flight* flight) 
{
    if (flight != NULL) 
    {
        printf("Departure Time: %d, Price: %.2f, Source: %s, Destination: %s\n", flight->departure_time, flight->price, flight->source, flight->destination);
    }
}

// Manually populate flight data
Flight* populateFlights() 
{
    Flight* root = NULL;

    root = insert(root, 1200, 250.00, "New York","Miami",1111);
    root = insert(root, 1230, 280.25, "New York","Chicago",5555);
    root = insert(root, 1600, 160.75, "New York","Los Angeles",6666);
    root = insert(root, 1430, 250.50, "Chicago","Los Angeles",2222);
    root = insert(root, 1130, 180.25, "Chicago","New York",9028);
    root = insert(root, 1000, 160.75, "Chicago","Miami",7026);
    root = insert(root, 900, 300.75, "Los Angeles","Miami",3333);
    root = insert(root, 1540, 150.25, "Los Angeles","New York",7777);
    root = insert(root, 1300, 140.25, "Los Angeles","Chicago",8877);
    root = insert(root, 1740, 190.75, "Miami","New York",4444);
    root = insert(root, 1630, 210.25, "Miami","Chicago",8052);
    root = insert(root, 1800, 280.25, "Miami","Los Angeles",2454);

    return root;
}

// Function to display the menu and get user input for "from" and "to" locations
void displayMenuAndGetData(char* from, char* to) 
{
    printf("Available flight locations:\n");
    printf("1. New York\n");
    printf("2. Los Angeles\n");
    printf("3. Chicago\n");
    printf("4. Miami\n");

    int fromChoice, toChoice;
    printf("Select your departure location: ");
    scanf("%d", &fromChoice);
    printf("Select your destination location: ");
    scanf("%d", &toChoice);

    // Map the numeric choices to location names
    char* locations[] = { "New York", "Los Angeles", "Chicago", "Miami" };
    strcpy(from, locations[fromChoice - 1]);
    strcpy(to, locations[toChoice - 1]);
}


// Function to create a new SeatBooking instance
SeatBooking* createSeatBooking(int seatNumber, SeatClass seatClass, float price, const char actionType[]) 
{
    SeatBooking* booking = (SeatBooking*)malloc(sizeof(SeatBooking));
    booking->seatNumber = seatNumber;
    booking->seatClass = seatClass;
    booking->isAvailable = 1;
    booking->price = price;
    strcpy(booking->name, "N/A");
    strcpy(booking->phone, "N/A");
    strcpy(booking->email, "N/A");
    strcpy(booking->actionType, actionType);  // Set the action type
    booking->next = NULL;
    return booking;
}


// Function to append a new booking to the booking history
SeatBooking* appendToBookingHistory(SeatBooking* head, int seatNumber, SeatClass seatClass, float price, const char name[], const char phone[], const char email[], const char actionType[]) 
{
    SeatBooking* booking = createSeatBooking(seatNumber, seatClass, price, actionType);
    strcpy(booking->name, name);
    strcpy(booking->phone, phone);
    strcpy(booking->email, email);

    if (head == NULL) 
    {
        return booking;
    }

    SeatBooking* current = head;
    while (current->next != NULL) 
    {
        current = current->next;
    }
    current->next = booking;
    return head;
}

// Function to cancel a seat booking
void cancelSeat(SeatBooking* seatBookings[NUM_ROWS][NUM_COLS], int seatNumber, SeatBooking** bookingHistory)
{
    int row = (seatNumber - 1) / NUM_COLS;
    int col = (seatNumber - 1) % NUM_COLS;
    SeatBooking* selectedSeat = seatBookings[row][col];

    if (selectedSeat->isAvailable)
    {
        printf("Seat %d is not booked, so it cannot be canceled.\n", seatNumber);
    }
    else
    {
        selectedSeat->isAvailable = 1;
        printf("The money will be credited to your account within 3-4 working days.\n");
        printf("Seat %d has been canceled.\n", seatNumber);

        // Add cancellation to booking history
        *bookingHistory = appendToBookingHistory(*bookingHistory, seatNumber, selectedSeat->seatClass, -selectedSeat->price,
                                                 "-", "-", "-", "Cancellation");
    }
}



// Function to display the seat map
void displaySeatMap(SeatBooking* seatBookings[NUM_ROWS][NUM_COLS]) 
{
    printf("Seat Map:\n");
    for (int i = 0; i < NUM_ROWS; i++) 
    {
        for (int j = 0; j < NUM_COLS; j++) 
        {
            SeatBooking* booking = seatBookings[i][j];
            char seatClassChar = (booking->seatClass == FIRST) ? 'F' : (booking->seatClass == BUSINESS) ? 'B' : 'E';
            char isAvailableChar = (booking->isAvailable) ? 'A' : 'O';
            printf("[%c %d %c]  ", seatClassChar, booking->seatNumber, isAvailableChar);
        }
        printf("\n");
    }
}

// Function to print booking history
void printBookingHistory(SeatBooking* bookingHistory) 
{
    int bookingNumber = 1;
    printf("\nBooking History:\n");
    SeatBooking* current = bookingHistory;

    while (current != NULL) 
    {
        printf("Booking #%d: \nSeat %d - Name: %s, Phone: %s, Email: %s\n",
               bookingNumber, current->seatNumber,
               current->name, current->phone, current->email);
        printf("    Seat Class: %s, Action: %s\n",
               (current->seatClass == FIRST) ? "First" : (current->seatClass == BUSINESS) ? "Business" : "Economy",
                current->actionType);
        current = current->next;
        bookingNumber++;
    }
}

// Function to free the memory used for booking history
void freeBookingHistory(SeatBooking* bookingHistory) 
{
    SeatBooking* current = bookingHistory;

    while (current != NULL) 
    {
        SeatBooking* next = current->next;
        free(current);
        current = next;
    }
}

// Array to store payment transactions
struct Payment paymentDatabase[MAX_PAYMENTS];
int paymentCount = 0;

// Function to add a payment to the database
void addPayment(int transactionID,double amount, const char* paymentMethod) 
{
    if (paymentCount < MAX_PAYMENTS) 
    {

        
        struct Payment newPayment;
        newPayment.transactionID = transactionID;
        newPayment.amount = amount;
        strcpy(newPayment.paymentMethod, paymentMethod);

        paymentDatabase[paymentCount] = newPayment;
        paymentCount++;
        printf("Payment added to the database.\n");
    } 
    else 
    {
        printf("Payment database is full. Cannot add more payments.\n");
    }
}

// Function to display all payment transactions
void displayPayments() 
{
    printf("Payment Transactions:\n");
    for (int i = 0; i < paymentCount; i++) 
    {
        printf("Transaction ID: %d, Amount: $%.2f, Method: %s\n",
               paymentDatabase[i].transactionID, paymentDatabase[i].amount, paymentDatabase[i].paymentMethod);
    }
}

int transact()
{
    int min = 100000000;
    int max = 999999999;
    int x = min + rand() % (max - min + 1);

    return x;
}

int main() 
{
    Flight* root = populateFlights();

    SeatBooking *seatBookings[NUM_ROWS][NUM_COLS];  

    char from[50], to[50];
    displayMenuAndGetData(from, to);

    // Search for flights based on "from" and "to" locations
    Flight* found_flight = searchFlights(root, from, to);


    // Display the found flight, if any
    if (found_flight != NULL) 
    {

    // Initialize seatBookings array
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            SeatClass seatClass;
            float price;

            if (i < 2)
            {
                seatClass = FIRST;
                price = 500.0;
            }
            else if (i < 6)
            {
                seatClass = BUSINESS;
                price = 300.0;
            }
            else
            {
                seatClass = ECONOMY;
                price = 100.0;
            }

            seatBookings[i][j] = createSeatBooking(i * NUM_COLS + j + 1, seatClass, price, "Booking"); // or any other actionType as needed

        }
    }

    SeatBooking *bookingHistory = NULL;
    int option;

    do
    {
        printf("\nMenu:\n");
        printf("1. Book a seat\n");
        printf("2. Cancel a seat\n");
        printf("3. Display booking history\n");
        printf("4. Main Menu \n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
        {
            int seatNumber;

            displaySeatMap(seatBookings);
            printf("Enter the seat number you want to book: ");
            scanf("%d", &seatNumber);

            if (seatNumber < 1 || seatNumber > TOTAL_SEATS)
            {
                printf("Invalid seat selection.\n");
                break;
            }
            
            int price;
            if (seatNumber < 2)
            {
                price = 500.0;
            }
            else if (seatNumber < 6)
            {
                price = 300.0;
            }
            else
            {
                price = 100.0;
            }

            int row = (seatNumber - 1) / NUM_COLS;
            int col = (seatNumber - 1) % NUM_COLS;
            SeatBooking *selectedSeat = seatBookings[row][col];

            if (selectedSeat->isAvailable)
            {
                char name[50], phone[15], email[50];
                printf("Enter your name: ");
                scanf(" %[^\n]s", name);
                getchar(); // Consume the newline character
                printf("Enter your phone number: ");
                scanf(" %[^\n]s", phone);
                getchar(); // Consume the newline character
                printf("Enter your email: ");
                scanf(" %[^\n]s", email);
                getchar(); // Consume the newline character

                selectedSeat->isAvailable = 0;
               bookingHistory = appendToBookingHistory(bookingHistory, selectedSeat->seatNumber, selectedSeat->seatClass, selectedSeat->price, name, phone, email, "Booking");

            }
            else
            {
                printf("Sorry, seat %d is already occupied.\n", seatNumber);
            }

            int transactionId = transact();
            char paymentMethod[50];
            int op;
            printf("Choose payment method 1. UPI 2. Credit Card 3. Debit Card: ");
            scanf("%d", &op);
            switch (op)
            {
                case 1:
                    strncpy(paymentMethod, "UPI", sizeof(paymentMethod));
                    break;
                case 2:
                    strncpy(paymentMethod, "Credit Card", sizeof(paymentMethod));
                    break;
                case 3:
                    strncpy(paymentMethod, "Debit Card", sizeof(paymentMethod));
                    break;
                default:
                    printf("Invalid payment method.\n");
                    break;
            }

            addPayment(transactionId, price, paymentMethod);
            printf("Seat %d booked successfully!\n", seatNumber);
            break;
        }

       case 2:
        {
            int seatNumber;
            displaySeatMap(seatBookings);
            printf("Enter the seat number you want to cancel: ");
            scanf("%d", &seatNumber);
            cancelSeat(seatBookings, seatNumber, &bookingHistory);  // Pass bookingHistory by reference
            break;
        }


        case 3:
            printBookingHistory(bookingHistory);
            displayPayments();
            break;

        case 4:
            displayMenuAndGetData(from, to);
            break;
        case 5:
            // Free seat bookings
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLS; j++)
                {
                    free(seatBookings[i][j]);
                }
            }

            // Free booking history
            freeBookingHistory(bookingHistory);
             return 0;

        default:
            printf("Invalid option. Please try again.\n");
        }
    }
    while (option != 5);

    } 
    else 
    {
        printf("No flight found for the specified criteria (from %s to %s).\n", from, to);
    }
}