#ifndef DIALOGUE_H
#define DIALOGUE_H
#define MAX_DIALOGUE 100
typedef struct {
    TriggerType trigger;
    bool auto_scroll; //True for dialogue that scrolls automatically, false if it requires user input to scroll
    bool bold; //Will be true for text containing critical information
    bool fired;
int trigger_time;
int color; 
char *text; 
}dialogue_entry;
extern dialogue_entry dialogue_table_1[];
extern dialogue_entry dialogue_table_2[];
extern dialogue_entry dialogue_table_3[];
extern const int dialogue_table_1_count;
extern const int dialogue_table_2_count;
extern const int dialogue_table_3_count;
// Commented out because it was inexplicably failing to work
//extern WINDOW *dialogue_win;
#define DIALOGUE_HEIGHT 1   
#define DIALOGUE_WIDTH 102  // match HUD_WIDTH for visual consistency
// void init_dialogue_window(int offset_y, int offset_x);
void update_dialogue(dialogue_entry *dialogue_table);
#endif