#ifndef SCORES_H
#define SCORES_H
#define MAX_SCORES_ALLOWED 500
#define NAME_MAX_LEN 40
#define MAX_SCORES_DISPLAYED 20
//Struct for storing high scores once they have been read
typedef struct High_Scores{
    char name[NAME_MAX_LEN];
    int score;
    char class_player[NAME_MAX_LEN];
}High_Scores;
extern High_Scores scores_story[MAX_SCORES_ALLOWED];
extern High_Scores scores_endless[MAX_SCORES_ALLOWED];
const char *get_class_name(enum PlayerClass class);
void Write_high_scores(Player *player);
void Read_Scores();
void Sort_Score_Array(High_Scores *scores_story, High_Scores *scores_endless);
void Display_High_Scores();
#endif