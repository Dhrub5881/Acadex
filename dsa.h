#ifndef ACADEX_DSA_H
#define ACADEX_DSA_H

#define MAX_NOTIFICATIONS 20

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NoteNode {
    int id;
    char section[32];
    char subject[64];
    char title[120];
    char content[256];
    struct NoteNode *next;
} NoteNode;

typedef struct {
    char items[MAX_NOTIFICATIONS][180];
    int top;
} NotificationStack;

NoteNode *add_note(NoteNode *head, int id, const char *section,
                   const char *subject, const char *title, const char *content);
void print_notes_for_section(const NoteNode *head, const char *section);
void print_all_notes(const NoteNode *head);
void free_notes(NoteNode *head);

void stack_init(NotificationStack *stack);
void stack_push(NotificationStack *stack, const char *message);
void stack_print_latest_first(const NotificationStack *stack);
void stack_pop(NotificationStack *stack, char *out, int out_size);

#ifdef __cplusplus
}
#endif

#endif
