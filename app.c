#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EMPLOYEES EMPLOYEE_MAX_ARRAY_SIZE
#define EMPLOYEE_MAX_ARRAY_SIZE 1000
#define EMPLOYEE_MAX_NAME_LENGTH 50
#define EMPLOYEE_MAX_DEPARTMENT_LENGTH 50
#define CHAR_MAX_BUFFER_SIZE 256
#define CHAR_MY_BUFFER_SIZE 100

#pragma region Structures & Typedefs
struct Employee
{
   int id;              
   char name[EMPLOYEE_MAX_NAME_LENGTH];       
   char department[EMPLOYEE_MAX_DEPARTMENT_LENGTH]; 
   float salary;

    /*
      Total: 108 bytes per employee (4 + 50 + 50 + 4 = (108 * 100) = 10800 bytes
      or (10.8 KB) or (0.0108 MB) (100 employees type structures)
   */
};

typedef enum return_status
{
   STATUS_ALLOCATION_FAILED = -1,
   STATUS_INVALID_INPUT = -2,
   STATUS_ARRAY_IS_FULL = -3,
   STATUS_INVALID_INT_RANGE = -4,
   STATUS_BAD = 0,
   STATUS_GOOD = 1,
} status_t;
#pragma endregion

#pragma region Helper Functions
// fscan & fgets do not consume the newline character, so we need to clear the input buffer
void clearInputBuffer()
{
   int c;
   while ((c = getchar()) != '\n' && c != EOF); // Get the next character in the buffer
} //while that character is not a newline or EOF, keep getting the next character

void print_array_size(int *arr, int array_size)
{
   if (arr == NULL)
   {
      printf("Array is NULL\n");
      return;
   }
   printf("Elements in Array: %d\n", array_size);
   return;
}

void print_employee_array_size(struct Employee *arr, int array_size)
{
   if (arr == NULL)
   {
      printf("Structure Array is NULL\n");
      return;
   }
   printf("Elements in Employee Structure Array: %d\n", array_size);
   return;
}

void print_employee_array_size_in_bytes(struct Employee *arr, int array_size)
{
   if (arr == NULL)
   {
      printf("Structure Array is NULL\n");
      return;
   }
   size_t size_in_bytes = array_size * sizeof(struct Employee);
   printf("Size of Employee Structure Array: %zu bytes\n", size_in_bytes);
}

// Doubling method on array increase and direct shrinking on decrease
status_t resize_int_array(int **arr, size_t *current_size, size_t new_size)
{
   if (new_size > *current_size)
   {
      while (*current_size < new_size)
      {
         *current_size *= 1.5; // Times the capacity by 1.5 to cut down on reallocation calls (they cost a-lot)
      }
      int *temp = realloc(*arr, *current_size * sizeof(int)); // Allocate new memory
      if (temp == NULL)
      {
         *arr = NULL;
         return STATUS_BAD; // Memory allocation failed
      }
      *arr = temp;
   }
   else if (new_size < *current_size)
   { // Direct Shrinking
      // Only shrink if the new size is less than half of the current capacity
      if (new_size < *current_size / 2)
      {
         int *temp = realloc(*arr, new_size * sizeof(int));
         if (temp == NULL)
         {
            *arr = NULL;
            return STATUS_BAD; // Memory allocation failed
         }
         *arr = temp;
         *current_size = new_size;
      }
   }
   return new_size;
}
#pragma endregion

#pragma region Database Logic Functions
/// @brief Add Employee to Database
/// @param emp Employee Structure Array
/// @param count Employee Count
/// @return STATUS_GOOD, STATUS_INVALID_INPUT, STATUS_ARRAY_IS_FULL, STATUS_INVALID_INT_RANGE
/// @note This function adds an employee to the employee database structure array
status_t addEmployee(struct Employee emp[], int *count);

void searchEmployee(struct Employee emp[], int count);
void displayEmployee(struct Employee emp); 
void deleteEmployee(struct Employee emp[], int *count);
void PrintTitle(char *title, int *count);
void printMenu();
status_t getChoice();
void clearScreen();

/* Start of Database Logic Functions */
void clearScreen()
{
   printf("\e[1;1H\e[2J"); // Non-Standard escape sequence for clearing the screen, works with most modern terminals
}

status_t addEmployee(struct Employee emp[], int *count)
{

   if(*count >= MAX_EMPLOYEES)
   {
      printf("Database is full.\n");
      //TODO: implement resize function
      return STATUS_ARRAY_IS_FULL;
   }

   printf("Enter employee ID: ");
   char userInputBuffer[100];
   char *endPtr;
   long stringToLongId;

   fgets(userInputBuffer, sizeof(userInputBuffer), stdin);
   stringToLongId = strtol(userInputBuffer, &endPtr, 10); // Convert the string to a long integer
   if (endPtr == userInputBuffer || *endPtr != '\n')
   {
      printf("Invalid ID format.\n");
      clearInputBuffer(); // Clear the buffer on error
      return STATUS_INVALID_INPUT;
   }
   else if (stringToLongId < 0 || stringToLongId > INT_MAX)
   {
      printf("Invalid ID range.\n");
      clearInputBuffer(); // Clear the buffer on error
      return STATUS_INVALID_INT_RANGE;
   }
   else
   {
      emp[*count].id = (int)stringToLongId;
   }

   printf("Enter employee name: ");
   fgets(emp[*count].name, EMPLOYEE_MAX_NAME_LENGTH, stdin);
   emp[*count].name[strcspn(emp[*count].name, "\n")] = 0; // Remove the newline character from fgets
   printf("Enter department: ");
   fgets(emp[*count].department, EMPLOYEE_MAX_DEPARTMENT_LENGTH, stdin);
   emp[*count].department[strcspn(emp[*count].department, "\n")] = 0; // Remove the newline character from fgets

   printf("Enter salary: ");
   if (scanf("%f", &emp[*count].salary) != 1)
   {
      printf("Invalid salary\n\n");
      clearInputBuffer();
      return STATUS_INVALID_INPUT;
   }

   printf("Employee added successfully.\n\n");
   (*count)++;
   clearInputBuffer();
   return STATUS_GOOD;

   // clearScreen();
}

void searchEmployee(struct Employee emp[], int count)
{
   int searchId;
   printf("[DATABASE :: SEARCH EMPLOYEE]\n");
   printf("Enter employee ID to search: ");
   if (scanf("%d", &searchId) != 1)
   {
      printf("Invalid ID\n\n");
      clearInputBuffer();
      return;
   }

   for (int i = 0; i < count; i++)
   {
      if (emp[i].id == searchId)
      {
         displayEmployee(emp[i]);
         return;
      }
   }
   printf("[DATABASE :: SEARCH RESULT]\n");
   printf("Employee not found.\n\n");
}

void displayEmployee(struct Employee emp)
{
   printf("ID [%d]:\n", emp.id);
   printf("{\n");
   printf("\tName: %s\n", emp.name);
   printf("\tDepartment: %s\n", emp.department);
   printf("\tSalary: $%.2f/Hour\n", emp.salary);
   printf("}\n");
   printf("\n\n");
}

void deleteEmployee(struct Employee emp[], int *count)
{
   int deleteId;
   printf("[DATABASE :: DELETE EMPLOYEE]\n");
   for(int t = 0; t < *count; t++)
   {
      printf("[ID Found: %d] ", emp[t].id);
   }
   printf("Enter employee ID to delete: ");
   if (scanf("%d", &deleteId) != 1)
   {
      printf("Invalid ID\n\n");
      clearInputBuffer();
      return;
   }

   for (int i = 0; i < *count; i++)
   {
      if (emp[i].id == deleteId)
      {
         // Shift remaining employees to fill the gap
         for (int j = i; j < *count - 1; j++)
         {
            emp[j] = emp[j + 1];
         }
         (*count)--;
         printf("Employee deleted successfully.\n\n");

         return;
      }
   }

   printf("Employee not found.\n\n");
}

void printMenu()
{
   printf("{1.} Add Employee\n");
   printf("{2.} Display All Employees\n");
   printf("{3.} Search Employee\n");
   printf("{4.} Delete Employee\n");
   printf("{0.} Exit\n");
   return;
}

void printTitle(char *title, int *count)
{
   printf("*********************************************\n");
   printf("%s (%d)\n", title, *count);
   printf("*********************************************\n");
   return;
}

status_t getChoice()
{
   int choice;
   printf("Select An Option: ");
   if (scanf("%d", &choice) != 1)
   {
      clearInputBuffer();
      return STATUS_BAD; // Indicate invalid input
   }
   return choice;
}
#pragma endregion

#pragma region main() Entry Point
int main()
{
   int employeeCount = 0;
   int choice;

   /* allocate approximately 10,800 bytes (108 bytes/employee * 100 employees = 10,800 bytes through malloc). */
   struct Employee *employees = malloc(EMPLOYEE_MAX_ARRAY_SIZE * sizeof(struct Employee));
   size_t num_elements = EMPLOYEE_MAX_ARRAY_SIZE;
   if (employees == NULL)
   {
      printf("Memory allocation failed\n");
      return STATUS_ALLOCATION_FAILED;
   }

   do
   {
      print_employee_array_size(employees, num_elements);
      print_employee_array_size_in_bytes(employees, num_elements);
      printTitle("Employee Database | Employee's Registered", &employeeCount);
      printMenu();
      choice = getChoice();
      clearInputBuffer();

      switch (choice)
      {
      case 1:
         // printf("\n");
         clearScreen();
         addEmployee(employees, &employeeCount);
         clearScreen();
         break;

      case 2:
         // printf("\n");
         clearScreen();
         if (employeeCount == 0)
         {
            printf("[DATABASE :: RESULT]\n");
            printf("No Employees in the Database\n\n");
         }
         else
         {
            for (int i = 0; i < employeeCount; i++)
            {
               printf("[DATABASE :: RESULT]\n");
               displayEmployee(employees[i]);
            }
            // clearScreen();
         }
         break;

      case 3:
         // printf("\n");
         clearScreen();
         searchEmployee(employees, employeeCount);
         break;

      case 4:
         // printf("\n");
         clearScreen();
         deleteEmployee(employees, &employeeCount);
         break;

      case 0:
         // printf("\n");
         clearScreen();
         printf("Exiting...\n");
         printf("Database Has Shutdown\n");
         break;

      default:
         // printf("\n");
         clearScreen();
         printf("Invalid choice!\n");
      }
   } while (choice != 0);

   free(employees);
   return 0;
}
#pragma endregion