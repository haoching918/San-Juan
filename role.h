#ifndef ROLE
#define ROLE

#include "setting.h"

// builder
void builder(uint8_t turn);
int8_t select_card(Player *player, uint8_t smithy, uint8_t quarry, uint8_t cost_discount);
int8_t check_card(Player *player, int8_t idx, uint8_t cost_discount, uint8_t smithy, uint8_t quarry);
int8_t discard(Player *player, int8_t idx, int8_t card_idx);
uint8_t blackMarket(Player *player);
void build(Player *player, int8_t card_idx, uint8_t cost_discount);
void Crane(Player *player, uint8_t smithy, uint8_t quarry, uint8_t carpenter, uint8_t cost_discount);
void craneBuild(Player *player, int8_t target, uint8_t smithy, uint8_t quarry, uint8_t carpenter, uint8_t cost_discount);
int8_t craneCheck(Player *player, int8_t src, int8_t target, uint8_t smithy, uint8_t quarry, uint8_t cost_discount);

// councillor
void councillor(uint8_t turn);
void Archive(Player *player, int8_t take, int8_t keep);

// producer
void producer(uint8_t turn);
void produce_good(Player *player, int8_t n);

// prospector
void prospector(uint8_t turn);
void goldMine(Player *player);

// trader
void trader(uint8_t turn);
void sell(Player *player, uint8_t n, uint8_t market_hall);

#endif