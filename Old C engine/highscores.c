#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pdcurses.h>
#include "gamedata.h"
#include "highscores.h"
#include "window.h"
#include "sound.h"

//Helper function for to get the player's class name
const char *get_class_name(enum PlayerClass class) {
    switch (class) {
        case PLAYER_FIGHTER:      return "Fighter Jet";
        case PLAYER_FORTRESS:     return "Flying Fortress";
        case PLAYER_EXPERIMENTAL: return "Experimental Fighter";
        default:                  return "Unknown";
    }
}
//Stores high scores in a file
void Write_high_scores(Player *player){
    erase();
    FILE *score_file_story = fopen("High_Scores_Story.txt", "a");
    FILE *score_file_endless = fopen("High_Scores_Endless.txt", "a");
    if (score_file_endless == NULL || score_file_story == NULL){
        fprintf(stderr, "Unable to access score record files!");
    return;
    }
    mvprintw(offset_y + PLAYFIELD_H/3 , offset_x + PLAYFIELD_W/3, "Enter high scores?(y/n)");
    char score_choice = 'a';
    while(score_choice != 'y' && score_choice != 'n'){
        scrollok(stdscr, FALSE);
        nodelay(stdscr, FALSE);
    score_choice = getch();
    if (score_choice == 'y'){
        const char *player_class = get_class_name(player->class);
        char player_name[NAME_MAX_LEN] = "BLANK";
        mvprintw(offset_y + PLAYFIELD_H/3 + 2 , offset_x + PLAYFIELD_W/3, "Enter your name.");
        wgetnstr(stdscr, player_name, NAME_MAX_LEN);
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
    if(game_mode == LEVEL_SELECT || game_mode == STORY_MODE){
        fprintf(score_file_story, "Name: %s Score: %d Class: %s\n", player_name, player->score, player_class);
    }
    else if(game_mode == ENDLESS_MODE){
       fprintf(score_file_endless, "Name: %s Score: %d Class: %s\n", player_name, player->score, player_class);
    }
    }
    if(score_choice == 'n'){
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
    mvprintw(offset_y + PLAYFIELD_H/3 + 2 , offset_x + PLAYFIELD_W/3, "Score not saved.");
    }
    }
fclose(score_file_story);
fclose(score_file_endless);
}
//Structs for storing scores after they have been read
// -1 tells the display function not to read
High_Scores scores_story[MAX_SCORES_ALLOWED] = {0};
High_Scores scores_endless[MAX_SCORES_ALLOWED] = {0};
//Variables for storing line count
    int line_count_story = 0, line_count_endless = 0;
//Function to read scores from the file and put them into the score structs
void Read_Scores(){
    line_count_story =0, line_count_endless=0;
  FILE *score_file_story = fopen("High_Scores_Story.txt", "r");
    FILE *score_file_endless = fopen("High_Scores_Endless.txt", "r");
    if (score_file_endless == NULL || score_file_story == NULL){
        fprintf(stderr, "Unable to access score record files!");
    return;
    }  
    //Counts number of lines in the score record files
    int ch1, ch2;
while ((ch1 = fgetc(score_file_story)) != EOF) {
    if (ch1 == '\n') line_count_story++;
}
rewind(score_file_story);
while ((ch2 = fgetc(score_file_endless)) != EOF) {
    if (ch2 == '\n') line_count_endless++;
}
rewind(score_file_endless);
//Section to read and store number of lines
char lines_story[MAX_SCORES_ALLOWED][200];
char lines_endless[MAX_SCORES_ALLOWED][200];
for(int i=0; i < line_count_story; i++){
    fgets(lines_story[i], sizeof(lines_story[i]), score_file_story);
}
for(int i=0; i < line_count_endless; i++){
    fgets(lines_endless[i], sizeof(lines_endless[i]), score_file_endless);
}
//Section to parse the string literals from the file into struct data
//Story mode loop
for (int i = 0; i < line_count_story; i++) {
    char *score_marker = strstr(lines_story[i], "Score:");
    char *class_marker = strstr(lines_story[i], "Class:");

    strncpy(scores_story[i].name, lines_story[i] + 6, score_marker - (lines_story[i] + 6) - 1);
    scores_story[i].name[score_marker - (lines_story[i] + 6) - 1] = '\0';

    scores_story[i].score = atoi(score_marker + 6);

    strcpy(scores_story[i].class_player, class_marker + 6);
    scores_story[i].class_player[strcspn(scores_story[i].class_player, "\n")] = '\0';
}
//Endless mode loop
for (int i = 0; i < line_count_endless; i++) {
    char *score_marker = strstr(lines_endless[i], "Score:");
    char *class_marker = strstr(lines_endless[i], "Class:");

    strncpy(scores_endless[i].name, lines_endless[i] + 6, score_marker - (lines_endless[i] + 6) - 1);
    scores_endless[i].name[score_marker - (lines_endless[i] + 6) - 1] = '\0';

    scores_endless[i].score = atoi(score_marker + 6);

    strcpy(scores_endless[i].class_player, class_marker + 6);
    scores_endless[i].class_player[strcspn(scores_endless[i].class_player, "\n")] = '\0';
}
fclose(score_file_endless);
fclose(score_file_story);
fprintf(stderr, "%s", lines_story[0]);
fprintf(stderr, "Read scores successfully!");
}
//Helper function to sort in descending order
int compare(const void *a, const void *b){
    const High_Scores *x = (const High_Scores*)a;
    const High_Scores *y = (const High_Scores*)b;
    return y->score - x->score;
};
//Function to sort the array from highest to lowest
void Sort_Score_Array(High_Scores *scores_story, High_Scores *scores_endless){
qsort(scores_story, MAX_SCORES_ALLOWED, sizeof(*scores_story), compare);
qsort(scores_endless, MAX_SCORES_ALLOWED, sizeof(*scores_endless), compare);
fprintf(stderr, "Sorted scores successfully!");
}
//Function to display the array of structs vertically
void Display_High_Scores(){  
     ma_sound_start(&loaded_sounds[Titlescreen_MUSIC]);
    erase();
fprintf(stderr, "Displaying scores");
    nodelay(stdscr, TRUE);
    int max_x =0 , max_y= 0;
    getmaxyx(stdscr, max_y, max_x);
    int choice = 'a';
    while(choice != 'q'){
        if(choice == KEY_RESIZE){
            erase();
        resize_term(0, 0);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        refresh();
        syncConsoleBufferToWindow();
        }
   mvprintw(3 ,  max_x/10, "Story Mode High Scores");
   mvprintw(3 , max_x/2, "Endless Mode High Scores");
for(int i = 0; i < MAX_SCORES_DISPLAYED; i++){
    if(scores_story[i].score <= 0) continue;
mvprintw(5 + i,  max_x/10, "%s", scores_story[i].name);
mvprintw(5 + i,  max_x/10 + 10, "%d", scores_story[i].score);
mvprintw(5 + i,  max_x/10 + 15, "%s", scores_story[i].class_player);
}
for(int p = 0; p < MAX_SCORES_DISPLAYED; p++){
 if(scores_endless[p].score <= 0) continue;
 mvprintw(5 + p, max_x/2, "%s", scores_endless[p].name);
mvprintw(5 + p,  max_x/2 + 10, "%d", scores_endless[p].score);
mvprintw(5 + p,  max_x/2 + 15, "%s", scores_endless[p].class_player);
}
choice = getch();
 if (choice == 'q'){
ma_sound_stop(&loaded_sounds[Titlescreen_MUSIC]);
    return; 
 }   
} 
}