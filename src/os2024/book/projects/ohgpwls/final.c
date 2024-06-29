
#include <stdio.h>
#include <time.h>

#define MAX_TASKS 10

typedef struct {
    char name[100];
    int duration;  // Duration in days
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int task_count;
    struct tm final_date;
} ReverseProjectManager;

void add_task(ReverseProjectManager* manager, const char* name, int duration) {
    if (manager->task_count < MAX_TASKS) {
        Task* task = &manager->tasks[manager->task_count++];
        snprintf(task->name, sizeof(task->name), "%s", name);
        task->duration = duration;
    }
}

void print_date(struct tm date) {
    printf("%04d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
}

void calculate_schedule(ReverseProjectManager* manager) {
    struct tm current_date = manager->final_date;

    for (int i = manager->task_count - 1; i >= 0; --i) {
        Task* task = &manager->tasks[i];

        // Calculate start date
        current_date.tm_mday -= task->duration;
        mktime(&current_date);  // Normalize the date structure

        printf("Task: %s, Start Date: ", task->name);
        print_date(current_date);
        printf(", End Date: ");
        current_date.tm_mday += task->duration;
        mktime(&current_date);  // Normalize the date structure
        print_date(current_date);
        printf("\n");
    }
}

int main() {
    ReverseProjectManager manager = { .task_count = 0 };

    // Set the final date (example: 2024-07-01)
    manager.final_date.tm_year = 2024 - 1900;
    manager.final_date.tm_mon = 7 - 1;
    manager.final_date.tm_mday = 1;
    manager.final_date.tm_hour = 0;
    manager.final_date.tm_min = 0;
    manager.final_date.tm_sec = 0;
    mktime(&manager.final_date);  // Normalize the date structure

    // Add tasks (task name, duration in days)
    add_task(&manager, "최종 검토", 1);
    add_task(&manager, "초안 작성", 3);
    add_task(&manager, "자료 수집", 2);
    add_task(&manager, "주제 선정", 1);

    // Calculate and print the schedule
    calculate_schedule(&manager);

    return 0;
}
