#ifndef GAME
#define GAME

#include "setting.h"

void game();
uint8_t select_role(uint8_t turn, uint8_t role_list[]);
void show_hand(Player *player);
void check_status();
uint8_t update();
void overflow(Player *player);
void draw(Player *player, int8_t take, int8_t keep);
void chapel(Player *player);
void show_supply();
void recycle(Card *card);
void gameover();

#endif