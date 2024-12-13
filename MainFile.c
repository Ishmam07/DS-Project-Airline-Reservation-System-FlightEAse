

// In the name of almighty ALLAH

// headers
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

// admin pannel credentials
#define ADMIN_USERNAME "binarybandits007"
#define ADMIN_PASSWORD "IREJS256315099232366"

// pre defination of arrow(all set)
#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER_KEY 13

typedef struct Booking
{
    int flight_number;
    char flight_name[50];
    char source[50];
    char destination[50];
    char date[15];
    char time[10];
    int seats_booked;
    char class_type[10]; // Economy / Business
    struct Booking *next;
} Booking;

typedef struct PassengerDetails
{
    int id;
    char username[20];
    char password[30];
    char email[50];
    char contact_info[15];
    char address[100];
    int flight_points;
    struct PassengerDetails *next;
    Booking *bookings;
} passenger;

typedef struct FlightDetails
{
    int flight_number;
    char flight_name[50];
    char source[50];
    char destination[50];
    char date[15];
    char time[10];
    char estimated_travel_time[10];
    char estimated_arrival_time[10];
    int available_seats;
    int economy_price;
    int business_price;
    struct FlightDetails *next;
} flight;

typedef struct PriorityQueueNode
{
    flight *flight_data;
    struct PriorityQueueNode *next;
} PriorityQueueNode;

typedef struct PriorityQueue
{
    PriorityQueueNode *front;
} PriorityQueue;

// Initialize an empty priority queue
PriorityQueue *create_priority_queue()
{
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->front = NULL;
    return pq;
}

int parse_travel_time(char *time_str)
{
    int hours = 0, minutes = 0;
    sscanf(time_str, "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

void enqueue(PriorityQueue *pq, flight *new_flight)
{
    PriorityQueueNode *new_node = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode));
    new_node->flight_data = new_flight;
    new_node->next = NULL;

    int travel_time = parse_travel_time(new_flight->estimated_travel_time);

    if (pq->front == NULL ||
        parse_travel_time(pq->front->flight_data->estimated_travel_time) > travel_time)
    {
        // Insert at the front
        new_node->next = pq->front;
        pq->front = new_node;
    }
    else
    {
        // Traverse and find the correct position
        PriorityQueueNode *current = pq->front;
        while (current->next != NULL &&
               parse_travel_time(current->next->flight_data->estimated_travel_time) <= travel_time)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

flight *dequeue(PriorityQueue *pq)
{
    if (pq->front == NULL)
        return NULL; // Empty queue

    PriorityQueueNode *temp = pq->front;
    flight *flight_data = temp->flight_data;
    pq->front = pq->front->next;
    free(temp);
    return flight_data;
}

passenger *head = NULL;
passenger *currentPassenger = NULL;

flight *flight_head = NULL;

flight *create_flight(int flight_number, char *flight_name, char *source, char *destination, char *date, char *time, char *travel_time, char *arrival_time, int available_seats, int economy_price, int business_price)
{
    flight *newFlight = (flight *)malloc(sizeof(flight));
    if (newFlight == NULL)
    {
        printf("\n\tUnable to add flight.\n");
        return NULL;
    }

    newFlight->flight_number = flight_number;
    strcpy(newFlight->flight_name, flight_name);
    strcpy(newFlight->source, source);
    strcpy(newFlight->destination, destination);
    strcpy(newFlight->date, date);
    strcpy(newFlight->time, time);
    strcpy(newFlight->estimated_travel_time, travel_time);
    strcpy(newFlight->estimated_arrival_time, arrival_time);
    newFlight->available_seats = available_seats;
    newFlight->economy_price = economy_price;   // Set the economy price
    newFlight->business_price = business_price; // Set the business price
    newFlight->next = NULL;

    return newFlight;
}

void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

void debug_flight_list()
{
    printf("\n\t\t\t\t\t\t\t\t--- Debugging Flight List ---\n");
    flight *temp = flight_head;
    if (temp == NULL)
    {
        printf("\n\t\t\t\t\t\t\t\tNo flights in the list.\n");
        return;
    }
    while (temp != NULL)
    {
        printf("\t\t\t\t\t\t\t\tFlight No: %d, Source: %s, Destination: %s\n",
               temp->flight_number, temp->source, temp->destination);
        temp = temp->next;
    }
}

// Add a flight

void add_flight()
{
    int flight_number, available_seats, economy_price, business_price;
    char flight_name[50], source[50], destination[50], date[15], time[10], travel_time[10], arrival_time[10];

    printf("\n\t\t\t\t\t\t\t\t\e[1;31mAdd Flight\033[0m\n");
    printf("\t\t\t\t\t\t\t\t==========================\n\n");

    // Prompt for flight name
    printf("\n\t\t\t\t\t\t\t\tEnter Flight Name: ");
    fgets(flight_name, sizeof(flight_name), stdin);
    flight_name[strcspn(flight_name, "\n")] = '\0'; // Remove trailing newline

    // Ensure unique flight number
    while (1)
    {
        printf("\n\t\t\t\t\t\t\t\tEnter Flight Number: ");
        if (scanf("%d", &flight_number) != 1 || flight_number <= 0) // Ensure valid input
        {
            printf("\n\t\t\t\t\t\t\t\t\033[1;31mError: Please enter a valid positive flight number.\033[0m\n");
            while (getchar() != '\n')
                ; // Clear invalid input
            continue;
        }
        getchar(); // Clear buffer

        // Check for duplicates
        flight *tmp = flight_head;
        int dup = 0;
        while (tmp != NULL)
        {
            if (tmp->flight_number == flight_number)
            {
                dup = 1;
                break;
            }
            tmp = tmp->next;
        }

        if (dup)
        {
            printf("\n\t\t\t\t\t\t\t\t\033[1;31mError: Flight Number %d already exists. Please enter a unique flight number.\033[0m\n", flight_number);
        }
        else
        {
            break; // Unique flight number
        }
    }

    // Prompt for other details
    printf("\n\t\t\t\t\t\t\t\tEnter Source: ");
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = '\0';

    printf("\n\t\t\t\t\t\t\t\tEnter Destination: ");
    fgets(destination, sizeof(destination), stdin);
    destination[strcspn(destination, "\n")] = '\0';

    printf("\n\t\t\t\t\t\t\t\tEnter Date (dd/mm/yyyy): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    printf("\n\t\t\t\t\t\t\t\tEnter Time (hh:mm): ");
    fgets(time, sizeof(time), stdin);
    time[strcspn(time, "\n")] = '\0';

    printf("\n\t\t\t\t\t\t\t\tEnter Estimated Travel Time (hh:mm): ");
    fgets(travel_time, sizeof(travel_time), stdin);
    travel_time[strcspn(travel_time, "\n")] = '\0';

    printf("\n\t\t\t\t\t\t\t\tEnter Estimated Arrival Time (hh:mm): ");
    fgets(arrival_time, sizeof(arrival_time), stdin);
    arrival_time[strcspn(arrival_time, "\n")] = '\0';

    printf("\n\t\t\t\t\t\t\t\tEnter Available Seats: ");
    while (scanf("%d", &available_seats) != 1 || available_seats <= 0)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mError: Please enter a valid positive number for available seats.\033[0m\n");
        while (getchar() != '\n')
            ; // Clear invalid input
    }
    getchar(); // Clear buffer

    printf("\n\t\t\t\t\t\t\t\tEnter Economy Price: ");
    while (scanf("%d", &economy_price) != 1 || economy_price <= 0)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mError: Please enter a valid positive number for economy price.\033[0m\n");
        while (getchar() != '\n')
            ; // Clear invalid input
    }
    getchar(); // Clear buffer

    printf("\n\t\t\t\t\t\t\t\tEnter Business Price: ");
    while (scanf("%d", &business_price) != 1 || business_price <= 0)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mError: Please enter a valid positive number for business price.\033[0m\n");
        while (getchar() != '\n')
            ; // Clear invalid input
    }
    getchar(); // Clear buffer

    // Create a new flight using the provided details
    flight *newFlight = create_flight(flight_number, flight_name, source, destination, date, time, travel_time, arrival_time, available_seats, economy_price, business_price);
    if (newFlight == NULL)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mError: Unable to allocate memory for the new flight.\033[0m\n");
        return;
    }

    // Append the new flight to the flight list
    if (flight_head == NULL)
    {
        flight_head = newFlight;
    }
    else
    {
        flight *temp = flight_head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newFlight;
    }

    printf("\n\t\t\t\t\t\t\t\t\033[1;32mFlight added successfully!\033[0m\n");

    // Wait for input to return
    printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
    getchar();
}

// Edit a flight

void edit_flight()
{
    printf("\n\t\t\t\t\t\t\t\t\033[1;31mEdit Flight Information\033[0m\n");
    printf("\t\t\t\t\t\t\t\t==========================================\n\n");

    int choice = 1; // Start at the first menu option
    int key;        // Variable to capture key press
    const char *menu[] = {
        "1. Edit Flight Name",
        "2. Edit Source",
        "3. Edit Destination",
        "4. Edit Date",
        "5. Edit Time",
        "6. Edit Estimated Travel Time",
        "7. Edit Estimated Arrival Time",
        "8. Edit Available Seats",
        "9. Edit Economy Price",
        "10. Edit Business Price",
        "11. Exit Edit Menu"};
    int menuSize = sizeof(menu) / sizeof(menu[0]);

    // Function to draw the menu
    void drawMenu()
    {
        for (int i = 0; i < menuSize; i++)
        {
            if (i + 1 == choice)                                              // Highlight the current option
                printf("\r\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n", menu[i]); // Green with arrow
            else
                printf("\t\t\t\t\t\t\t\t  %s\n", menu[i]); // Normal color
        }
    }

    // Initial draw
    drawMenu();

    // Menu navigation loop
    while (1)
    {
        key = _getch(); // Get key press

        if (key == 224)
        {                                                       // Arrow keys prefix
            key = _getch();                                     // Get the actual arrow key code
            if (key == 72)                                      // Up arrow
                choice = (choice == 1) ? menuSize : choice - 1; // Wrap to last option
            else if (key == 80)                                 // Down arrow
                choice = (choice == menuSize) ? 1 : choice + 1; // Wrap to first option

            // Redraw the menu options
            printf("\033[%dA", menuSize); // Move the cursor up to the start of the menu
            drawMenu();
        }
        else if (key == 13)
        { // Enter key
            system("cls");

            if (choice == 11) // Exit Edit Menu
                return;

            int flight_number;
            printf("\n\t\t\t\t\t\t\t\t\033[1;31mEdit Flight Information\033[0m\n");
            printf("\t\t\t\t\t\t\t\t==========================================\n\n");
            printf("\n\t\t\t\t\t\t\t\tEnter Flight Number to Edit: ");
            scanf("%d", &flight_number);
            getchar(); // Clear buffer

            flight *temp = flight_head;
            while (temp != NULL)
            {
                if (temp->flight_number == flight_number)
                {
                    // When the flight is found
                    switch (choice)
                    {
                    case 1: // Edit Flight Name
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Flight Name:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Flight Name: ");
                        fgets(temp->flight_name, sizeof(temp->flight_name), stdin);
                        temp->flight_name[strcspn(temp->flight_name, "\n")] = '\0'; // Remove newline
                        printf("\n\t\t\t\t\t\t\t\tFlight Name updated successfully!\n");
                        break;

                    case 2: // Edit Source
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Source:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Source: ");
                        fgets(temp->source, sizeof(temp->source), stdin);
                        temp->source[strcspn(temp->source, "\n")] = '\0';
                        printf("\n\t\t\t\t\t\t\t\tSource updated successfully!\n");
                        break;

                    case 3: // Edit Destination
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Destination:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Destination: ");
                        fgets(temp->destination, sizeof(temp->destination), stdin);
                        temp->destination[strcspn(temp->destination, "\n")] = '\0';
                        printf("\n\t\t\t\t\t\t\t\tDestination updated successfully!\n");
                        break;

                    case 4: // Edit Date
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Date:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Date (DD-MM-YYYY): ");
                        fgets(temp->date, sizeof(temp->date), stdin);
                        temp->date[strcspn(temp->date, "\n")] = '\0';
                        printf("\n\t\t\t\t\t\t\t\tDate updated successfully!\n");
                        break;

                    case 5: // Edit Time
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Time:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Time (HH:MM): ");
                        fgets(temp->time, sizeof(temp->time), stdin);
                        temp->time[strcspn(temp->time, "\n")] = '\0';
                        printf("\n\t\t\t\t\t\t\t\tTime updated successfully!\n");
                        break;

                    case 6: // Edit Estimated Travel Time
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Estimated Travel Time:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Estimated Travel Time (HH:MM): ");
                        fgets(temp->estimated_travel_time, sizeof(temp->estimated_travel_time), stdin);
                        temp->estimated_travel_time[strcspn(temp->estimated_travel_time, "\n")] = '\0';
                        printf("\n\t\t\t\t\t\t\t\tEstimated Travel Time updated successfully!\n");
                        break;

                    case 7: // Edit Estimated Arrival Time
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Estimated Arrival Time:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Estimated Arrival Time (HH:MM): ");
                        fgets(temp->estimated_arrival_time, sizeof(temp->estimated_arrival_time), stdin);
                        temp->estimated_arrival_time[strcspn(temp->estimated_arrival_time, "\n")] = '\0';
                        printf("\n\t\t\t\t\t\t\t\tEstimated Arrival Time updated successfully!\n");
                        break;

                    case 8: // Edit Available Seats
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Available Seats:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Number of Available Seats: ");
                        scanf("%d", &temp->available_seats);
                        getchar(); // Clear buffer
                        printf("\n\t\t\t\t\t\t\t\tAvailable Seats updated successfully!\n");
                        break;

                    case 9: // Edit Economy Price
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Economy Price:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Economy Price: ");
                        scanf("%d", &temp->economy_price);
                        getchar(); // Clear buffer
                        printf("\n\t\t\t\t\t\t\t\tEconomy Price updated successfully!\n");
                        break;

                    case 10: // Edit Business Price
                        printf("\n\t\t\t\t\t\t\t\t\033[1;31mEditing Business Price:\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t==========================================\n\n");
                        printf("\t\t\t\t\t\t\t\tEnter New Business Price: ");
                        scanf("%d", &temp->business_price);
                        getchar(); // Clear buffer
                        printf("\n\t\t\t\t\t\t\t\tBusiness Price updated successfully!\n");
                        break;
                    }

                    Sleep(1000); // Pause to let user see the update
                    system("cls");
                    break;
                }
                temp = temp->next;
            }

            if (temp == NULL)
            {
                printf("\n\t\t\t\t\t\t\t\tFlight not found.\n");
                Sleep(1000);
                system("cls");
            }
        }
    }
}

void cancel_flight()
{
    int flight_number;
    printf("\033[1;31m"); // Bold Red
    printf("\n\t\t\t\t\t\t\t\tCancel Flight\n");
    printf("\t\t\t\t\t\t\t\t======================\n");
    printf("\033[0m");
    printf("\n\n\t\t\t\t\t\t\t\tEnter Flight Number to Cancel: ");
    scanf("%d", &flight_number);

    flight *temp = flight_head, *prev = NULL;
    while (temp != NULL)
    {
        if (temp->flight_number == flight_number)
        {
            if (prev == NULL)
            {
                flight_head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            fflush(stdin);
            printf("\n\t\t\t\t\t\t\t\t\e[1;32mFlight cancelled successfully.\033[0m\n");
            printf("\n\t\t\t\t\t\t\t\tPress Enter to return to menu...\n");
            getchar();
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("\n\t\t\t\t\t\t\t\tFlight not found.\n");
}

void reschedule_flight()
{
    int flight_number;
   printf("\033[1;31m"); // Bold Red
    printf("\n\t\t\t\t\t\t\t\tReschedule Flight\n");
    printf("\t\t\t\t\t\t\t\t============================\n");
    printf("\033[0m");
    printf("\n\t\t\t\t\t\t\t\tEnter Flight Number to Reschedule: ");
    scanf("%d", &flight_number);

    flight *temp = flight_head;

    while (temp != NULL)
    {
        if (temp->flight_number == flight_number)
        {
            printf("\n\t\t\t\t\t\t\t\tRescheduling Flight %d\n", flight_number);
            printf("\n\t\t\t\t\t\t\t\tEnter New Date (dd/mm/yyyy): ");
            getchar();
            fgets(temp->date, sizeof(temp->date), stdin);
            temp->date[strcspn(temp->date, "\n")] = '\0';
            printf("\n\t\t\t\t\t\t\t\tEnter New Time (hh:mm): ");
            fgets(temp->time, sizeof(temp->time), stdin);
            temp->time[strcspn(temp->time, "\n")] = '\0';

            fflush(stdin);
            printf("\n\t\t\t\t\t\t\t\t\e[1;32mFlight rescheduled successfully.\033[0m\n");
            printf("\n\t\t\t\t\t\t\t\tPress Enter to return to menu...\n");
            getchar();
            return;
        }
        temp = temp->next;
    }
    printf("\n\t\t\t\t\t\t\t\tFlight not found.\n");
}

void view_flights()
{
    system("cls");

    printf("\n\t\t\t\t\t\t\t\t\t\t\e[1;31mAvailable Flights\033[0m\n\n");

    // Create and populate the priority queue
    PriorityQueue *pq = create_priority_queue();
    flight *current = flight_head;

    while (current != NULL)
    {
        enqueue(pq, current); // Add all flights to the priority queue
        current = current->next;
    }

    // Header for flight table
    printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
    printf(" \e[1;34m| %-5s | %-15s | %-15s | %-25s | %-12s | %-8s | %-14s | %-14s | %-6s | %-12s | %-14s |\033[0m\n",
           "No", "Source", "Destination", "Flight Name", "Date", "Time", "Travel Time", "Arrival Time", "Seats", "Economy Price", "Business Price");
    printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");

    // Display sorted flights
    int count = 1;
    while ((current = dequeue(pq)) != NULL)
    {
        printf(" \e[0;32m| %-5d | %-15s | %-15s | %-25s | %-12s | %-8s | %-14s | %-14s | %-6d | %-12d  | %-14d |\033[0m\n",
               current->flight_number, current->source, current->destination, current->flight_name, current->date, current->time,
               current->estimated_travel_time, current->estimated_arrival_time, current->available_seats,
               current->economy_price, current->business_price);
        printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
    }

    // Prompt to return to the menu
    printf("\n\tPress Enter to return to the menu...");
    getchar();
}

// Flight management menu
void flight_management_menu()
{
    while (1)
    {
        int choice = 1; // Start at the first menu option
        int key;        // Variable to capture key press
        const char *menu[] = {
            "1. Add Flight",
            "2. View Flights",
            "3. Edit Flight",
            "4. Cancel Flight",
            "5. Reschedule Flight",
            "6. Exit"};
        int menuSize = sizeof(menu) / sizeof(menu[0]);

        // Function to draw the menu dynamically
        void drawMenu()
        {
            // Menu header
            printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mFlight Management\033[0m\n");
            printf("\t\t\t\t\t\t\t\t\t==========================\n\n");

            // Loop through the menu items and highlight the current choice
            for (int i = 0; i < menuSize; i++)
            {
                if (i + 1 == choice)                                              // Highlight the current option
                    printf("\r\t\t\t\t\t\t\t\t\t\e[1;32m> %s\033[0m\n", menu[i]); // Green with arrow
                else
                    printf("\t\t\t\t\t\t\t\t\t  %s\n", menu[i]); // Normal color
            }
        }

        // Initial draw
        system("cls"); // Clear the screen once at the beginning
        drawMenu();    // Draw the initial menu

        // Menu navigation loop
        while (1)
        {
            key = _getch(); // Get key press

            if (key == 224) // Arrow keys prefix
            {
                key = _getch();                                     // Get the actual arrow key code
                if (key == 72)                                      // Up arrow
                    choice = (choice == 1) ? menuSize : choice - 1; // Wrap to last option
                else if (key == 80)                                 // Down arrow
                    choice = (choice == menuSize) ? 1 : choice + 1; // Wrap to first option

                // Clear screen and redraw menu options
                system("cls");
                drawMenu();
            }
            else if (key == 13) // Enter key
            {
                system("cls");
                switch (choice)
                {
                case 1:
                    add_flight();
                    break;
                case 2:
                    view_flights();
                    break;
                case 3:
                    edit_flight();
                    break;
                case 4:
                    cancel_flight();
                    break;
                case 5:
                    reschedule_flight();
                    break;
                case 6:
                    return; // Exit the flight management menu
                default:
                    printf("\n\t\t\t\t\t\t\t\t\tInvalid choice. Try again.\n");
                }
                break; // Exit inner navigation loop after handling the option
            }
        }
    }
}

void view_recent_bookings(passenger *current_passenger)
{
    // Check if the passenger has no bookings
    if (!current_passenger->bookings)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mView Recent Bookings\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\e[1;31mNo recent bookings found.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\tPress Enter to continue...");

        // Ensure any leftover characters are cleared
        // while (getchar() != '\n')
        ;

        // Wait for the user to press Enter
        getchar();
        return;
    }

    // Clear the screen
    system("cls");

    // Header for the recent bookings display
    printf("\n\t\t\t\t\t\t\t\t\t\t\e[1;31mRecent Bookings\033[0m\n\n");

    // Print the table frame and column headers
    printf("\t\t\t \e[1;31m+---------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
    printf("\t\t\t \e[1;34m| %-5s | %-25s | %-15s | %-15s | %-12s | %-8s | %-6s | %-14s |\033[0m\n",
           " No", "      Flight Name", "    Source", "  Destination", "   Date", "  Time", " Seats", "  Class");
    printf("\t\t\t \e[1;31m+---------------------------------------------------------------------------------------------------------------------------+\033[0m\n");

    // Display the bookings in a loop
    Booking *current = current_passenger->bookings;
    // flight *tmp = flight_head;
    //  int count = 1; // Counter for serial numbers

    while (current != NULL)
    {
        // Display booking details row by row
        printf("\t\t\t \e[0;32m|  %-5d| %-25s | %-15s | %-15s | %-12s | %-8s | %-6d | %-14s |\033[0m\n",
               current->flight_number,
               current->flight_name,
               current->source,
               current->destination,
               current->date,
               current->time,
               current->seats_booked,
               current->class_type);

        printf("\t\t\t \e[1;31m+---------------------------------------------------------------------------------------------------------------------------+\033[0m\n");

        current = current->next; // Move to the next booking
    }

    // Prompt the user to press Enter to return
    printf("\n\t\t\tPress Enter to continue...");

    // Clear any leftover input in the buffer
    // while (getchar() != '\n')
    ;

    // Wait for the user to press Enter
    getchar();
}

int generate_id()
{
    static int id = 1000;
    return id++;
}

passenger *create_passenger(char *username, char *password, char *email, char *contact_info, char *address)
{
    passenger *newPassenger = (passenger *)malloc(sizeof(passenger));
    newPassenger->id = generate_id();
    strcpy(newPassenger->username, username);
    strcpy(newPassenger->password, password);
    strcpy(newPassenger->email, email);
    strcpy(newPassenger->contact_info, contact_info);
    strcpy(newPassenger->address, address);
    newPassenger->flight_points = 0;
    newPassenger->bookings = NULL;
    newPassenger->next = NULL;

    return newPassenger;
}

int calculate_price(int duration_minutes, passenger *current_passenger)
{
    // Normalize travel time to a range between 5000 and 12000
    int min_price = 5000;
    int max_price = 12000;

    // Calculate the base economy price
    int normalized_price = min_price + (max_price - min_price) * (240 - duration_minutes) / 180;

    // Apply a 10% discount if the passenger has 50 or more flight points
    if (current_passenger && current_passenger->flight_points >= 50)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;32mCongratulations! You earned a 10%% discount for accumulating 50+ flight points.\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\033[1;34m50 flight points have been deducted from your account.\033[0m\n");

        normalized_price = (int)(normalized_price * 0.9); // Apply 10% discount
        current_passenger->flight_points -= 50;           // Deduct 50 points
    }

    return normalized_price;
}

void initialize_test_data()
{
    // Create predefined passengers
    passenger *passenger1 = create_passenger("john_doe", "qwerty", "john@example.com", "123-456-7890", "123 Elm St");
    passenger *passenger2 = create_passenger("jane_doe", "securepass", "jane@example.com", "987-654-3210", "456 Oak St");
    passenger *passenger3 = create_passenger("susan_smith", "mypassword", "susan@example.com", "555-123-4567", "789 Pine St");

    // Link passengers together
    head = passenger1;
    passenger1->next = passenger2;
    passenger2->next = passenger3;

    // Create flights with adjusted prices based on travel time and business class price being 25% more
    flight *flight1 = create_flight(
        69,
        "Biman Bangladesh Airlines",
        "Barishal",
        "Dhaka",
        "27/11/2024",
        "8:30am",
        "1h 30m",
        "10:00am",
        120,
        calculate_price(90, passenger1),              // Economy Price for 1h 30m flight
        (int)(calculate_price(90, passenger1) * 1.25) // Business Price (25% more than Economy)
    );

    flight *flight2 = create_flight(
        70,
        "US-Bangla Airlines",
        "Sylhet",
        "Chittagong",
        "30/11/2024",
        "2:30pm",
        "2h 15m",
        "4:45pm",
        155,
        calculate_price(135, passenger2),              // Economy Price for 2h 15m flight
        (int)(calculate_price(135, passenger2) * 1.25) // Business Price
    );

    flight *flight3 = create_flight(
        71,
        "Novoair",
        "Dhaka",
        "Sylhet",
        "01/12/2024",
        "3:00pm",
        "3h 00m",
        "6:00pm",
        200,
        calculate_price(180, passenger3),              // Economy Price for 3h flight
        (int)(calculate_price(180, passenger3) * 1.25) // Business Price
    );

    flight *flight4 = create_flight(
        72,
        "Regent Airways",
        "Chittagong",
        "Dhaka",
        "03/12/2024",
        "7:00am",
        "2h 30m",
        "9:30am",
        180,
        calculate_price(150, passenger1),              // Economy Price for 2h 30m flight
        (int)(calculate_price(150, passenger1) * 1.25) // Business Price
    );

    flight *flight5 = create_flight(
        73,
        "Air Astra",
        "Dhaka",
        "Chittagong",
        "05/12/2024",
        "6:00pm",
        "1h 20m",
        "7:20pm",
        100,
        calculate_price(80, passenger2),              // Economy Price for 1h 20m flight
        (int)(calculate_price(80, passenger2) * 1.25) // Business Price
    );

    flight *flight6 = create_flight(
        74,
        "US-Bangla Airlines",
        "Barishal",
        "Dhaka",
        "27/11/2024",
        "6:00am",
        "1h 30m",
        "7:30am",
        150,
        calculate_price(90, passenger1),              // Economy Price for 1h 30m flight
        (int)(calculate_price(90, passenger1) * 1.25) // Business Price
    );

    flight *flight7 = create_flight(
        75,
        "Novoair",
        "Sylhet",
        "Chittagong",
        "30/11/2024",
        "4:00pm",
        "2h 00m",
        "6:00pm",
        160,
        calculate_price(120, passenger2),              // Economy Price for 2h flight
        (int)(calculate_price(120, passenger2) * 1.25) // Business Price
    );

    flight *flight8 = create_flight(
        76,
        "Biman Bangladesh Airlines",
        "Dhaka",
        "Sylhet",
        "02/12/2024",
        "9:00am",
        "2h 30m",
        "11:30am",
        120,
        calculate_price(150, passenger1),              // Economy Price for 2h 30m flight
        (int)(calculate_price(150, passenger1) * 1.25) // Business Price
    );

    flight *flight9 = create_flight(
        77,
        "Regent Airways",
        "Chittagong",
        "Dhaka",
        "03/12/2024",
        "2:00pm",
        "2h 00m",
        "4:00pm",
        140,
        calculate_price(120, passenger2),              // Economy Price for 2h flight
        (int)(calculate_price(120, passenger2) * 1.25) // Business Price
    );

    flight *flight10 = create_flight(
        78,
        "Air Astra",
        "Barishal",
        "Dhaka",
        "28/11/2024",
        "10:00am",
        "1h 45m",
        "11:45am",
        90,
        calculate_price(105, passenger3),              // Economy Price for 1h 45m flight
        (int)(calculate_price(105, passenger3) * 1.25) // Business Price
    );

    flight *flight11 = create_flight(
        79,
        "US-Bangla Airlines",
        "Dhaka",
        "Sylhet",
        "06/12/2024",
        "10:00am",
        "1h 30m",
        "11:30am",
        70,
        calculate_price(90, passenger1),              // Economy Price for 1h 30m flight
        (int)(calculate_price(90, passenger1) * 1.25) // Business Price
    );

    flight *flight12 = create_flight(
        80,
        "Novoair",
        "Cumilla",
        "Dhaka",
        "07/12/2024",
        "5:30pm",
        "1h 30m",
        "7:00pm",
        85,
        calculate_price(90, passenger2),              // Economy Price for 1h 30m flight
        (int)(calculate_price(90, passenger2) * 1.25) // Business Price
    );

    // Link the flights
    flight_head = flight1;
    flight1->next = flight2;
    flight2->next = flight3;
    flight3->next = flight4;
    flight4->next = flight5;
    flight5->next = flight6;
    flight6->next = flight7;
    flight7->next = flight8;
    flight8->next = flight9;
    flight9->next = flight10;
    flight10->next = flight11;
    flight11->next = flight12;

    // Optionally save data
    // save_passenger_data();
    // save_flight_data();
}

void draw_flight_book_Menu(char *menu[], int menuSize, int choice)
{
    for (int i = 0; i < menuSize; i++)
    {
        if (i + 1 == choice)                                                // Highlight the current option
            printf("\r\t\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n", menu[i]); // Green with arrow
        else
            printf("\t\t\t\t\t\t\t\t\t  %s\n", menu[i]); // Normal color
    }
}

int payment_process_bkash(char *class_type, int total_amount)
{
    char account_number[15], account_pin[6]; // Account number and PIN
    int otp, entered_otp;
    int entered_amount;
    int max_attempts = 3; // Maximum attempts for PIN and amount

    // Prompt for bKash account number
    printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter your bKash account number:\033[0m ");
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0';

    // Generate OTP
    srand(time(NULL));
    otp = rand() % 9000 + 1000; // Generate a random 4-digit number

    // Simulate OTP being sent
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;34mAn OTP has been sent to your bKash account: %s.\033[0m\n", account_number);
    printf("\t\t\t\t\t\t\t\t\t\033[1;34mYour OTP is: %d (for testing purposes).\033[0m\n", otp);

    // Prompt user to enter OTP
    printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter the OTP:\033[0m ");
    scanf("%d", &entered_otp);
    getchar(); // Clear buffer

    if (entered_otp != otp)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mOTP verification failed. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed
    }

    // Prompt user to enter bKash account PIN
    int pin_attempts = max_attempts;
    while (pin_attempts > 0)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter your bKash account PIN:\033[0m ");
        fgets(account_pin, sizeof(account_pin), stdin);
        account_pin[strcspn(account_pin, "\n")] = '\0'; // Remove newline character

        if (strcmp(account_pin, "1234") == 0) // Example: hardcoded correct PIN for testing
        {
            break; // PIN is correct
        }
        else
        {
            pin_attempts--;
            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mIncorrect PIN. You have %d attempts remaining.\033[0m\n", pin_attempts);
            if (pin_attempts == 0)
            {
                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mPayment unsuccessful due to incorrect PIN.\033[0m\n");
                printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
                getchar();
                return 0; // Payment failed
            }
        }
    }

    // Prompt user to enter the amount
    int amount_attempts = max_attempts;
    while (amount_attempts > 0)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter the amount to pay\033[0m \e[1;32m(Total: %d BDT):\033[0m ", total_amount);
        scanf("%d", &entered_amount);
        getchar(); // Clear buffer

        if (entered_amount == total_amount)
        {
            break; // Amount is correct
        }
        else
        {
            amount_attempts--;
            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mIncorrect amount entered. You have %d attempts remaining.\033[0m\n", amount_attempts);
            if (amount_attempts == 0)
            {
                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mPayment unsuccessful due to incorrect amount.\033[0m\n");
                printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
                getchar();
                return 0; // Payment failed
            }
        }
    }

    // Payment successful
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mPayment successful! You paid %d BDT for %s class.\033[0m\n", total_amount, class_type);
    return 1; // Payment successful
}

int payment_process_nagad(char *class_type, int total_amount)
{
    char account_number[15], account_pin[6]; // Account number and PIN
    int otp, entered_otp;
    int entered_amount;

    // Prompt for Nagad account number
    printf("\n\t\t\t\t\t\t\t\t\tEnter your Nagad account number: ");
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0';

    // Generate OTP
    srand(time(NULL));
    otp = rand() % 9000 + 1000; // Generate a random 4-digit number

    // Simulate OTP being sent
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;34mAn OTP has been sent to your Nagad account: %s.\033[0m\n", account_number);
    printf("\t\t\t\t\t\t\t\t\t\t\033[1;34mYour OTP is: %d (for testing purposes).\033[0m\n", otp);

    // Prompt user to enter OTP
    printf("\n\t\t\t\t\t\t\t\t\tEnter the OTP: ");
    scanf("%d", &entered_otp);
    getchar(); // Clear buffer

    if (entered_otp != otp)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mOTP verification failed. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed
    }

    // Prompt user to enter Nagad account PIN
    printf("\n\t\t\t\t\t\t\t\t\tEnter your Nagad account PIN: ");
    fgets(account_pin, sizeof(account_pin), stdin);
    account_pin[strcspn(account_pin, "\n")] = '\0'; // Remove newline character

    // Simulate PIN validation (for testing purposes, the PIN is always "1234")
    if (strcmp(account_pin, "1234") != 0) // Example: hardcoded correct PIN for testing
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mIncorrect PIN. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed due to incorrect PIN
    }

    // Prompt user to enter the amount
    printf("\n\t\t\t\t\t\t\t\t\tEnter the amount to pay (Total: %d BDT): ", total_amount);
    scanf("%d", &entered_amount);
    getchar(); // Clear buffer

    if (entered_amount != total_amount)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mIncorrect amount entered. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed
    }

    // Payment successful
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mPayment successful! You paid %d BDT for %s class.\033[0m\n", total_amount, class_type);
    return 1; // Payment successful
}

int payment_process_rocket(char *class_type, int total_amount)
{
    char account_number[15], account_pin[6]; // Account number and PIN
    int otp, entered_otp;
    int entered_amount;

    // Prompt for Rocket account number
    printf("\n\t\t\t\t\t\t\t\t\tEnter your Rocket account number: ");
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0';

    // Generate OTP
    srand(time(NULL));
    otp = rand() % 9000 + 1000; // Generate a random 4-digit number

    // Simulate OTP being sent
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;34mAn OTP has been sent to your Rocket account: %s.\033[0m\n", account_number);
    printf("\t\t\t\t\t\t\t\t\t\033[1;34mYour OTP is: %d (for testing purposes).\033[0m\n", otp);

    // Prompt user to enter OTP
    printf("\n\t\t\t\t\t\t\t\t\tEnter the OTP: ");
    scanf("%d", &entered_otp);
    getchar(); // Clear buffer

    if (entered_otp != otp)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mOTP verification failed. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed
    }

    // Prompt user to enter Rocket account PIN
    printf("\n\t\t\t\t\t\t\t\t\tEnter your Rocket account PIN: ");
    fgets(account_pin, sizeof(account_pin), stdin);
    account_pin[strcspn(account_pin, "\n")] = '\0'; // Remove newline character

    // for testing purposes, the PIN is always "1234"
    if (strcmp(account_pin, "1234") != 0) // Example: hardcoded correct PIN for testing
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mIncorrect PIN. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed due to incorrect PIN
    }

    // Prompt user to enter the amount
    printf("\n\t\t\t\t\t\t\t\t\tEnter the amount to pay (Total: %d BDT): ", total_amount);
    scanf("%d", &entered_amount);
    getchar(); // Clear buffer

    if (entered_amount != total_amount)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mIncorrect amount entered. Payment unsuccessful.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
        getchar();
        return 0; // Payment failed
    }

    // Payment successful
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mPayment successful! You paid %d BDT for %s class.\033[0m\n", total_amount, class_type);
    return 1; // Payment successful
}

void save_bookings_to_file(passenger *head)
{
    FILE *file = fopen("bookings.txt", "w");
    if (file == NULL)
    {
        printf("Error opening bookings file for writing.\n");
        return;
    }

    passenger *current_passenger = head;
    while (current_passenger != NULL)
    {
        Booking *current_booking = current_passenger->bookings;
        while (current_booking != NULL)
        {
            fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%d|%d\n",
                    current_passenger->username,   // Passenger's username
                    current_booking->flight_name,  // Flight name
                    current_booking->source,       // Source
                    current_booking->destination,  // Destination
                    current_booking->date,         // Date
                    current_booking->time,         // Time
                    current_booking->class_type,   // Class type
                    current_booking->seats_booked, // Number of seats booked
                    current_booking->flight_number // Flight number
            );
            current_booking = current_booking->next;
        }
        current_passenger = current_passenger->next;
    }

    fclose(file);
}

void load_bookings_from_file(passenger *head)
{
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL)
    {
        printf("Bookings file not found. Starting fresh.\n");
        return;
    }

    char line[300];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char username[30], flight_name[50], source[50], destination[50];
        char date[20], time[10], class_type[10];
        int seats_booked, flight_number;

        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d",
               username, flight_name, source, destination,
               date, time, class_type, &seats_booked, &flight_number);

        passenger *current_passenger = head;
        while (current_passenger != NULL)
        {
            if (strcmp(current_passenger->username, username) == 0)
            {
                // Found the matching passenger, add booking
                Booking *new_booking = (Booking *)malloc(sizeof(Booking));
                strcpy(new_booking->flight_name, flight_name);
                strcpy(new_booking->source, source);
                strcpy(new_booking->destination, destination);
                strcpy(new_booking->date, date);
                strcpy(new_booking->time, time);
                new_booking->seats_booked = seats_booked;
                strcpy(new_booking->class_type, class_type);
                new_booking->flight_number = flight_number;
                new_booking->next = current_passenger->bookings;
                current_passenger->bookings = new_booking;
                break;
            }
            current_passenger = current_passenger->next;
        }
    }

    fclose(file);
}

void add_booking(passenger *current_passenger, char *flight_name, char *source, char *destination, char *date, char *time, int seats_booked, char *class_type, int flight_number)
{
    // Allocate memory for the new booking
    Booking *new_booking = (Booking *)malloc(sizeof(Booking));
    if (new_booking == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // Populate the booking data
    strcpy(new_booking->flight_name, flight_name);
    strcpy(new_booking->source, source);
    strcpy(new_booking->destination, destination);
    strcpy(new_booking->date, date);
    strcpy(new_booking->time, time);
    new_booking->seats_booked = seats_booked;
    new_booking->flight_number = flight_number; // Add flight number
    strcpy(new_booking->class_type, class_type);

    // Add the new booking to the passenger's bookings list
    new_booking->next = current_passenger->bookings;
    current_passenger->bookings = new_booking; // Insert at the beginning of the list

    save_bookings_to_file(current_passenger);
}

void book_flight(passenger *current_passenger)
{
    char student_name[50], student_id[20], student_email[50]; // For Rapid Student Booking
    char source[50], destination[50];                         // For filtering flights
    char key;                                                 // For navigation
    int is_student = 0, varsity_intial[50];                   // Flag for student booking

    // Step 1: Display All Available Departures
    system("cls");
    printf("\n\t\t\t\t\t\t\t\t\t\t\033[1;31mAvailable Departures\033[0m\n\n");

    flight *current = flight_head;
    int count = 1;

    // Display flight details
    printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
    printf(" \e[1;34m| %-5s | %-15s | %-15s | %-25s | %-12s | %-8s | %-14s | %-14s | %-6s | %-12s | %-14s |\033[0m\n",
           "No", "Source", "Destination", "Flight Name", "Date", "Time", "Travel Time", "Arrival Time", "Seats", "Economy Price", "Business Price");
    printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");

    while (current != NULL)
    {
        printf(" \e[0;32m| %-5d | %-15s | %-15s | %-25s | %-12s | %-8s | %-14s | %-14s | %-6d | %-12d  | %-14d |\033[0m\n",
               current->flight_number, current->source, current->destination, current->flight_name, current->date, current->time,
               current->estimated_travel_time, current->estimated_arrival_time, current->available_seats,
               current->economy_price, current->business_price);
        printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
        current = current->next;
    }

    printf("\n\e[1;34mPress Enter to continue to booking options...\033[0m\n");
    getchar();

    // Step 2: Choose Booking Type (Student or Normal)
    int booking_choice = 1; // Default to Rapid Student Booking
    while (1)
    {
        system("cls");
        printf("\n\t\t\t\t\t\t\t\t\t\t\033[1;31mChoose Booking Type\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\t\033[1;31m====================================\033[0m\n\n");

        if (booking_choice == 1)
        {
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> Rapid Student Booking\033[0m\n\n");
            printf("\t\t\t\t\t\t\t\t\t  Normal Booking\n");
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t  Rapid Student Booking\n\n");
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> Normal Booking\033[0m\n");
        }

        key = _getch();
        if (key == 13)
            break; // Enter key to confirm
        if (key == 72)
            booking_choice = (booking_choice == 1) ? 2 : 1; // Up arrow
        if (key == 80)
            booking_choice = (booking_choice == 2) ? 1 : 2; // Down arrow
    }

    is_student = (booking_choice == 1);

    // Step 3: If Student, Collect Details
    if (is_student)
    {
        system("cls");
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mRapid Student Booking\033[0m\n");
        printf("\t\t\t\t\t\t\t\t=====================================\n\n");

        printf("\t\t\t\t\t\t\t\tEnter Your Name: ");
        fgets(student_name, sizeof(student_name), stdin);
        student_name[strcspn(student_name, "\n")] = '\0';

        printf("\t\t\t\t\t\t\t\tEnter Your University ID: ");
        fgets(student_id, sizeof(student_id), stdin);
        student_id[strcspn(student_id, "\n")] = '\0';

        // printf("\t\t\t\t\t\t\t\tEnter Your University Initial: ");
        // fgets(varsity_intial, sizeof(varsity_intial), stdin);
        // varsity_intial[strcspn(varsity_intial, "\n")] = '\0';

        // Loop for email validation
        do
        {
            printf("\t\t\t\t\t\t\t\tEnter Your University Email: ");
            fgets(student_email, sizeof(student_email), stdin);
            student_email[strcspn(student_email, "\n")] = '\0';

            if (strlen(student_email) == 0)
            {
                printf("\n\t\t\t\t\t\t\t\t\033[1;31mEmail cannot be empty. Please try again.\033[0m\n");
            }
            else if (strchr(student_email, '@') == NULL)
            {
                printf("\n\t\t\t\t\t\t\t\t\033[1;31mInvalid email address. Must contain '@'. Please try again.\033[0m\n");
            }
        } while (strlen(student_email) == 0 || strchr(student_email, '@') == NULL);

        printf("\n\t\t\t\t\t\t\t\t\033[1;32mStudent Verification Complete. Proceeding to booking...\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\tPress Enter to continue...\n");
        getchar();
    }

    // Step 4: Input Source and Destination
    system("cls");
    printf("\n\t\t\t\t\t\t\t\t\033[1;31mFlight Booking System\033[0m\n");
    printf("\t\t\t\t\t\t\t\t==========================================\n\n");

    printf("\t\t\t\t\t\t\t\tEnter Source: ");
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = '\0';

    printf("\t\t\t\t\t\t\t\tEnter Destination: ");
    fgets(destination, sizeof(destination), stdin);
    destination[strcspn(destination, "\n")] = '\0';

    // Step 5: Display Matching Flights
    PriorityQueue *pq = create_priority_queue();
    current = flight_head;
    int found = 0;

    while (current != NULL)
    {
        if (strcasecmp(current->source, source) == 0 && strcasecmp(current->destination, destination) == 0)
        {
            enqueue(pq, current);
            found = 1;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mSearching for flights\033[0m");
        for (int i = 0; i < 5; i++)
        {
            printf("\e[1;31m.\033[0m");
            fflush(stdout);
            usleep(200000);
        }

        printf("\n\t\t\t\t\t\t\t\t\033[1;31mNo flights found for the specified route.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu\n");
        getchar();
        return;
    }

    printf("\n\t\t\t\t\t\t\t\t\e[1;34mSearching for flights\033[0m");
    for (int i = 0; i < 5; i++)
    {
        printf("\e[1;31m.\033[0m");
        fflush(stdout);
        usleep(200000);
    }

    system("cls");
    printf("\n\t\t\t\t\t\t\t\t\t\t\033[1;31mMatching Flights\033[0m\n\n");

    printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
    printf(" \e[1;34m| %-5s | %-15s | %-15s | %-25s | %-12s | %-8s | %-14s | %-14s | %-6s | %-12s | %-14s |\033[0m\n",
           "No", "Source", "Destination", "Flight Name", "Date", "Time", "Travel Time", "Arrival Time", "Seats", "Economy Price", "Business Price");
    printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");

    PriorityQueueNode *node = pq->front;
    count = 1;
    while (node != NULL)
    {
        flight *f = node->flight_data;
        printf(" \e[0;32m| %-5d | %-15s | %-15s | %-25s | %-12s | %-8s | %-14s | %-14s | %-6d | %-12d  | %-14d |\033[0m\n",
               f->flight_number, f->source, f->destination, f->flight_name, f->date, f->time,
               f->estimated_travel_time, f->estimated_arrival_time, f->available_seats,
               f->economy_price, f->business_price);
        printf(" \e[1;31m+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\033[0m\n");
        node = node->next;
    }

    // Ask the user for the flight number they want to book
    int flight_number, tickets;
    printf("\n\t\t\t\t\t\t\t\t\e[1;34mEnter Flight Number to Book:\033[0m ");
    scanf("%d", &flight_number);
    getchar(); // Clear buffer

    flight *selected_flight = NULL;
    node = pq->front;
    while (node != NULL)
    {
        if (node->flight_data->flight_number == flight_number)
        {
            selected_flight = node->flight_data;
            break;
        }
        node = node->next;
    }

    if (!selected_flight)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mInvalid Flight Number. Booking failed.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu\n");
        getchar();
        return;
    }

    // Ask the user for the class (Economy / Business)
    int class_choice = 1; // 1 for Economy, 2 for Business
    // char key;
    while (1)
    {
        system("cls"); // Clear screen
        printf("\n\t\t\t\t\t\t\t\t\t\t\e[1;31mChoose Class\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\t\033[1;31m===========================\033[0m\n\n");

        if (class_choice == 1)
        {
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> Economy\033[0m\n\n");
            printf("\t\t\t\t\t\t\t\t\t  Business\n");
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t  Economy\n\n");
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> Business\033[0m\n");
        }
        key = _getch();
        if (key == 13)
            break; // Enter key to select
        if (key == 72)
            class_choice = 1; // Up arrow
        if (key == 80)
            class_choice = 2; // Down arrow
    }

    // Ask for the number of tickets
    printf("\n\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter Number of Tickets:\033[0m ");
    scanf("%d", &tickets);
    getchar(); // Clear buffer

    // Check seat availability
    if (selected_flight->available_seats < tickets)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mNot enough seats available. Booking failed.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu\n");
        getchar();
        return;
    }

    int price = (class_choice == 1) ? selected_flight->economy_price : selected_flight->business_price;
    int total_cost = price * tickets;

    if (is_student)
    {
        total_cost *= 0.95;
    }

    // Check if passenger is eligible for a discount based on flight points
    if (current_passenger->flight_points >= 50)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mYou are eligible for a 10%% discount due to 50+ flight points!\033[0m\n");
        total_cost = total_cost * 0.9; // Apply 10% discount
        current_passenger->flight_points -= 50;
    }

    // Display the price with the discount (if applied)
    printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mTotal cost after discount (if applicable):\033[0m \e[1;32m%d\033[0m BDT\n", total_cost);
    printf("\n\t\t\t\t\t\t\t\t\tPress Enter to continue to payment process\n");
    getchar();

    // Payment method selection
    int payment_choice = 1; // Default to bKash
    while (1)
    {
        system("cls");
        printf("\n\t\t\t\t\t\t\t\t\t\t\033[1;31mChoose Payment Method\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\t\033[1;31m====================================\033[0m\n\n");

        if (payment_choice == 1)
        {
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> 1. bKash\033[0m\n\n");
            printf("\t\t\t\t\t\t\t\t\t  2. Nagad\n\n");
            printf("\t\t\t\t\t\t\t\t\t  3. Rocket\n\n");
        }
        else if (payment_choice == 2)
        {
            printf("\t\t\t\t\t\t\t\t\t  1. bKash\n\n");
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> 2. Nagad\033[0m\n\n");
            printf("\t\t\t\t\t\t\t\t\t  3. Rocket\n\n");
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t  1. bKash\n\n");
            printf("\t\t\t\t\t\t\t\t\t  2. Nagad\n\n");
            printf("\t\t\t\t\t\t\t\t\t\033[1;32m> 3. Rocket\033[0m\n\n");
        }

        key = _getch();
        if (key == 13)
            break; // Enter key to confirm
        if (key == 72)
            payment_choice = (payment_choice == 1) ? 3 : payment_choice - 1; // Up arrow
        if (key == 80)
            payment_choice = (payment_choice == 3) ? 1 : payment_choice + 1; // Down arrow
    }

    // Call the payment process based on the user's choice
    if (payment_choice == 1)
    {
        system("cls");
        printf("\n\t\t\t\t\t\t\t\t\t\t \033[1;31mPayment Process\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\t\033[1;31m==================================\033[0m\n\n");

        if (!payment_process_bkash(class_choice == 1 ? "Economy" : "Business", total_cost))
        {
            return;
        }
    }
    else if (payment_choice == 2)
    {
        system("cls");
        printf("\n\t\t\t\t\t\t\t\t\t\t \033[1;31mPayment Process\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\t\033[1;31m==================================\033[0m\n\n");

        if (!payment_process_nagad(class_choice == 1 ? "Economy" : "Business", total_cost))
        {
            return;
        }
    }
    else if (payment_choice == 3)
    {
        system("cls");

        printf("\n\t\t\t\t\t\t\t\t\t\t \033[1;31mPayment Process\033[0m\n");
        printf("\t\t\t\t\t\t\t\t\t\033[1;31m==================================\033[0m\n\n");

        if (!payment_process_rocket(class_choice == 1 ? "Economy" : "Business", total_cost))
        {
            return;
        }
    }

    // Proceed with the rest of the booking process
    selected_flight->available_seats -= tickets;
    int points = (class_choice == 1) ? 5 * tickets : 10 * tickets;
    current_passenger->flight_points += points;

    // Create the booking
    char *class_type = (class_choice == 1) ? "Economy" : "Business";
    add_booking(current_passenger, selected_flight->flight_name, source, destination,
                selected_flight->date, selected_flight->time, tickets, class_type, selected_flight->flight_number);

    // Booking success message
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mBooking Successful!\033[0m\n");
    printf("\t\t\t\t\t\t\t\t\t\033[1;32mYou booked %d tickets in %s class on %s flight.\033[0m\n",
           tickets, class_type, selected_flight->flight_name);

    printf("\t\t\t\t\t\t\t\t\t\033[1;32mYou earned %d flight points!\033[0m\n", points);
    printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu\n");
    getchar();
}

void reschedule_booking(passenger *current_passenger)
{
    if (!current_passenger->bookings)
    {
        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mNo bookings found to reschedule.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
        getchar();
        return;
    }

    view_recent_bookings(current_passenger);

    // printf("\n\t\t\t\t\t\t\t\tReschedule your Flight\n");

    printf("\n\t\t\t\t\t\t\t\t\e[1;34mEnter the flight number to reschedule:\033[0m ");
    int flight_number;
    scanf("%d", &flight_number);
    getchar();

    Booking *current = current_passenger->bookings;
    // flight *tmp = flight_head;
    while (current != NULL)
    {
        if (current->flight_number == flight_number)
        {
            system("cls");
            printf("\n\t\t\t\t\t\t\t\t\033[1;31mRescheduling Booking\033[0m\n");
            printf("\n\t\t\t\t\t\t\t\t\e[1;34mEnter New Date (dd/mm/yyyy):\033[0m ");
            fgets(current->date, sizeof(current->date), stdin);
            current->date[strcspn(current->date, "\n")] = '\0';

            printf("\t\t\t\t\t\t\t\t\e[1;34mEnter New Time (hh:mm):\033[0m ");
            fgets(current->time, sizeof(current->time), stdin);
            current->time[strcspn(current->time, "\n")] = '\0';

            printf("\n\t\t\t\t\t\t\t\t\033[1;32mBooking rescheduled successfully!\033[0m\n");
            printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
            getchar();
            return;
        }
        current = current->next;
        // tmp = tmp->next;
    }

    printf("\n\t\t\t\t\t\t\t\t\033[1;31mBooking not found.\033[0m\n");
    printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
    getchar();
}

void cancel_booking(passenger *current_passenger)
{
    if (!current_passenger->bookings)
    {
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mNo bookings found to cancel.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
        getchar();
        return;
    }

    view_recent_bookings(current_passenger);
    printf("\n\t\t\t\t\t\t\t\t\e[1;34mEnter the flight number to cancel:\033[0m ");
    int flight_number;
    scanf("%d", &flight_number);
    getchar();

    system("cls");

    printf("\n\t\t\t\t\t\t\t\t\e[1;31mCancel your booking\033[0m\n");

    Booking *current = current_passenger->bookings;
    // flight *tmp = flight_head;
    Booking *prev = NULL;
    while (current != NULL)
    {
        if (current->flight_number == flight_number)
        {
            if (prev == NULL)
                current_passenger->bookings = current->next;
            else
                prev->next = current->next;

            free(current);
            printf("\n\t\t\t\t\t\t\t\t\033[1;32mBooking cancelled successfully!\033[0m\n");
            printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
            getchar();
            return;
        }
        prev = current;
        current = current->next;
        // tmp = tmp->next;
    }

    printf("\n\t\t\t\t\t\t\t\t\033[1;31mBooking not found.\033[0m\n");
    printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
    getchar();
}

passenger *add_passenger(passenger *head, char *username, char *password, char *email, char *contact_info, char *address)
{
    passenger *newPassenger = create_passenger(username, password, email, contact_info, address);

    if (head == NULL)
    {
        return newPassenger;
    }
    else
    {
        passenger *tmp = head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = newPassenger;
    }
    return head;
}

passenger *initialize_data()
{
    // Load passenger data from file
    FILE *passenger_file = fopen("passengers.txt", "r");
    passenger *head = NULL;

    if (passenger_file == NULL)
    {
        // No passenger data found
        return head;
    }

    char line[300];
    while (fgets(line, sizeof(line), passenger_file))
    {
        char username[30], password[30], email[50], contact_info[15], address[100];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", username, password, email, contact_info, address);

        head = add_passenger(head, username, password, email, contact_info, address);
    }
    fclose(passenger_file);

    // Load booking data from file
    FILE *booking_file = fopen("bookings.txt", "r");
    if (booking_file == NULL)
    {
        // No booking data found
        return head;
    }

    while (fgets(line, sizeof(line), booking_file))
    {
        char username[30], flight_name[50], source[50], destination[50];
        char date[20], time[10], class_type[10];
        int seats_booked, flight_number;

        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d",
               username, flight_name, source, destination, date, time, class_type, &seats_booked, &flight_number);

        passenger *current_passenger = head;

        // Find the matching passenger
        while (current_passenger != NULL)
        {
            if (strcmp(current_passenger->username, username) == 0)
            {
                // Add the booking to the passenger's list
                Booking *new_booking = (Booking *)malloc(sizeof(Booking));
                strcpy(new_booking->flight_name, flight_name);
                strcpy(new_booking->source, source);
                strcpy(new_booking->destination, destination);
                strcpy(new_booking->date, date);
                strcpy(new_booking->time, time);
                new_booking->seats_booked = seats_booked;
                strcpy(new_booking->class_type, class_type);
                new_booking->flight_number = flight_number;

                new_booking->next = current_passenger->bookings;
                current_passenger->bookings = new_booking;
                break;
            }
            current_passenger = current_passenger->next;
        }
    }
    fclose(booking_file);

    return head;
}

void update_passenger(passenger *current)
{
    printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Yout Information\033[0m\n");
    printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

    int choice = 1; // Start at the first menu option
    int key;        // Variable to capture key press
    const char *menu[] = {
        "1. Update Password",
        "2. Update Email",
        "3. Update Contact Number",
        "4. Update Address",
        "5. Exit Update Menu"};
    int menuSize = sizeof(menu) / sizeof(menu[0]);

    // Function to draw the menu
    void drawMenu()
    {
        for (int i = 0; i < menuSize; i++)
        {
            if (i + 1 == choice)                                                // Highlight the current option
                printf("\r\t\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n", menu[i]); // Green with arrow
            else
                printf("\t\t\t\t\t\t\t\t\t  %s\n", menu[i]); // Normal color
        }
    }

    // Initial draw
    drawMenu();

    // Menu navigation loop
    while (1)
    {
        key = _getch(); // Get key press

        if (key == 224) // Arrow keys prefix
        {
            key = _getch();                                     // Get the actual arrow key code
            if (key == 72)                                      // Up arrow
                choice = (choice == 1) ? menuSize : choice - 1; // Wrap to last option
            else if (key == 80)                                 // Down arrow
                choice = (choice == menuSize) ? 1 : choice + 1; // Wrap to first option

            // Redraw the menu options
            printf("\033[%dA", menuSize); // Move the cursor up to the start of the menu
            drawMenu();
        }
        else if (key == 13) // Enter key
        {
            system("cls");

            if (choice == 1) // Update Password
            {
                system("cls");
                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Password:\033[0m\n");
                printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                char current_pass[100], new_pass[100], confirm_new_pass[100];

                // Prompt for the current password
                printf("\n\t\t\t\t\t\t\t\t\tEnter current password: ");
                fgets(current_pass, sizeof(current_pass), stdin);
                current_pass[strcspn(current_pass, "\n")] = '\0'; // Remove trailing newline

                if (strcmp(current_pass, current->password) == 0) // Check if the current password matches
                {
                    // Prompt for the new password
                    printf("\n\t\t\t\t\t\t\t\t\tEnter new password: ");
                    fgets(new_pass, sizeof(new_pass), stdin);
                    new_pass[strcspn(new_pass, "\n")] = '\0'; // Remove trailing newline

                    int attempt = 3;

                    while (attempt > 0) // Retry confirmation up to 3 times
                    {
                        printf("\n\t\t\t\t\t\t\t\t\tConfirm new password: ");
                        fgets(confirm_new_pass, sizeof(confirm_new_pass), stdin);
                        confirm_new_pass[strcspn(confirm_new_pass, "\n")] = '\0'; // Remove trailing newline

                        if (strcmp(new_pass, confirm_new_pass) == 0) // If passwords match
                        {
                            strcpy(current->password, new_pass); // Update the password
                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mPassword updated successfully!\033[0m\n");
                            Sleep(1000);
                            return;
                        }
                        else // Passwords do not match
                        {
                            attempt--;
                            if (attempt > 0)
                            {
                                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mError: Passwords do not match. Try again (%d attempts left).\033[0m\n", attempt);
                                Sleep(1000);
                            }
                            else
                            {
                                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mError: Password update failed after 3 attempts.\033[0m\n");
                                Sleep(1000);
                            }
                        }
                    }
                }
                else
                {
                    printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mError: Incorrect current password. Password update failed.\033[0m\n");
                    Sleep(1000);
                    system("cls");
                    return;
                }
            }
            else if (choice == 2) // Update Email
            {
                system("cls");
                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Email:\033[0m\n");
                printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                char new_email[100];
                while (1) // Loop until a valid email is provided
                {
                    printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter new email:\033[0m ");
                    fgets(new_email, sizeof(new_email), stdin);
                    new_email[strcspn(new_email, "\n")] = '\0'; // Remove newline character

                    if (strstr(new_email, "@gmail.com") != NULL) // Check for valid domain
                    {
                        break; // Exit loop if valid email is provided
                    }

                    printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mInvalid email!\033[0m\n");
                }

                // OTP Verification
                srand(time(NULL));              // Seed for randomness
                int otp = rand() % 9000 + 1000; // Generate a random 4-digit number

                printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mLoading\033[0m");
                for (int i = 0; i < 5; i++)
                {
                    printf("\e[1;31m.\033[0m");
                    fflush(stdout);
                    usleep(200000);
                }

                system("cls");

                printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mA 4-digit OTP has been sent to your email: \033[0m \033[1;32m%s\033[0m\n", new_email);

                // Wait for the user to press Enter
                printf("\n\t\t\t\t\t\t\t\t\tPress Enter to continue...");
                getchar(); // Consume the Enter key

                system("cls");

                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Email:\033[0m\n");
                printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                // OTP Verification
                int entered_otp;
                // printf("\n\t\t\t\t\t\t\t\t\tLoading");
                // for (int i = 0; i < 5; i++)
                // {
                //     printf(".");
                //     fflush(stdout);
                //     usleep(200000);
                // }

                printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mYour OTP is: \033[1;32m%d\033[0m\n", otp);
                printf("\n\t\t\t\t\t\t\t\t\tEnter the OTP to verify: ");
                scanf("%d", &entered_otp);

                // Clear input buffer
                while (getchar() != '\n')
                    ;

                if (entered_otp == otp)
                {
                    printf("\n\t\t\t\t\t\t\t\t\t\e[1;32mUpdating details\033[0m");
                    for (int i = 0; i < 5; i++)
                    {
                        printf("\e[1;32m.\033[0m");
                        fflush(stdout);
                        usleep(200000);
                    }
                    strcpy(current->email, new_email);
                    printf("\n\t\t\t\t\t\t\t\t\t\e[1;32mEmail updated successfully!\033[0m\n");
                    printf("\n\t\t\t\t\t\t\t\t\tPress Enter to continue...");
                    getchar();

                    return;
                }
                else
                {
                    printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mOTP verification failed! Please try again.\033[0m\n");
                }
            }
            else if (choice == 3) // Update Contact Number
            {
                printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mEnter new contact number:\033[0m ");
                fgets(current->contact_info, sizeof(current->contact_info), stdin);
                current->contact_info[strcspn(current->contact_info, "\n")] = '\0';
                printf("\n\t\t\t\t\t\t\t\t\t\e[1;32mContact number updated successfully!\033[0m\n");
                Sleep(1000);
                return;
            }
            else if (choice == 4) // Update Address
            {
                printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mPassenger ID:\033[0m \e[1;32m%d\033[0m\n", current->id);
                printf("\t\t\t\t\t\t\t\t\t\e[1;34mUsername:\033[0m \e[1;32m%s\033[0m\n", current->username);
                printf("\t\t\t\t\t\t\t\t\t\e[1;34mPassword:\033[0m \e[1;32m%s\033[0m\n", current->password);
                printf("\t\t\t\t\t\t\t\t\t\e[1;34mEmail:\033[0m \e[1;32m%s\033[0m\n", current->email);
                printf("\t\t\t\t\t\t\t\t\t\e[1;34mContact Number:\033[0m \e[1;32m%s\033[0m\n", current->contact_info);
                printf("\t\t\t\t\t\t\t\t\t\e[1;34mAddress:\033[0m \e[1;32m%s\033[0m\n", current->address);
            }
            else if (choice == 5) // Exit
            {
                break;
            }
        }
    }
}

void view_faq()
{
    system("cls");

    printf("\n\t\t\t\t\t\t\t\t\t\t\033[1;31mFrequently Asked Questions (FAQ)\033[0m\n\n");

    printf("\t\t\e[1;34mQ1: How do I book a flight?\033[0m\n");
    printf("\t\t\033[1;32mA1: You can book a flight by selecting the 'Book Flight' option from the menu. Then, choose your source and destination, select the flight, and confirm your booking.\033[0m\n\n");

    printf("\t\t\e[1;34mQ2: How do I update my profile?\033[0m\n");
    printf("\t\t\033[1;32mA2: To update your profile, select the 'Update Profile' option. You can then update your contact information, address, or password.\033[0m\n\n");

    printf("\t\t\e[1;34mQ3: What if I forget my password?\033[0m\n");
    printf("\t\t\033[1;32mA3: If you forget your password, please contact customer support or use the password reset feature on the website.\033[0m\n\n");

    printf("\t\t\e[1;34mQ4: How do I view my recent bookings?\033[0m\n");
    printf("\t\t\033[1;32mA4: To view your recent bookings, select the 'Recent Bookings' option from the menu. You'll be able to see all your past flight bookings.\033[0m\n\n");

    printf("\t\t\e[1;34mQ5: How are flight points earned?\033[0m\n");
    printf("\t\t\033[1;32mA5: Flight points are earned based on the class you book. You earn 5 points for Economy and 10 points for Business class.\033[0m\n\n");

    printf("\n\t\tPress Enter to return to the menu...");
    getchar(); // Wait for user to return
}

passenger *login(passenger *head, char *username, char *password)
{
    passenger *tmp = head;
    toLowerCase(username);

    printf("\n\t\t\t\t\t\t\t\t\e[1;34mAuthenticating\033[0m");
    for (int i = 0; i < 5; i++)
    {
        printf("\e[1;34m.\033[0m");
        fflush(stdout);
        usleep(200000); // Sleep for 200ms
    }
    printf("\n");

    while (tmp != NULL)
    {
        char tempoUsername[30];
        strcpy(tempoUsername, tmp->username);
        toLowerCase(tempoUsername);

        if (strcmp(tempoUsername, username) == 0)
        {
            // Match found for username
            int attempts = 3; // Allow 3 attempts for password
            while (attempts > 0)
            {
                if (strcmp(tmp->password, password) == 0)
                {
                    // Login successful
                    printf("\n\t\t\t\t\t\t\t\t\e[1;32mLogin successful! Welcome,\033[0m \e[1;34m%s.\033[0m\n", tmp->username);
                    printf("\n\t\t\t\t\t\t\t\tPress Enter key to continue...");
                    getchar();

                    // Proceed to menu
                    while (1)
                    {
                        system("cls");
                        int choice = 1; // Start at the first menu option
                        int key;
                        const char *menu[] = {
                            "1. View Profile",
                            "2. Update Profile",
                            "3. Book Flight",
                            "4. Reschedule Bookings",
                            "5. Cancel Booking",
                            "6. View Recent Booking",
                            "7. FAQ",
                            "8. Logout"};
                        int menuSize = sizeof(menu) / sizeof(menu[0]);

                        void drawMenu()
                        {
                            system("cls");
                            printf("\n\t\t\t\t\t\t\t\t\e[1;31mProfile Overview\033[0m\n");
                            printf("\t\t\t\t\t\t\t\t==========================\n\n");

                            for (int i = 0; i < menuSize; i++)
                            {
                                printf("\r\t\t\t\t\t\t\t\t\033[2K");
                                if (i + 1 == choice)
                                    printf("\r\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n", menu[i]);
                                else
                                    printf("\r\t\t\t\t\t\t\t\t  %s\n", menu[i]);
                            }
                        }

                        drawMenu();

                        while (1)
                        {
                            key = _getch(); // Use _getch() for arrow key detection
                            if (key == 224)
                            {                   // Arrow key detection (Windows specific)
                                key = _getch(); // Get the actual arrow key code
                                if (key == 72)  // Up arrow
                                    choice = (choice == 1) ? menuSize : choice - 1;
                                else if (key == 80) // Down arrow
                                    choice = (choice == menuSize) ? 1 : choice + 1;

                                printf("\033[%dA", menuSize); // Redraw menu
                                drawMenu();
                            }
                            else if (key == 13)
                            { // Enter key
                                if (choice == 1)
                                { // View Profile
                                    system("cls");
                                    printf("\n\t\t\t\t\t\t\t\t\e[1;31m--- Profile Details ---\033[0m\n\n");
                                    printf("\t\t\t\t\t\t\t\t\e[1;34mID:\033[0m \e[1;32m%d\033[0m\n", tmp->id);
                                    printf("\t\t\t\t\t\t\t\t\e[1;34mUsername:\033[0m \e[1;32m%s\033[0m\n", tmp->username);
                                    printf("\t\t\t\t\t\t\t\t\e[1;34mEmail:\033[0m \e[1;32m%s\033[0m\n", tmp->email);
                                    printf("\t\t\t\t\t\t\t\t\e[1;34mContact Info:\033[0m \e[1;32m%s\033[0m\n", tmp->contact_info);
                                    printf("\t\t\t\t\t\t\t\t\e[1;34mAddress:\033[0m \e[1;32m%s\033[0m\n", tmp->address);
                                    printf("\t\t\t\t\t\t\t\t\e[1;34mFlight Points:\033[0m \e[1;32m%d\033[0m\n", tmp->flight_points);

                                    printf("\n\t\t\t\t\t\t\t\t\e[1;34mPress Enter to continue...\033[0m");
                                    getchar(); // Wait for user to continue
                                }
                                else if (choice == 2)
                                { // Update Profile
                                    system("cls");
                                    update_passenger(tmp);
                                }
                                else if (choice == 3)
                                { // Book Flight
                                    system("cls");
                                    book_flight(tmp);
                                }
                                else if (choice == 4)
                                { // Recent Bookings
                                    system("cls");
                                    reschedule_booking(tmp);
                                }
                                else if (choice == 5)
                                { // FAQ
                                    system("cls");
                                    cancel_booking(tmp);
                                }
                                else if (choice == 8)
                                { // Logout
                                    system("cls");
                                    printf("\n\t\t\t\t\t\t\t\t\e[1;31mProfile Overview\033[0m\n");
                                    printf("\t\t\t\t\t\t\t\t==========================\n\n");

                                    printf("\n\t\t\t\t\t\t\t\t\e[1;31mLogging out\033[0m");
                                    for (int i = 0; i < 5; i++)
                                    {
                                        printf(".");
                                        fflush(stdout);
                                        usleep(200000); // Sleep for 200ms
                                    }
                                    printf("\n");
                                    return tmp; // Return logged in passenger
                                }
                                else if (choice == 6)
                                {
                                    system("cls");
                                    view_recent_bookings(tmp);
                                }
                                else if (choice == 7)
                                {
                                    system("cls");
                                    view_faq(tmp);
                                }
                                break; // Exit inner navigation loop after option is selected
                            }
                        }
                    }
                }
                else
                {
                    attempts--;
                    printf("\n\t\t\t\t\t\t\t\t\033[1;31mIncorrect password.\033[0m");

                    if (attempts == 0)
                    {
                        // Prompt for Forgot Password
                        char choice;
                        printf("\n\t\t\t\t\t\t\t\tForgot password? (y/n): ");
                        scanf(" %c", &choice);
                        getchar(); // Clear buffer

                        if (tolower(choice) == 'y')
                        {
                            // Verify phone number
                            char phone[15];
                            printf("\n\t\t\t\t\t\t\t\tEnter your registered phone number: ");
                            fgets(phone, sizeof(phone), stdin);
                            phone[strcspn(phone, "\n")] = '\0';

                            if (strcmp(phone, tmp->contact_info) != 0)
                            {
                                printf("\n\t\t\t\t\t\t\t\t\033[1;31mPhone number does not match our records.\033[0m\n");
                                printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
                                getchar();
                                return NULL; // Exit login
                            }

                            // OTP Verification
                            srand(time(NULL));
                            int otp = rand() % 9000 + 1000; // Generate a 4-digit OTP
                            printf("\n\t\t\t\t\t\t\t\t\033[1;34mAn OTP has been sent to your phone: %s.\033[0m\n", phone);
                            printf("\t\t\t\t\t\t\t\t\033[1;34mYour OTP is: %d (for testing purposes).\033[0m\n", otp);

                            int entered_otp;
                            printf("\n\t\t\t\t\t\t\t\tEnter the OTP: ");
                            scanf("%d", &entered_otp);
                            getchar(); // Clear buffer

                            if (entered_otp != otp)
                            {
                                printf("\n\t\t\t\t\t\t\t\t\033[1;31mOTP verification failed.\033[0m\n");
                                printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
                                getchar();
                                return NULL; // Exit login
                            }

                            // Set a new password
                            char new_password[30], confirm_password[30];
                            printf("\n\t\t\t\t\t\t\t\tEnter your new password: ");
                            fgets(new_password, sizeof(new_password), stdin);
                            new_password[strcspn(new_password, "\n")] = '\0';

                            printf("\t\t\t\t\t\t\t\tConfirm your new password: ");
                            fgets(confirm_password, sizeof(confirm_password), stdin);
                            confirm_password[strcspn(confirm_password, "\n")] = '\0';

                            if (strcmp(new_password, confirm_password) != 0)
                            {
                                printf("\n\t\t\t\t\t\t\t\t\033[1;31mPasswords do not match. Password reset failed.\033[0m\n");
                                printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the menu...\n");
                                getchar();
                                return NULL; // Exit login
                            }

                            // Update password
                            strcpy(tmp->password, new_password);
                            printf("\n\t\t\t\t\t\t\t\t\033[1;32mPassword reset successful! Please log in again.\033[0m\n");
                            printf("\n\t\t\t\t\t\t\t\tPress Enter to return to the login menu...\n");
                            getchar();
                            return NULL; // Exit login for re-login
                        }
                        else
                        {
                            printf("\n\t\t\t\t\t\t\t\tReturning to login menu.\n");
                            return NULL; // Exit login
                        }
                    }
                    else
                    {
                        // Prompt to re-enter password
                        printf("\n\t\t\t\t\t\t\t\tRe-enter password: ");
                        fgets(password, 30, stdin);
                        password[strcspn(password, "\n")] = '\0';
                    }
                }
            }
        }
        tmp = tmp->next; // Move to the next passenger in the list
    }

    printf("\n\t\t\t\t\t\t\t\t\033[1;31mInvalid credentials.\033[0m\n");
    printf("\n\t\t\t\t\t\t\t\tPress Enter to return to menu...\n");
    getchar();
    return NULL; // Return NULL if login fails
}

void animateLoading(const char *message, int delay_ms, int dots)
{
    printf("\t\t\t\t\t\t\t\t%s", message);
    for (int i = 0; i < dots; i++)
    {
        printf(".");
        fflush(stdout); // Ensure the dot is printed immediately
        usleep(200000); // Convert milliseconds to microseconds
    }
    printf("\n\n");
}

void show_details(passenger *head)
{

    printf("\n\n\t\t\t\t\t\t\t\t\t   \033[1;31mPassenger Details:\033[0m\n");

    printf("\t\t\t\t\t\t\t\t\e[1;31m==========================================\033[0m\n\n");

    // Animated Loading
    animateLoading("Loading database", 300, 3); // 300ms delay, 3 dots

    system("cls");

    if (head == NULL)
    {
        printf("\n\t\t\t\t\t\t\t\t\t   \033[1;31mPassenger Details:\033[0m\n");

        printf("\t\t\t\t\t\t\t\t\e[1;31m==========================================\033[0m\n\n");
        printf("\n\t\t\t\t\t\t\t\t\033[1;31mNo passenger records found.\033[0m\n");

        return;
    }

    printf("\n\t\t\t\t\t\t\t\t\t   \033[1;31mPassenger Details:\033[0m\n");

    printf("\t\t\t\t\t\t\t\t\e[1;31m==========================================\033[0m\n\n");

    passenger *current = head;

    while (current != NULL)
    {
        printf("\t\t\t\t\t\t\t\t\e[1;34mID:\033[0m \e[1;32m%d\033[0m\n", current->id);

        // Check for empty fields to avoid printing garbage data
        if (current->username && strlen(current->username) > 0)
            printf("\t\t\t\t\t\t\t\t\e[1;34mUsername:\033[0m \e[1;32m%s\e[0m\n", current->username);
        else
            printf("\t\t\t\t\t\t\t\t\e[1;34mUsername:\033[0m \e[1;32mN/A\033[0m\n");

        if (current->email && strlen(current->email) > 0)
            printf("\t\t\t\t\t\t\t\t\e[1;34mEmail:\033[0m \e[1;32m%s\033[0m\n", current->email);
        else
            printf("\t\t\t\t\t\t\t\t\e[1;34mEmail:\033[0m \e[1;32mN/A\033[0m\n");

        if (current->contact_info && strlen(current->contact_info) > 0)
            printf("\t\t\t\t\t\t\t\t\e[1;34mContact Info:\033[0m \e[1;32m%s\033[0m\n", current->contact_info);
        else
            printf("\t\t\t\t\t\t\t\t\e[1;34mContact Info:\033[0m \e[1;32mN/A\033[0m\n");

        if (current->address && strlen(current->address) > 0)
            printf("\t\t\t\t\t\t\t\t\e[1;34mAddress:\033[0m \e[1;32m%s\033[0m\n", current->address);
        else
            printf("\t\t\t\t\t\t\t\t\e[1;34mAddress:\033[0m \e[1;32mN/A\033[0m\n");

        printf("\t\t\t\t\t\t\t\t\e[1;31m----------------------------------------------------\033[0m\n\n");

        current = current->next;
    }
}

void searchPassenger(passenger *head)
{

    printf("\n\n\t\t\t\t\t\t\t\t\e[1;31m      --- Passenger Database ---\033[0m\n");
    printf("\t\t\t\t\t\t\t\t\e[1;31m ====================================\033[0m\n\n");

    int search_id;
    printf("\t\t\t\t\t\t\t\t \e[1;34m Enter passenger ID to search:\033[0m ");
    scanf("%d", &search_id);
    getchar();

    passenger *current = head;
    int found = 0;

    printf("\n\t\t\t\t\t\t\t\t  \e[1;31mLoading\033[0m");

    for (int i = 0; i < 5; i++)
    {
        printf(".");
        fflush(stdout);
        usleep(200000);
    }
    printf("\n");

    system("cls");

    printf("\n\n\t\t\t\t\t\t\t\t\t   \033[1;31mPassenger Details:\033[0m\n");

    printf("\t\t\t\t\t\t\t\t\e[1;31m==========================================\033[0m\n\n");

    // Traverse the linked list and search for the passenger by ID
    while (current != NULL)
    {
        if (current->id == search_id)
        {
            found = 1;
            // Print the passenger details if found

            printf("\n\t\t\t\t\t\t\t\t\e[1;34mPassenger ID:\033[0m \e[1;32m%d\033[0m\n", current->id);
            printf("\t\t\t\t\t\t\t\t\e[1;34mUsername:\033[0m \e[1;32m%s\033[0m\n", current->username);
            printf("\t\t\t\t\t\t\t\t\e[1;34mEmail:\033[0m \e[1;32m%s\033[0m\n", current->email);
            printf("\t\t\t\t\t\t\t\t\e[1;34mContact Info:\033[0m \e[1;32m%s\033[0m\n", current->contact_info);
            printf("\t\t\t\t\t\t\t\t\e[1;34mAddress:\033[0m \e[1;32m%s\033[0m\n", current->address);
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("\nt\t\t\t\t\t\t\t \e[1;31mNo passenger found with ID\033[0m \e[1;32m%d.\033[0m\n", search_id);
    }
}

void update_passenger_info(passenger *head)
{

    int search_id;
    printf("\n\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Passenger Information:\033[0m\n");
    printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

    printf("\t\t\t\t\t\t\t\t\tEnter passenger ID to update: ");
    scanf("%d", &search_id);
    getchar(); // Clear input buffer

    passenger *current = head;
    int found = 0;

    printf("\n\t\t\t\t\t\t\t\t\tLoading");
    for (int i = 0; i < 5; i++)
    {
        printf(".");
        fflush(stdout);
        usleep(200000); // Delay for loading effect
    }

    while (current != NULL)
    {
        if (current->id == search_id)
        {
            found = 1;

            while (1) // Loop to handle returning to the menu
            {
                system("cls");
                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdating details for passenger ID: %d\033[0m\n", current->id);
                printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                int choice = 1; // Start at the first menu option
                int key;        // Variable to capture key press
                const char *menu[] = {
                    "1. Update Password",
                    "2. Update Email",
                    "3. Update Contact Number",
                    "4. View Current Details",
                    "5. Exit Update Menu"};
                int menuSize = sizeof(menu) / sizeof(menu[0]);

                // Function to draw the menu
                void drawMenu()
                {
                    for (int i = 0; i < menuSize; i++)
                    {
                        if (i + 1 == choice)                                                // Highlight the current option
                            printf("\r\t\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n", menu[i]); // Green with arrow
                        else
                            printf("\t\t\t\t\t\t\t\t\t  %s\n", menu[i]); // Normal color
                    }
                }

                // Initial draw
                drawMenu();

                // Menu navigation loop
                while (1)
                {
                    key = _getch(); // Get key press

                    if (key == 224) // Arrow keys prefix
                    {
                        key = _getch();                                     // Get the actual arrow key code
                        if (key == 72)                                      // Up arrow
                            choice = (choice == 1) ? menuSize : choice - 1; // Wrap to last option
                        else if (key == 80)                                 // Down arrow
                            choice = (choice == menuSize) ? 1 : choice + 1; // Wrap to first option

                        // Redraw the menu options
                        printf("\033[%dA", menuSize); // Move the cursor up to the start of the menu
                        drawMenu();
                    }
                    else if (key == 13) // Enter key
                    {
                        system("cls");

                        if (choice == 1) // Update Password
                        {
                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Password:\033[0m\n");
                            printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                            char current_pass[100], new_pass[100], confirm_new_pass[100];

                            // Prompt for the current password
                            printf("\n\t\t\t\t\t\t\t\t\tEnter current password: ");
                            fgets(current_pass, sizeof(current_pass), stdin);
                            current_pass[strcspn(current_pass, "\n")] = '\0'; // Remove trailing newline

                            if (strcmp(current_pass, current->password) == 0) // Check if the current password matches
                            {
                                // Prompt for the new password
                                printf("\n\t\t\t\t\t\t\t\t\tEnter new password: ");
                                fgets(new_pass, sizeof(new_pass), stdin);
                                new_pass[strcspn(new_pass, "\n")] = '\0'; // Remove trailing newline

                                int attempt = 3;

                                while (attempt > 0) // Retry confirmation up to 3 times
                                {
                                    printf("\n\t\t\t\t\t\t\t\t\tConfirm new password: ");
                                    fgets(confirm_new_pass, sizeof(confirm_new_pass), stdin);
                                    confirm_new_pass[strcspn(confirm_new_pass, "\n")] = '\0'; // Remove trailing newline

                                    if (strcmp(new_pass, confirm_new_pass) == 0) // If passwords match
                                    {
                                        strcpy(current->password, new_pass); // Update the password
                                        printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mPassword updated successfully!\033[0m\n");
                                        break;
                                    }
                                    else // Passwords do not match
                                    {
                                        attempt--;
                                        if (attempt > 0)
                                        {
                                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mError: Passwords do not match. Try again (%d attempts left).\033[0m\n", attempt);
                                        }
                                        else
                                        {
                                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mError: Password update failed after 3 attempts.\033[0m\n");
                                        }
                                    }
                                }
                            }
                            else
                            {
                                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mError: Incorrect current password. Password update failed.\033[0m\n");
                            }
                        }
                        else if (choice == 2) // Email
                        {
                            system("cls");
                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Email:\033[0m\n");
                            printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                            char new_email[100];
                            while (1) // Loop until a valid email is provided
                            {
                                printf("\n\t\t\t\t\t\t\t\t\tEnter new email: ");
                                fgets(new_email, sizeof(new_email), stdin);
                                new_email[strcspn(new_email, "\n")] = '\0';

                                if (strstr(new_email, "@gmail.com") != NULL) // Check for validation
                                {
                                    break; // Exit loop if valid email is provided
                                }

                                printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mInvalid email! Must include '@gmail.com'.\033[0m\n");
                            }

                            // Generate OTP
                            srand(time(NULL));              // Seed for randomness
                            int otp = rand() % 9000 + 1000; // Generate a random 4-digit number

                            printf("\n\t\t\t\t\t\t\t\t\tLoading");
                            for (int i = 0; i < 5; i++)
                            {
                                printf(".");
                                fflush(stdout);
                                usleep(200000);
                            }

                            system("cls");

                            printf("\n\t\t\t\t\t\t\t\t\t\e[1;34mA 4-digit OTP has been sent to your email: \033[0m \033[1;32m%s\033[0m\n", new_email);

                            // Wait for the user to press Enter
                            printf("\n\t\t\t\t\t\t\t\t\tPress Enter to continue...");
                            getchar(); // Consume the Enter key

                            system("cls");

                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mUpdate Email:\033[0m\n");
                            printf("\t\t\t\t\t\t\t\t\t==========================================\n\n");

                            // OTP Verification
                            int entered_otp;
                            // printf("\n\t\t\t\t\t\t\t\t\tLoading");
                            // for (int i = 0; i < 5; i++)
                            // {
                            //     printf(".");
                            //     fflush(stdout);
                            //     usleep(200000);
                            // }

                            printf("\n\t\t\t\t\t\t\t\t\tYour OTP is: \033[1;32m%d\033[0m\n", otp);
                            printf("\n\t\t\t\t\t\t\t\t\tEnter the OTP to verify: ");
                            scanf("%d", &entered_otp);

                            // Clear input buffer
                            while (getchar() != '\n')
                                ;

                            if (entered_otp == otp)
                            {
                                printf("\n\t\t\t\t\t\t\t\t\t\e[1;32mUpdating details\033[0m");
                                for (int i = 0; i < 5; i++)
                                {
                                    printf(".");
                                    fflush(stdout);
                                    usleep(200000);
                                }
                                strcpy(current->email, new_email);
                                printf("\n\t\t\t\t\t\t\t\t\t\e[1;32mEmail updated successfully!\033[0m\n");
                            }
                            else
                            {
                                printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mIncorrect OTP. Email update failed.\033[0m\n");
                            }

                            printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
                            getchar(); // Consume the Enter key before returning to the menu
                            break;
                        }

                        else if (choice == 3) // Update Contact Number
                        {
                            printf("\n\t\t\t\t\t\t\t\t\tEnter new contact number: ");
                            fgets(current->contact_info, sizeof(current->contact_info), stdin);
                            current->contact_info[strcspn(current->contact_info, "\n")] = '\0';
                            printf("\n\t\t\t\t\t\t\t\t\t\033[1;32mContact number updated successfully!\033[0m\n");
                        }
                        else if (choice == 4) // View Current Details
                        {
                            printf("\n\t\t\t\t\t\t\t\t\tPassenger ID: %d\n", current->id);
                            printf("\t\t\t\t\t\t\t\t\tUsername: %s\n", current->username);
                            printf("\t\t\t\t\t\t\t\t\tPassword: %s\n", current->password);
                            printf("\t\t\t\t\t\t\t\t\tEmail: %s\n", current->email);
                            printf("\t\t\t\t\t\t\t\t\tContact Number: %s\n", current->contact_info);
                            printf("\t\t\t\t\t\t\t\t\tAddress: %s\n", current->address);
                        }
                        else if (choice == 5) // Exit Update Menu
                        {
                            printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mExiting update menu\033[0m");
                            for (int i = 0; i < 5; i++)
                            {
                                printf(".");
                                fflush(stdout);
                                usleep(200000);
                            }
                            return;
                        }
                        // printf("\n\t\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
                        // getchar(); // Pause for the user to see the message
                    }
                }
            }
        }
        current = current->next;
    }

    if (!found)
    {

        printf("\n\t\t\t\t\t\t\t\t\t\033[1;31mPassenger ID not found!\033[0m\n");
        // printf("\n\t\t\t\t\t\t\t\t\tPress Enter to return to the menu...");
        // fflush(stdin);

        // getchar(); // Pause until the user presses Enter to return
        Sleep(2000);
        return;
    }
}

void admin_pannel()
{
    initialize_test_data();
    passenger *head = initialize_data();

    char username[30], password[30];

    printf("\n\n\t\t\t\t\t\t\t\t\t \e[1;31mAdmin Login\033[0m\n");
    printf("\t\t\t\t\t\t\t\t\t ==========================\n\n");

    printf("\t\t\t\t\t\t\t\t\t \e[1;34mEnter Admin Username:\033[0m ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\t\t\t\t\t\t\t\t\t \e[1;34mEnter Admin Password:\033[0m ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0)
    {
        printf("\n\t\t\t\t\t\t\t\t\t \e[1;34mAuthenticating admin credentials\033[0m");
        for (int i = 0; i < 5; i++)
        {
            printf(".");
            fflush(stdout);
            usleep(200000);
        }
        printf("\n");
        printf("\n\t\t\t\t\t\t\t\t\t \e[1;32mAdmin login successful! Access granted.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\t Press Enter to continue...");
        getchar();

        while (1)
        {
            int choice = 1; // Start at the first menu option
            int key;        // Variable to capture key press
            const char *menu[] = {
                "1. View Passenger Details",
                "2. Search Passenger",
                "3. Update Passenger Information",
                "4. Flight Management",
                "5. Logout"};
            int menuSize = sizeof(menu) / sizeof(menu[0]);

            // Function to draw the menu dynamically
            void drawMenu()
            {
                // Menu header
                printf("\n\n\t\t\t\t\t\t\t\t\t\e[1;31mAdmin Panel\033[0m\n");
                printf("\t\t\t\t\t\t\t\t\t==========================\n\n");

                // Loop through the menu items and highlight the current choice
                for (int i = 0; i < menuSize; i++)
                {
                    if (i + 1 == choice)                                                // Highlight the current option
                        printf("\r\t\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n", menu[i]); // Green with arrow
                    else
                        printf("\t\t\t\t\t\t\t\t\t  %s\n", menu[i]); // Normal color
                }
            }

            // Initial draw
            system("cls"); // Clear the screen once at the beginning
            drawMenu();    // Draw the initial menu

            // Menu navigation loop
            while (1)
            {
                key = _getch(); // Get key press

                if (key == 224) // Arrow keys prefix
                {
                    key = _getch();                                     // Get the actual arrow key code
                    if (key == 72)                                      // Up arrow
                        choice = (choice == 1) ? menuSize : choice - 1; // Wrap to last option
                    else if (key == 80)                                 // Down arrow
                        choice = (choice == menuSize) ? 1 : choice + 1; // Wrap to first option

                    // Clear screen and redraw menu options
                    system("cls");
                    drawMenu();
                }
                else if (key == 13) // Enter key
                {
                    system("cls");
                    if (choice == 1) // View Passenger Details
                    {
                        show_details(head);
                        printf("\n\t\t\t\t\t\t\t\tPress Enter key to continue...");
                        getchar();
                    }
                    else if (choice == 2) // Search Passenger
                    {
                        searchPassenger(head);
                        printf("\n\t\t\t\t\t\t\t\tPress Enter key to continue...");
                        getchar();
                    }
                    else if (choice == 3) // Update Passenger Information
                    {
                        update_passenger_info(head);
                    }
                    else if (choice == 4) // Flight Management
                    {
                        flight_management_menu();
                    }
                    else if (choice == 5) // Logout
                    {
                        printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mAdmin Panel\033[0m\n");
                        printf("\t\t\t\t\t\t\t\t\t==========================\n\n");

                        printf("\n\t\t\t\t\t\t\t\t\t\e[1;31mLogging out\033[0m");
                        for (int i = 0; i < 5; i++)
                        {
                            printf(".");
                            fflush(stdout);
                            usleep(200000);
                        }
                        printf("\n");
                        return; // Exit the admin panel function
                    }
                    break; // Exit inner navigation loop after handling the option
                }
            }
        }
    }
    else
    {
        printf("\n\t\t\t\t\t\t\t\t\t \e[1;34mAuthenticating admin credentials\033[0m");
        for (int i = 0; i < 5; i++)
        {
            printf("\e[1;34m.\033[0m");
            fflush(stdout);
            usleep(200000);
        }
        printf("\n\t\t\t\t\t\t\t\t\t \e[1;31mInvalid admin credentials. Access denied.\033[0m\n");
    }

    Sleep(1000);
}

passenger *register_passenger(passenger *head)
{
    char username[30], password[30], confirm_password[30], email[50], contact_info[15], address[100];

    printf("\t\t\t\t\t\t\t\t      --- \e[1;31mRegister a new account\033[0m ---\n");
    printf("\t\t\t\t\t\t\t\t\e[1;31m========================================\033[0m\n\n");

    int uniqueUsername = 0; // to check if username is unique
    do
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mUsername: \033[0m");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';
        toLowerCase(username);

        // Check for duplicate username
        passenger *tmp = head;
        uniqueUsername = 1; // Assume it's unique
        while (tmp != NULL)
        {
            if (strcmp(tmp->username, username) == 0)
            {
                printf("\n\t\t\t\t\t\t\t\t\e[1;31mError! Username already exists. Please choose a different username.\033[0m\n");
                uniqueUsername = 0; // Found a duplicate
                break;
            }
            tmp = tmp->next;
        }
    } while (!uniqueUsername); // Repeat

    // Password and confirm password check
    while (1)
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mPassword: \033[0m");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';

        if (strlen(password) == 0)
        {
            printf("\n\t\t\t\t\t\t\t\t\e[1;31mPassword cannot be empty. Please enter a valid password.\033[0m\n");
            continue;
        }

        printf("\n\t\t\t\t\t\t\t\t\e[1;34mConfirm Password: \033[0m");
        fgets(confirm_password, sizeof(confirm_password), stdin);
        confirm_password[strcspn(confirm_password, "\n")] = '\0';

        if (strcmp(password, confirm_password) == 0)
        {
            break;
        }
        else
        {
            printf("\n\t\t\t\t\t\t\t\t\e[1;31mPasswords do not match. Please try again.\033[0m\n\n");
        }
    }

    // Email checked
    do
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mEmail: \033[0m");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';
        if (strlen(email) == 0)
        {
            printf("\n\t\t\t\t\t\t\t\t\e[1;31mEmail cannot be empty. Please enter a valid email.\033[0m\n");
        }
        else if (strstr(email, "@gmail.com") == NULL || strcmp(email + strlen(email) - 10, "@gmail.com") != 0)
        {
            printf("\n\t\t\t\t\t\t\t\t\e[1;31mInvalid Email address. Please try again.\033[0m\n");
        }
        else
        {
            break; // valid
        }

    } while (1);

    // Contact info checked with non-empty validation
    do
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mContact Info: \033[0m");
        fgets(contact_info, sizeof(contact_info), stdin);
        contact_info[strcspn(contact_info, "\n")] = '\0';
        if (strlen(contact_info) == 0)
        {
            printf("\n\t\t\t\t\t\t\t\t\e[1;31mContact info cannot be empty. Please enter valid contact info.\033[0m\n");
        }
    } while (strlen(contact_info) == 0);

    // Address check
    do
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mAddress: \033[0m");
        fgets(address, sizeof(address), stdin);
        address[strcspn(address, "\n")] = '\0';
        if (strlen(address) == 0)
        {
            printf("\n\t\t\t\t\t\t\t\t\e[1;31mAddress cannot be empty. Please enter a valid address.\033[0m\n");
        }
    } while (strlen(address) == 0);

    // OTP
    srand(time(0));
    int otp, enteredOtp;

    otp = rand() % 9000 + 1000;
    printf("\n\t\t\t\t\t\t\t\t\e[1;34mYour OTP for registration is:\033[0m \e[1;32m%d\033[0m\n", otp);

    // verification
    printf("\t\t\t\t\t\t\t\t\e[1;34mEnter the OTP to complete registration: \033[0m");
    scanf("%d", &enteredOtp);
    getchar(); // Clear any leftover input (like Enter key)

    if (enteredOtp != otp)
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;31mIncorrect OTP. Registration failed.\033[0m\n");
        printf("\n\t\t\t\t\t\t\t\t\e[1;34mPress Enter key to continue...\033[0m");
        getchar();
        return head;
    }

    head = add_passenger(head, username, password, email, contact_info, address);
    FILE *file = fopen("passengers.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s,%s,%s,%s,%s\n", username, password, email, contact_info, address);
        fclose(file);
    }
    else
    {
        printf("\n\t\t\t\t\t\t\t\t\e[1;31mError saving passenger data. Try again later.\033[0m\n");
    }

    printf("\n\t\t\t\t\t\t\t\t\e[1;32mRegistration successful! You can now log in.\033[0m\n");

    printf("\n\t\t\t\t\t\t\t\t\e[1;34mPress Enter key to continue...\033[0m");
    getchar(); // Clear input buffer after registration success

    return head;
}

void printAnimatedText(char *text, int delay)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        putchar(text[i]);
        fflush(stdout);
        Sleep(delay / 2000); // Delay
    }
}

void printMenu()
{
    printf("\t\t\t\t\t\t\t\t----------------------------------------------------\n");
    printf("\t\t\t\t\t\t\t\t-                                                  -\n");
    printf("\t\t\t\t\t\t\t\t-                ");
    printAnimatedText("Welcome to FlightEase", 40000); // 40 ms delay per character
    printf("             -\n");
    printf("\t\t\t\t\t\t\t\t-             ");
    printAnimatedText("Your Journey, Our Priority!", 40); // 40 ms delay per character
    printf("          -\n");
    printf("\t\t\t\t\t\t\t\t-                                                  -\n");
    printf("\t\t\t\t\t\t\t\t----------------------------------------------------\n");

    printf("\n");
    printf("\t\t\t\t\t\t\t\t\tCopyright © 2024 Binary Bandits. All Rights Reserved\n\n");

    printf("\t\t\t\t\t\t\t\t\t\t      --- \e[1;31mMain Menu\033[0m ---\n");
    printf("\t\t\t\t\t\t\t\t\t\t=============================\n\n");

    printf("\t\t\t\t\t\t\t\t1. Register\n");
    printf("\t\t\t\t\t\t\t\t2. Login\n");
    printf("\t\t\t\t\t\t\t\t3. Admin Panel (Authenticated personnel only)\n");
    printf("\t\t\t\t\t\t\t\t4. Show Passenger Details\n");
    printf("\t\t\t\t\t\t\t\t5. Exit\n");
}

void printSelectedMenu(int currentOption)
{
    // Print the static menu with the selected option highlighted
    for (int i = 1; i <= 5; i++)
    {
        if (i == currentOption)
        {
            printf("\t\t\t\t\t\t\t\t>> ");
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t   ");
        }

        switch (i)
        {
        case 1:
            printf("1. Register\n");
            break;
        case 2:
            printf("2. Login\n");
            break;
        case 3:
            printf("3. Admin Panel (Authenticated personnel only)\n");
            break;
        case 4:
            printf("4. Show Passenger Details\n");
            break;
        case 5:
            printf("5. Exit\n");
            break;
        }
    }
}

int displayMenu()
{
    int choice = 1; // Start at the first menu option
    int key;        // Variable to capture key press

    const char *menu[] = {
        "1. Register",
        "2. Login",
        "3. Admin panel (Authenticated personnel only)",
        "4. Exit"};
    int menuSize = sizeof(menu) / sizeof(menu[0]);

    // Clear the screen
    system("cls");

    // Display the menu header
    printf("\e[1;31m"); // ANSI code for red color

    // Top border of dashes
    printf("\t\t\t\t\t\t\t----------------------------------------------------\n");
    printf("\t\t\t\t\t\t\t\e[1;31m-                                                  -\033[0m\n");

    // Reset color for text
    printf("\033[0m");

    // Animated Welcome Message
    printf("\t\t\t\t\t\t\t\e[1;31m-                ");
    printAnimatedText("\e[1;34mWelcome to FlightEase\033[0m", 40000); // 40 ms delay per character
    printf("\e[1;31m             -\033[0m\n");
    printf("\t\t\t\t\t\t\t\e[1;31m-             \033[0m");
    printAnimatedText("\e[1;35mYour Journey, Our Priority!\033[0m", 40000); // 40 ms delay per character
    printf("\e[1;31m          -\033[0m\n");

    // Red color for border again
    printf("\033[1;31m"); // ANSI code for red color
    printf("\t\t\t\t\t\t\t-                                                  -\n");
    printf("\t\t\t\t\t\t\t----------------------------------------------------\n");

    // Reset color for text
    printf("\033[0m");

    // Menu options with red border
    printf("\n");
    printAnimatedText("\n\t\t\t\t\t\t\t \033[1;34mCopyright 2024 Binary Bandits. All Rights Reserved\033[0m", 40000);
    printf("\n\n\t\t\t\t\t\t\t\t\t   --- \e[1;96mMain Menu\033[0m ---\n\n\n");

    // Function to draw the menu options with gaps
    void drawMenu()
    {
        for (int i = 0; i < menuSize; i++)
        {
            if (i + 1 == choice)                                                // Highlight the current option
                printf("\r\t\t\t\t\t\t\t\t\033[1;32m> %s\033[0m\n\n", menu[i]); // Green with arrow
            else
                printf("\t\t\t\t\t\t\t\t  %s\n\n", menu[i]); // Normal color
        }
    }

    // Initial draw
    drawMenu();

    // Menu navigation loop
    while (1)
    {
        key = _getch(); // Get key press

        if (key == 224) // Arrow keys prefix
        {
            key = _getch();                                     // Get the actual arrow key code
            if (key == 72)                                      // Up arrow
                choice = (choice == 1) ? menuSize : choice - 1; // Wrap to the last option
            else if (key == 80)                                 // Down arrow
                choice = (choice == menuSize) ? 1 : choice + 1; // Wrap to the first option

            // Redraw the menu options
            printf("\033[%dA", menuSize * 2); // Move the cursor up to account for the extra gaps
            drawMenu();
        }
        else if (key == 13) // Enter key
        {
            fflush(stdin); // Clear any remaining input buffer
            return choice; // Return the selected option
        }
    }
}

int main()
{
    // passenger *head = NULL;
    // passenger *currentPassenger = NULL;

    initialize_test_data();
    passenger *head = initialize_data();

    save_bookings_to_file(head);
    // load_bookings_from_file(head);

    // load_passenger_data();
    // save_passenger_data();

    while (1)
    {
        system("cls");
        int choice = displayMenu();
        // getchar();

        if (choice == 1)
        {
            system("cls");
            head = register_passenger(head);

            // printf("\n\t\t\t\t\t\t\t\tPress Enter key to continue...");
            // getchar();
        }
        else if (choice == 2)
        {
            system("cls");

            printf("\t\t\t\t\t\t\t\t\e[1;31m      --- Login to your account\033[0m ---\n");
            printf("\t\t\t\t\t\t\t\t========================================\n\n");

            char username[30], password[30];
            printf("\t\t\t\t\t\t\t\t\e[1;34mEnter Username:\033[0m ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';
            printf("\n\t\t\t\t\t\t\t\t\e[1;34mEnter Password:\033[0m ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

            currentPassenger = login(head, username, password);
            // if (currentPassenger == NULL)
            // {
            //     printf("\n\t\t\t\t\t\t\t\tInvalid username or password. Please try again.\n");
            // }

            // printf("\n\t\t\t\t\t\t\t\tPress Enter key to continue...");
            // getchar();
        }

        else if (choice == 3)
        {
            system("cls");

            admin_pannel();

            // printf("\n\t\t\t\t\t\t\t\tPress Enter key to continue...");
            // getchar();
        }

        else if (choice == 4)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t\t\t\t\e[1;31mFlight Ease\033[0m");

            printf("\n\n\n\t\t\t\t\t\t\t\e[1;31m-----      ");

            printAnimatedText("\e[1;31mThank you for using FlightEase! Safe travels!\033[0m", 40000);
            printf("\e[1;31m     -----\033[0m\n");
            break;
        }
        else
        {
            printf("\t\t\t\t\t\t\t\tInvalid choice. Please try again.\n");
        }
    }

    return 0;
}
