#include "dsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DSA in C: singly linked list for notes */
NoteNode *add_note(NoteNode *head, int id, const char *section,
                   const char *subject, const char *title, const char *content) {
    NoteNode *node = (NoteNode *)malloc(sizeof(NoteNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed while adding a note.\n");
        return head;
    }

    node->id = id;
    snprintf(node->section, sizeof(node->section), "%s", section);
    snprintf(node->subject, sizeof(node->subject), "%s", subject);
    snprintf(node->title, sizeof(node->title), "%s", title);
    snprintf(node->content, sizeof(node->content), "%s", content);
    node->next = NULL;

    if (head == NULL) return node;

    NoteNode *current = head;
    while (current->next != NULL) current = current->next;
    current->next = node;
    return head;
}

void print_notes_for_section(const NoteNode *head, const char *section) {
    int found = 0;
    const NoteNode *current = head;

    while (current != NULL) {
        if (strcmp(current->section, section) == 0) {
            printf("\n[Note #%d] Section: %s | Subject: %s\nTitle: %s\nContent: %s\n",
                   current->id, current->section, current->subject,
                   current->title, current->content);
            found = 1;
        }
        current = current->next;
    }
    if (!found) printf("\nNo notes found for section %s.\n", section);
}

void print_all_notes(const NoteNode *head) {
    if (head == NULL) {
        printf("\nNo notes available.\n");
        return;
    }

    const NoteNode *current = head;
    while (current != NULL) {
        printf("\n[Note #%d] Section: %s | Subject: %s\nTitle: %s\nContent: %s\n",
               current->id, current->section, current->subject,
               current->title, current->content);
        current = current->next;
    }
}

void free_notes(NoteNode *head) {
    while (head != NULL) {
        NoteNode *next = head->next;
        free(head);
        head = next;
    }
}

/* DSA in C: stack (LIFO) for newest notifications */
void stack_init(NotificationStack *stack) {
    if (stack != NULL) stack->top = -1;
}

void stack_push(NotificationStack *stack, const char *message) {
    if (stack == NULL || message == NULL) return;

    if (stack->top == MAX_NOTIFICATIONS - 1) {
        /* Keep the newest 20 by dropping the oldest entry. */
        int i;
        for (i = 1; i < MAX_NOTIFICATIONS; ++i) {
            memmove(stack->items[i - 1], stack->items[i], sizeof(stack->items[i]));
        }
        stack->top = MAX_NOTIFICATIONS - 2;
    }

    stack->top++;
    snprintf(stack->items[stack->top], sizeof(stack->items[stack->top]), "%s", message);
}

void stack_print_latest_first(const NotificationStack *stack) {
    if (stack == NULL || stack->top < 0) {
        printf("\nNo notifications.\n");
        return;
    }

    printf("\n--- Notifications (latest first) ---\n");
    for (int i = stack->top; i >= 0; --i) {
        printf("%d. %s\n", stack->top - i + 1, stack->items[i]);
    }
}

void stack_pop(NotificationStack *stack, char *out, int out_size) {
    if (stack == NULL || stack->top < 0) {
        if (out != NULL && out_size > 0) out[0] = '\0';
        return;
    }

    if (out != NULL && out_size > 0) {
        snprintf(out, (size_t)out_size, "%s", stack->items[stack->top]);
    }
    stack->top--;
}
