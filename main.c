#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// structure for income
typedef struct {
    char source[50];
    float amount;
    char date[15];
} Income;

// structure for expense
typedef struct {
    char category[50];
    char subcategory[50];
    float amount;
    char date[15];
} Expense;

//structure for profile
typedef struct {
    char username[50];
    char password[50];
    float budget;
    float savings;
    Income income[100];
    Expense expense[100];
    int incomeCnt;
    int expenseCnt;
} Profile;

Profile profiles[100];
int profileCnt = 0;
int currentProfile = -1;


void addIncome();
void addExpense();
void addBudget();
void viewReport();
void saveData();
void loadData();
void personalSystem();
void createProfile();
void loginProfile();
void calculateSavings();

//for loading the previous data
void loadData() {
    FILE *file = fopen("profiles.dat", "rb");
    if (!file) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }
    fread(&profileCnt, sizeof(int), 1, file);
    fread(profiles, sizeof(Profile), profileCnt, file);
    fclose(file);
    printf("Data loaded successfully.\n");
}
// for saving the new data
void saveData() {
    FILE *file = fopen("profiles.dat", "wb");
    if (!file) {
        printf("Error: Could not save data.\n");
        return;
    }
    fwrite(&profileCnt, sizeof(int), 1, file);
    fwrite(profiles, sizeof(Profile), profileCnt, file);
    fclose(file);
    printf("Data saved successfully.\n");
}
// for creating profile
void createProfile() {
    if (profileCnt >= 100) {
        printf("Maximum profile limit reached. Cannot create a new profile.\n");
        return;
    }
    Profile *newProfile = &profiles[profileCnt];
    printf("Enter a username: ");
    getchar();
    fgets(newProfile->username, sizeof(newProfile->username), stdin);
    newProfile->username[strlen(newProfile->username) - 1] = '\0';

    printf("Enter a password: ");
    fgets(newProfile->password, sizeof(newProfile->password), stdin);
    newProfile->password[strlen(newProfile->password) - 1] = '\0';

    newProfile->incomeCnt = 0;
    newProfile->expenseCnt = 0;
    newProfile->budget = 0;
    newProfile->savings = 0;

    profileCnt++;
    saveData();
    printf("Profile created successfully!\n");
}

//for logging in

void loginProfile() {
    char username[50], password[50];
    printf("Enter your username: ");
    getchar();
    fgets(username, sizeof(username), stdin);
    username[strlen(username) - 1] = '\0';

    printf("Enter your password: ");
    fgets(password, sizeof(password), stdin);
    password[strlen(password) - 1] = '\0';

    for (int i = 0; i < profileCnt; i++) {
        if (strcmp(profiles[i].username, username) == 0 && strcmp(profiles[i].password, password) == 0) {
            currentProfile = i;
            printf("Login successful! Welcome, %s.\n", profiles[i].username);
            personalSystem();
            return;
        }
    }
    printf("Invalid username or password. Try again.\n");
}

//for calulate savings

void calculateSavings() {
    Profile *profile = &profiles[currentProfile];
    float totalIncome = 0, totalExpenses = 0;

    for (int i = 0; i < profile->incomeCnt; i++) {
        totalIncome += profile->income[i].amount;
    }

    for (int i = 0; i < profile->expenseCnt; i++) {
        totalExpenses += profile->expense[i].amount;
    }

    profile->savings = totalIncome - totalExpenses;

    if (totalExpenses > profile->budget) {
        printf("\nWarning: Expenses have exceeded the budget limit!\n");
    }
}

//for setting up budget

void addBudget() {
    Profile *profile = &profiles[currentProfile];
    printf("Enter your monthly budget: ");
    scanf("%f", &profile->budget);
    printf("Budget set successfully! Remaining budget: %.2f\n", profile->budget);
}

//for adding income

void addIncome() {
    Profile *profile = &profiles[currentProfile];
    if (profile->incomeCnt >= 100) {
        printf("Income limit reached. Cannot add more income entries.\n");
        return;
    }

    printf("Enter the Source of Income: ");
    getchar();
    fgets(profile->income[profile->incomeCnt].source, sizeof(profile->income[profile->incomeCnt].source), stdin);
    profile->income[profile->incomeCnt].source[strlen(profile->income[profile->incomeCnt].source) - 1] = '\0';

    printf("Enter the Amount: ");
    scanf("%f", &profile->income[profile->incomeCnt].amount);

    time_t t;
    struct tm *localTime;
    time(&t);
    localTime = localtime(&t);
    strftime(profile->income[profile->incomeCnt].date, sizeof(profile->income[profile->incomeCnt].date), "%d-%m-%Y", localTime);

    profile->incomeCnt++;
    calculateSavings();
    printf("Income added successfully! Current savings: %.2f\n", profile->savings);
}

// for adding expense

void addExpense() {
    Profile *profile = &profiles[currentProfile];
    if (profile->expenseCnt >= 100) {
        printf("Expense limit reached. Cannot add more expense entries.\n");
        return;
    }

    printf("Enter the Category: ");
    getchar();
    fgets(profile->expense[profile->expenseCnt].category, sizeof(profile->expense[profile->expenseCnt].category), stdin);
    profile->expense[profile->expenseCnt].category[strlen(profile->expense[profile->expenseCnt].category) - 1] = '\0';

    printf("Enter the Subcategory: ");
    fgets(profile->expense[profile->expenseCnt].subcategory, sizeof(profile->expense[profile->expenseCnt].subcategory), stdin);
    profile->expense[profile->expenseCnt].subcategory[strlen(profile->expense[profile->expenseCnt].subcategory) - 1] = '\0';

    printf("Enter the Amount: ");
    scanf("%f", &profile->expense[profile->expenseCnt].amount);

    profile->budget -= profile->expense[profile->expenseCnt].amount;
    if (profile->budget < 0) {
        printf("\nWarning: Budget exceeded! Remaining budget: %.2f\n", profile->budget);
    }

    time_t t;
    struct tm *localTime;
    time(&t);
    localTime = localtime(&t);
    strftime(profile->expense[profile->expenseCnt].date, sizeof(profile->expense[profile->expenseCnt].date), "%d-%m-%Y", localTime);

    profile->expenseCnt++;
    calculateSavings();
    printf("Expense added successfully! Current savings: %.2f\n", profile->savings);
}

//for view report
void viewReport() {
    Profile *profile = &profiles[currentProfile];
    printf("\n----- INCOME REPORT -----\n");
    printf("%-20s %-10s %-15s\n", "Source", "Amount", "Date");
    for (int i = 0; i < profile->incomeCnt; i++) {
        printf("%-20s %-10.2f %-15s\n", profile->income[i].source, profile->income[i].amount, profile->income[i].date);
    }

    printf("\n----- EXPENSE REPORT -----\n");
    printf("%-20s %-20s %-10s %-15s\n", "Category", "Subcategory", "Amount", "Date");
    for (int i = 0; i < profile->expenseCnt; i++) {
        printf("%-20s %-20s %-10.2f %-15s\n", profile->expense[i].category, profile->expense[i].subcategory, profile->expense[i].amount, profile->expense[i].date);
    }

    printf("\nRemaining Budget: %.2f\n", profile->budget);
    printf("Total Savings: %.2f\n", profile->savings);
}

// for initiating main personal system
void personalSystem() {
    int choice;
    do {
        printf("\nPersonal Finance Assistant - Profile: %s\n", profiles[currentProfile].username);
        printf("1. Add Income\n");
        printf("2. Add Expense\n");
        printf("3. Set Budget\n");
        printf("4. View Report\n");
        printf("5. Save Data\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addIncome(); break;
            case 2: addExpense(); break;
            case 3: addBudget(); break;
            case 4: viewReport(); break;
            case 5: saveData(); break;
            case 6: saveData(); printf("Logging out...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);
}

int main() {
    loadData();
    int choice;
    do {
        printf("\nPersonal Finance Assistant\n");
        printf("1. Create Profile\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        // at the starting of function
        
        switch (choice) {
            case 1: createProfile(); break;
            case 2: loginProfile(); break;
            case 3: saveData(); printf("Exiting...\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}
