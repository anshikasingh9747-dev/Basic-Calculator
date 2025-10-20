//High-Student Management System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"

struct Student {
    int id;
    char name[50];
    int age;
    char course[30];
};

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// Function to check if file exists, if not create it
void ensureFileExists() {
    FILE *fp = fopen(FILE_NAME, "ab+"); // open for append + read
    if (fp == NULL) {
        printf("Error: Unable to create or open file '%s'.\n", FILE_NAME);
        exit(1); // exit program if file can't be created
    }
    fclose(fp);
}

int main() {
    int choice;
    ensureFileExists(); // Make sure the file is ready

    while (1) {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }
        getchar(); // clear buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addStudent() {
    FILE *fp = fopen(FILE_NAME, "ab");
    struct Student s;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &s.id);
    getchar(); // buffer clear

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0; // remove newline

    printf("Enter Age: ");
    scanf("%d", &s.age);
    getchar(); // buffer clear

    printf("Enter Course: ");
    fgets(s.course, sizeof(s.course), stdin);
    s.course[strcspn(s.course, "\n")] = 0; // remove newline

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Student added successfully!\n");
}

void displayStudents() {
    FILE *fp = fopen(FILE_NAME, "rb");
    struct Student s;

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    int count = 0;
    while (fread(&s, sizeof(s), 1, fp)) {
        printf("\nID: %d\nName: %s\nAge: %d\nCourse: %s\n", s.id, s.name, s.age, s.course);
        count++;
    }
    if (count == 0) printf("No records to display.\n");
    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    struct Student s;
    int id, found = 0;

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Student ID to search: ");
    scanf("%d", &id);
    getchar(); // buffer clear

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            printf("\nRecord Found!\n");
            printf("ID: %d\nName: %s\nAge: %d\nCourse: %s\n", s.id, s.name, s.age, s.course);
            found = 1;
            break;
        }
    }

    if (!found) printf("Record not found.\n");
    fclose(fp);
}

void updateStudent() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    struct Student s;
    int id, found = 0;

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Student ID to update: ");
    scanf("%d", &id);
    getchar(); // buffer clear

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            printf("Enter new Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf("Enter new Age: ");
            scanf("%d", &s.age);
            getchar(); // buffer clear

            printf("Enter new Course: ");
            fgets(s.course, sizeof(s.course), stdin);
            s.course[strcspn(s.course, "\n")] = 0;

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            printf("Record updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Record not found.\n");
    fclose(fp);
}

void deleteStudent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Student s;
    int id, found = 0;

    if (fp == NULL) {
        printf("No records found.\n");
        fclose(temp);
        return;
    }

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);
    getchar(); // buffer clear

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id != id)
            fwrite(&s, sizeof(s), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("Record not found.\n");
}
   
