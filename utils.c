#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "utils.h"

const char* get_symbol_by_size_and_player(size s, player p) {
    char *empty = " ";
    switch (p) {
    case NO_PLAYER:
        return empty;
    case PLAYER_1:
        switch (s) {
            case NONE:
                return empty;
            case SMALL:
                return "x";
            case MEDIUM:
                return "X";
            case LARGE:
                return "#";
        }
    case PLAYER_2:
        switch (s) {
            case NONE:
                return empty;
            case SMALL:
                return "•";
            case MEDIUM:
                return "o";
            case LARGE:
                return "O";
        }  
    }
}

void reset_color(){
    printf("\033[0m");
}

void change_color_by_player(player p) {
    switch (p) {
    case NO_PLAYER:
        reset_color();
        break;
    case PLAYER_1:
        printf("\033[0;33m");   // yellow
        break;
    case PLAYER_2:
        printf("\033[0;34m");   // bold blue
        break;
    }
}

void change_to_red() {
    printf("\033[0;31m");   // red
}

void print_colored_piece(size s, player p) {
    change_color_by_player(p);
    printf("%s ", get_symbol_by_size_and_player(s, p));
    reset_color();
}

void print_board_level(board game,int level) {
    printf("|");
    for (int j = 0; j < DIMENSIONS; j++) {
            size s = get_piece_size(game, level, j);
            player p = get_place_holder(game, level, j);
            printf(" ");
            print_colored_piece(s, p);
            printf("|");
        }
}

void print_player_house(board game, player p) {
    for(int size = SMALL; size <= LARGE; size++){
        int nb = get_nb_piece_in_house(game, p, size);
        for(int i = 0; i < NB_INITIAL_PIECES; i++){
            if(i < nb){
                print_colored_piece(size, p);
            } else {
                print_colored_piece(NONE, NO_PLAYER);
            }
        }
    }
}

void print_house_level(board game,int level) {
    char separator[] = "      ";
    printf(separator);
    if(level == 0) {
        
        printf("Contenu des maisons");
        return;
    }
    print_player_house(game, level); // level 1 = player 1 and level 2 = player 2
}

void print_board_and_house(board game) {
    char levelSeparator[] = "+---+---+---+\n";
    printf("%s", levelSeparator);
    for (int i = 0; i < DIMENSIONS; i++) {
        print_board_level(game, i);
        print_house_level(game, i);
        printf("\n");
        printf("%s", levelSeparator);
    }
}

void print_input_error_message() {
    change_to_red();
    printf("ERREUR:\nEntrée incorrecte\n");
    reset_color();
}

char get_valid_char(char *message,char *valid_chars) {
    char c[50];
    while (1) {
        printf("%s", message);
        scanf("%s",c);
        if((strchr(valid_chars, c[0]) != NULL) && (strlen(c)<2)) {
            return c[0];
        } else {
            print_input_error_message();
        }
    }
}

int char_to_int(char c) {
    return c - '0';
}

char select_play_type() {
    char input;
    char *message = "Voulez-vous ajouter (0) ou déplacer (1) une pièce? (q pour quitter): ";	
    char *options = "01qQ";
    input = get_valid_char(message, options);

    return input;
}

int select_position(char *message) {
    char input;
    char *options = "123";
    input = get_valid_char(message, options);
    return char_to_int(input)-1;
}

int select_size() {
    char input;
    char *message = "Quelle taille de pièce voulez-vous ajouter?\n(1: petit, 2: moyen, 3: grand) ";	
    char *options = "123";
    input = get_valid_char(message, options);
    return char_to_int(input);
}

void print_separator() {
    printf("-------------------------------------\n");
}

void print_player_turn(player p) {
    change_color_by_player(p);
    printf("Joueur %d", p);
    reset_color();
    printf(", à vous de jouer :\n\n");
}

void print_winner(board game) {
    player winner = get_winner(game);
    change_color_by_player(winner);
    printf("BRAVO Joueur %d, vous avez GAGNÉ!", winner);
    reset_color();
}

void print_winner_by_quit(player winner) {
    printf("Le ");
    change_color_by_player(next_player(winner));
    printf("Joueur %d", next_player(winner));
    reset_color();
    printf(" a abandonné.\n");
    change_color_by_player(winner);
    printf("BRAVO Joueur %d, vous avez GAGNÉ!", winner);
    reset_color();
}

void print_place_error_message_by_code(int code) {
    change_to_red();
    switch (code) {
    case 1:
        printf("ERREUR:\nLa pièce n'est pas disponible dans la maison du joueur.\n");
        break;
    case 2:
        printf("ERREUR:\nLa pièce est trop petite pour être placée ici.\n");
        break;
    case 3:
        printf("ERREUR:\nEntrée incorrecte.\n");
        break;
    }
    reset_color();
}

void print_move_error_message_by_code(int code) {
    change_to_red();
    switch (code) {
    case 1:
        printf("ERREUR:\nIl n'y a pas de pièce à cet endroit\n");
        break;
    case 2:
        printf("ERREUR:\nLa pièce est trop petite pour être placée ici\n");
        break;
    case 3:
        printf("ERREUR:\nEntrée incorrecte\n");
        break;
    }
    reset_color();
}

bool exit_confirmation() {
    char input;
    char *message = "Voulez-vous vraiment quitter, Oui(o) ou Non(n)?";	
    char *options = "on";
    input = get_valid_char(message, options);
    if(input == 'o') {
        return 1;
    }
    return 0;
}

void play(board game, player currentPlayer) {
    do {
        int res = -1;
        print_separator();
        print_player_turn(currentPlayer);
        print_board_and_house(game);
        char play_type = select_play_type();
        int sourceRow;
        int sourceCol;
        int targetRow;
        int targetCol;
        switch (play_type) {
            case '0':
                targetRow = select_position("Sur quelle ligne voulez-vous jouer? ");
                targetCol = select_position("Sur quelle colonne voulez-vous jouer? ");
                int size = select_size();
                res = place_piece(game, currentPlayer, size, targetRow, targetCol);
                print_place_error_message_by_code(res);
                break;
            case '1':
                sourceRow = select_position("De quelle ligne voulez-vous déplacer? ");
                sourceCol = select_position("De quelle colonne voulez-vous déplacer? ");
                targetRow = select_position("Sur quelle ligne voulez-vous jouer? ");
                targetCol = select_position("Sur quelle colonne voulez-vous jouer? ");
                res = move_piece(game, sourceRow, sourceCol, targetRow, targetCol);
                print_move_error_message_by_code(res);
                break;
            case 'q':
                if(exit_confirmation()) {
                    print_winner_by_quit(next_player(currentPlayer));
                    exit(0);
                }
            default:
                break;
        }
        if (res==0) {
            currentPlayer = next_player(currentPlayer);
        }
    }
    while(get_winner(game)==NO_PLAYER);
    print_board_and_house(game);
}