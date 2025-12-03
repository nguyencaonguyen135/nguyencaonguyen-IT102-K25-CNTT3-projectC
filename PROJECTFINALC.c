#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#define MAX 100
#define PAGE_SIZE 5


typedef struct {
    char empId[20];
    char name[50];
    char position[15];
    double baseSalary;
    int workDays;
} Employee;

typedef struct {
    int logId;           
    char empId[20];
    char date[20];
    char status[10];
} TimeSheet;

Employee employees[MAX];
int empCount = 0;

TimeSheet timesheets[MAX];
int tsCount = 0;


void removeNewLine(char *s);
void toLowerCase(char *str);
void pressEnterToContinue();
void goodbyeScreen();
int isOnlySpaces(const char *s);

int  findEmployeeIndexById(Employee list[], int n, const char *empId);
void printEmployeeHeader();
void printEmployee(const Employee *e);
void printAllEmployees(Employee list[], int n);
void addEmployee(Employee list[], int *n);
void updateEmployee();
int  deleteEmployee(Employee list[], int *n, char deleteEmpId[]);
void searchEmployeeByName(Employee list[], int n);
void sortEmployeeBySalary();
void displayEmployeesByPage();
void initSampleEmployees();

int  isLeapYear(int year);
int  getDaysInMonth(int month, int year);
int  isValidDate(const char date[]);
int  isDuplicateTimeSheet(const char empId[], const char date[]);
void timeKeeping();
int  hasTimeSheetForDate(const char empId[], int day, int month, int year);
void viewPersonalTimeSheet();


int main() {
    int choose;
    char buffer[10];
    initSampleEmployees();
    
    while (1) {
        system("cls");
        printf("\033[36m=========================================================\033[0m\n");
        printf("\033[36m|               \033[1mMENU EMPLOYEE MANAGEMENT\033[0m\033[36m                |\033[0m\n");
        printf("\033[36m=========================================================\033[0m\n");

        printf("\033[36m| 1. \033[0mCreate New Employee \033[36m                               |\n");
        printf("\033[36m| 2. \033[0mUpdate Profile Employee\033[36m                            |\n");
        printf("\033[36m| 3. \033[0mPhysical Deletion\033[36m                                  |\n");
        printf("\033[36m| 4. \033[0mDisplay Employee List\033[36m                              |\n");
        printf("\033[36m| 5. \033[0mSearch Employee by Name\033[36m                            |\n");
        printf("\033[36m| 6. \033[0mSort Employee List by Base Salary\033[36m                  |\n");
        printf("\033[36m| 7. \033[0mTime Keeping\033[36m                                       |\n");
        printf("\033[36m| 8. \033[0mView Personal TimeSheet \033[36m                           |\n");
        printf("\033[36m| 9. \033[0mExit The Program\033[36m                                   |\n");

        printf("\033[36m=========================================================\033[0m\n");
        printf("Choose: ");

        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &choose) != 1) {
            printf("\nInvalid Selection! Please Try Again...\n");
            pressEnterToContinue();
            continue;
        }

        switch (choose) {
        case 1:{
            addEmployee(employees, &empCount);
            pressEnterToContinue();
            break;
        }
        case 2:{
            updateEmployee();
            pressEnterToContinue();
            break;
        }
        case 3: {
	    	char delId[20];
		    while (1) {
		        printf("Enter the employee ID to be deleted: ");
		        if (fgets(delId, sizeof(delId), stdin) == NULL) {
		            printf("Input error. Please try again.\n");
		            continue;
		        }
		
		        removeNewLine(delId);
		
		        if (strlen(delId) == 0) {
		            printf("Employee ID cannot be empty! Please re-enter.\n");
		            continue; 
		        }
		
		        deleteEmployee(employees, &empCount, delId);
		        break;
		    	}
	
			pressEnterToContinue();
	    	break;
		}

        case 4:{
            displayEmployeesByPage();
            break;
        }
        case 5:{
            searchEmployeeByName(employees, empCount);
            pressEnterToContinue();
            break;
        }
        case 6:{
            sortEmployeeBySalary();
            pressEnterToContinue();
            break;
        }
        case 7:{
            timeKeeping();
            pressEnterToContinue();
            break;
        }
        case 8:{
            viewPersonalTimeSheet();
            pressEnterToContinue();
            break;
        }
        case 9:
            goodbyeScreen();
            return 0;
        default:
            printf("\nInvalid Selection! Please Try Again...\n");
        }
    }

    return 0;
}


void removeNewLine(char *s) {
    if (s == NULL) 
	return;
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

void toLowerCase(char *str) {
    if (str == NULL) 
	return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void pressEnterToContinue() {
    char temp[10];
    printf("\nPress Enter to return to menu...");
    fgets(temp, sizeof(temp), stdin);
}

void goodbyeScreen() {
    system("cls");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M:%S", t);

    printf("\n");
    printf("============================================================\n");
    printf("|                   THANK YOU FOR USING                    |\n");
    printf("|                EMPLOYEE MANAGEMENT SYSTEM                |\n");
    printf("============================================================\n");

    char msg1[] = "We hope to see you again soon!";
    char msg2[] = "       Have a good day!";

    printf("\n  ");
    for (int i = 0; msg1[i] != '\0'; i++) {
        putchar(msg1[i]);
        fflush(stdout);
        Sleep(40);  
    }

    printf("\n  ");
    for (int i = 0; msg2[i] != '\0'; i++) {
        putchar(msg2[i]);
        fflush(stdout);
        Sleep(40);
    }

    printf("\n\n------------------------------------------------------\n");
    printf("|   Exit Time: %s                   |\n", timeStr);
    printf("------------------------------------------------------\n");

    printf("\nPress Enter to exit...");
    getchar();   
}

int isOnlySpaces(const char *s) {
    if (s == NULL) return 1;  

    for (int i = 0; s[i] != '\0'; i++) {
        if (!isspace((unsigned char)s[i])) {
            return 0;   
        }
    }
    return 1;
}



int findEmployeeIndexById(Employee list[], int n, const char *empId) {
    if (empId == NULL) return -1;

    for (int i = 0; i < n; i++) {
        if (strcmp(list[i].empId, empId) == 0) {
            return i;
        }
    }
    return -1;
}

void printEmployeeHeader() {
    printf("\n|%-10s | %-20s | %-15s | %-12s | %s|\n", "Id", "Name", "Position", "Salary", "WorkDays");
    printf("-------------------------------------------------------------------------------\n");
}

void printEmployee(const Employee *e) {
    if (e == NULL) 
	return; 
    printf("|%-10s | %-20s | %-15s | %12.0lf | %5d   |\n",
           e->empId, e->name, e->position, e->baseSalary, e->workDays);
}

void printAllEmployees(Employee list[], int n) {
    if (n == 0) {
        printf("Empty employee list.\n");
        return;
    }

    printEmployeeHeader();
    for (int i = 0; i < n; i++) {
        printEmployee(&list[i]);
    }
}

void addEmployee(Employee list[], int *n) {
    if (*n >= MAX) {
        printf("The employee list is full. Cannot add more!\n");
        return;
    }

    Employee newEmp;
    char buffer[100];

    while (1) {
        printf("Enter employee ID: ");
        fgets(newEmp.empId, sizeof(newEmp.empId), stdin);
        removeNewLine(newEmp.empId);

        if (strlen(newEmp.empId) == 0 || isOnlySpaces(newEmp.empId)) {
            printf("ID is empty. Please re-enter!\n");
            continue;
        }

        if (findEmployeeIndexById(list, *n, newEmp.empId) != -1) {
            printf("ID already exists. Please enter another one!\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Enter employee name: ");
        fgets(newEmp.name, sizeof(newEmp.name), stdin);
        removeNewLine(newEmp.name);

        if (strlen(newEmp.name) == 0 || isOnlySpaces(newEmp.name)) {
            printf("Name is empty. Please re-enter!\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Enter position: ");
        fgets(newEmp.position, sizeof(newEmp.position), stdin);
        removeNewLine(newEmp.position);

        if (strlen(newEmp.position) == 0 || isOnlySpaces(newEmp.position)) {
            printf("Position is empty. Please re-enter!\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Enter base salary: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%lf", &newEmp.baseSalary) != 1 || newEmp.baseSalary <= 0) {
            printf("Invalid salary. Please re-enter!\n");
            continue;
        }
        break;
    }

    newEmp.workDays = 0;

    list[*n] = newEmp;
    (*n)++;

    printf("Add employee successfully!\n");
}

void updateEmployee() {
    if (empCount == 0) {
        printf("Empty employee list.\n");
        return;
    }

    char empId[20];
    char buffer[100];
    double newSalary;

    while (1) {
        printf("Enter employee ID to update: ");
        fgets(empId, sizeof(empId), stdin);
        removeNewLine(empId);

        if (strlen(empId) == 0) {
            printf("Employee ID cannot be empty! Please re-enter.\n");
            continue;
        }
        break; 
    }

    int index = findEmployeeIndexById(employees, empCount, empId);
    if (index == -1) {
        printf("No employee with ID %s found.\n", empId);
        return;
    }

    printf("\n===== CURRENT INFORMATION =====\n");
    printf("Id        : %s\n", employees[index].empId);
    printf("Name      : %s\n", employees[index].name);
    printf("Position  : %s\n", employees[index].position);
    printf("BaseSalary: %.0f\n\n", employees[index].baseSalary);

    while (1) {
        printf("Enter new position: ");
        fgets(buffer, sizeof(buffer), stdin);
        removeNewLine(buffer);

        if (strlen(buffer) == 0) {
            printf("Position is empty! Please re-enter.\n");
            continue;
        }

        strncpy(employees[index].position, buffer,
                sizeof(employees[index].position) - 1);
        employees[index].position[sizeof(employees[index].position) - 1] = '\0';
        break;
    }

    while (1) {
        printf("Enter new base salary: ");
        fgets(buffer, sizeof(buffer), stdin);
        removeNewLine(buffer);

        if (strlen(buffer) == 0) {
            printf("Basic salary is empty! Please re-enter.\n");
            continue;
        }

        if (sscanf(buffer, "%lf", &newSalary) != 1 || newSalary <= 0) {
            printf("The base salary must be a positive number! Please re-enter.\n");
            continue;
        }

        employees[index].baseSalary = newSalary;
        break;
    }

    printf("Employee profile updated successfully.\n");
}

int deleteEmployee(Employee list[], int *n, char deleteEmpId[]) {

    int index = findEmployeeIndexById(list, *n, deleteEmpId);
    if (index == -1) {
        printf("No employee with ID %s found.\n", deleteEmpId);
        return 0;
    }

    printf("\n================= EMPLOYEE INFORMATION ==================\n");
    printf("ID       : %s\n", list[index].empId);
    printf("Name     : %s\n", list[index].name);
    printf("Position : %s\n", list[index].position);
    printf("Salary   : %.0lf\n", list[index].baseSalary);
    printf("WorkDays : %d\n", list[index].workDays);

    char confirm[10];
    printf("\nAre you sure you want to delete this employee? (Y/N): ");
    fgets(confirm, sizeof(confirm), stdin);
    removeNewLine(confirm);

    if (strcmp(confirm, "Y") == 0 || strcmp(confirm, "y") == 0) {
        for (int i = index; i < *n - 1; i++) {
            list[i] = list[i + 1];
        }
        (*n)--;
        printf("\nSuccessfully deleted employee %s!\n", deleteEmpId);
        return 1;
    }

    if (strcmp(confirm, "N") == 0 || strcmp(confirm, "n") == 0) {
        printf("\nDelete request has been canceled.\n");
        return 0;
    }

    printf("\nInvalid selection. Delete canceled.\n");
    return 0;
}

void searchEmployeeByName(Employee list[], int n) {
    if (n == 0) {
        printf("Empty employee list.\n");
        return;
    }

    char name[50];
    char nameLower[50];
    int found = 0;

    printf("Enter the name of the employee you want to look up: ");
    fgets(name, sizeof(name), stdin);
    removeNewLine(name);

    if (strlen(name) == 0) {
        printf("Name is empty. Please re-enter!\n");
        return;
    }

    strcpy(nameLower, name);
    toLowerCase(nameLower);

    printf("\n=========================== Search results by name ============================");
    printEmployeeHeader();

    for (int i = 0; i < n; i++) {
        char empNameLower[50];
        strncpy(empNameLower, list[i].name, sizeof(empNameLower) - 1);
        empNameLower[sizeof(empNameLower) - 1] = '\0';

        toLowerCase(empNameLower);

        if (strstr(empNameLower, nameLower) != NULL) {
            printEmployee(&list[i]);
            found = 1;
        }
    }
    printf("-------------------------------------------------------------------------------\n");

    if (!found) {
        printf("No employee found with name \"%s\".\n", name);
    }
}

void sortEmployeeBySalary() {
    if (empCount == 0) {
        printf("Empty employee list.\n");
        return;
    }

    int sortOrder;
    char buffer[20];

    while (1) {
        printf("\n======== Sort employees by base salary =========\n");
        printf("| 1. Ascending order                           |\n");
        printf("| 2. Descending order                          |\n");
        printf("------------------------------------------------\n");
        printf("Enter your choice: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Please try again!\n");
            continue;
        }

        removeNewLine(buffer);

        if (strlen(buffer) == 0) {
            printf("Choice cannot be empty! Please enter 1 or 2.\n");
            continue;
        }
        if (sscanf(buffer, "%d", &sortOrder) != 1) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        if (sortOrder != 1 && sortOrder != 2) {
            printf("Invalid selection! Please enter 1 or 2.\n");
            continue;
        }

        break; 
    }
    for (int i = 0; i < empCount - 1; i++) {
        for (int j = i + 1; j < empCount; j++) {

            int needSwap = 
                (sortOrder == 1 && employees[i].baseSalary > employees[j].baseSalary) ||
                (sortOrder == 2 && employees[i].baseSalary < employees[j].baseSalary);

            if (needSwap) {
                Employee temp = employees[i];
                employees[i] = employees[j];
                employees[j] = temp;
            }
        }
    }

    printf("\nSorted by salary (%s) successfully!\n",
           sortOrder == 1 ? "Ascending" : "Descending");
    printf("You choose option 4 to view the sorted employee list.\n");
}

void displayEmployeesByPage() { 
    if (empCount == 0) {
        printf("\nEmpty employee list.\n");
        pressEnterToContinue();
        return;
    }

    int totalPages = (empCount + PAGE_SIZE - 1) / PAGE_SIZE;
    int page;
    char buffer[20];

    while (1) {
        printf("\nEnter page number to view (1 - %d, or 0 to exit): ", totalPages);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nInput error. Please try again.\n");
            continue;
        }
        
        if (buffer[0] == '\n') {
            printf("\nPage number cannot be empty! Please enter a number.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &page) != 1) {
            printf("\nInvalid input. Please enter a number.\n");
            continue;
        }

        if (page == 0) {
            printf("\nReturn to menu...\n");
            return;
        }

        if (page < 1 || page > totalPages) {
            printf("\nPage not valid! Please try again...\n");
            continue;
        }

        int start = (page - 1) * PAGE_SIZE;
        int end   = start + PAGE_SIZE;
        if (end > empCount) end = empCount;

        printf("\n======================== EMPLOYEE LIST (PAGE %d / %d) ===========================",
               page, totalPages);
        printEmployeeHeader();

        for (int i = start; i < end; i++) {
            printEmployee(&employees[i]);
        }

        printf("-------------------------------------------------------------------------------\n");
        printf("Press Enter to continue...");
        getchar(); 
    }
}

void initSampleEmployees() {
    Employee sample[] = {
        {"01", "Nguyen Van Nam",   "President",  37500000, 25},
        {"02", "Pham Van Loc",     "Employee",   12300000, 26},
        {"03", "Nguyen Huy Hoang", "Manager",    22100500, 28},
        {"04", "Nguyen Thi Huong", "Accountant", 18450700, 26},
        {"05", "Tran Bich Ngoc",   "Employee",   11400800, 24},
        {"06", "Tran Minh Hoang",  "Employee",   15200000, 28},
        {"07", "Nguyen Thai Viet", "CEO",        29780000, 27},
        {"08", "Le Viet Tien",     "Leader",     19360000, 28},
        {"09", "Cao Thai Minh",    "Secretary",  17880000, 26},
        {"10","Le Xuan Quynh",     "Employee",   13330550, 27}
    };

    int sampleCount = (int)(sizeof(sample) / sizeof(sample[0]));

    for (int i = 0; i < sampleCount && empCount < MAX; i++) {
        employees[empCount++] = sample[i];
    }
}



int isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int getDaysInMonth(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

int isValidDate(const char date[]) {
    int d, m, y;
    if (sscanf(date, "%d/%d/%d", &d, &m, &y) != 3) {
        return 0;
    }
    if (y < 1900 || m < 1 || m > 12) {
        return 0;
    }
    int days = getDaysInMonth(m, y);
    if (days == 0) return 0;
    if (d < 1 || d > days) return 0;
    return 1;
}



int isDuplicateTimeSheet(const char empId[], const char date[]) {
    for (int i = 0; i < tsCount; i++) {
        if (strcmp(timesheets[i].empId, empId) == 0 &&
            strcmp(timesheets[i].date,  date)  == 0) {
            return 1;
        }
    }
    return 0;
}

void timeKeeping() {
    char empId[20];
    char checkInDate[20];

    if (empCount == 0) {
        printf("Empty employee list.\n");
        return;
    }

    while (1) {
        printf("\nEnter employee ID: ");
        fgets(empId, sizeof(empId), stdin);
        removeNewLine(empId);

        if (strlen(empId) == 0) {
            printf("Employee ID cannot be empty!\n");
            continue;
        }
        break;
    }

    int index = findEmployeeIndexById(employees, empCount, empId);
    if (index == -1) {
        printf("No employee with ID [%s] found!\n", empId);
        return;
    }

    while (1) {
        printf("Enter check-in date (dd/mm/yyyy): ");
        fgets(checkInDate, sizeof(checkInDate), stdin);
        removeNewLine(checkInDate);

        if (strlen(checkInDate) == 0) {
            printf("Check-in date cannot be empty!\n");
            continue;
        }

        if (!isValidDate(checkInDate)) {
            printf("Invalid date! Use dd/mm/yyyy.\n");
            continue;
        }
        break;
    }

    if (isDuplicateTimeSheet(empId, checkInDate)) {
        printf("Employee [%s] has already been clocked in on this day.\n", empId);
        return;
    }

    employees[index].workDays++;

    TimeSheet ts;
    ts.logId = tsCount + 1;
    strcpy(ts.empId, empId);
    strcpy(ts.date, checkInDate);
    strcpy(ts.status, "Present");

    timesheets[tsCount++] = ts;

    printf("Timekeeping for employee [%s] on [%s] was successful.\n",
           empId, checkInDate);
}

int hasTimeSheetForDate(const char empId[], int day, int month, int year) {
    char dateStr[20];
    sprintf(dateStr, "%02d/%02d/%04d", day, month, year);

    for (int i = 0; i < tsCount; i++) {
        if (strcmp(timesheets[i].empId, empId) == 0 &&
            strcmp(timesheets[i].date,  dateStr) == 0) {
            return 1;
        }
    }
    return 0;
}

void viewPersonalTimeSheet() {
    if (empCount == 0) {
        printf("\nEmpty employee list!\n");
        return;
    }

    char id[20];
    char monthInput[20];
    int month, year;

    while (1) {
        printf("\nEnter employee ID to view timesheet: ");
        fgets(id, sizeof(id), stdin);
        removeNewLine(id);

        if (strlen(id) == 0) {
            printf("Employee ID cannot be empty!\n");
            continue;
        }
        break;
    }

    int index = findEmployeeIndexById(employees, empCount, id);
    if (index == -1) {
        printf("\nEmployee ID \"%s\" does not exist!\n", id);
        return;
    }

    while (1) {
        printf("Enter month and year to view (mm/yyyy): ");
        fgets(monthInput, sizeof(monthInput), stdin);
        removeNewLine(monthInput);

        if (sscanf(monthInput, "%d/%d", &month, &year) != 2 ||
            month < 1 || month > 12 || year < 1900) {
            printf("Invalid month/year format! Please use mm/yyyy.\n");
            continue;
        }

        if (getDaysInMonth(month, year) == 0) {
            printf("Invalid month/year! Please re-enter.\n");
            continue;
        }
        break;
    }

    int daysInMonth = getDaysInMonth(month, year);

    printf("\n========== PERSONAL TIMESHEET ==========\n");
    printf("Employee : %s - %s\n", employees[index].empId, employees[index].name);
    printf("Month/Year: %02d/%04d\n", month, year);
    printf("|--------------------------------------|\n");
    printf("|%-10s | %-12s | %-10s|\n", "ID Log", "Date", "Status");
    printf("|--------------------------------------|\n");

    for (int day = 1; day <= daysInMonth; day++) {
        char dateStr[20];
        sprintf(dateStr, "%02d/%02d/%04d", day, month, year);

        int present = hasTimeSheetForDate(id, day, month, year);

        printf("|%-10d | %-12s | %-10s|\n",
               day,
               dateStr,
               present ? "Present" : "Absent");
    }

    printf("----------------------------------------\n");
    printf("Timesheet displayed successfully!\n");
}

