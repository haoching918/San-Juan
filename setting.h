#ifndef SETTING
#define SETTING

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define BLUE 0
#define WHITE 1
#define BROWN 2
#define COFFEE 3
#define GRAY 4
#define PURPLE 5

#define CLEARBUF                \
    while ((getchar()) != '\n') \
        ;
#define ERROR printf("\033[1;31mInvalid input\033[0m\n");

struct _Card
{
    char *name;                     // 卡片名稱
    uint8_t cost;                   // 費用
    uint8_t points;                 // 得分
    uint8_t color;                  // 牌面顏色
    bool is_special_struct;         // 是否為特殊建築
    bool have_good;                 // 是否有貨物   
} __attribute__((__packed__));
typedef struct _Card Card;

struct _Player
{
    uint8_t id;                     // 玩家編號
    uint8_t cards;                  // 手牌數量
    uint8_t builds;                 // 建築數量
    uint32_t points;                // 積分
    Card *board[12];                // 版面卡片
    Card *hand[20];                 // 手上卡片                
    bool is_bot;                    // 使否為電腦玩家
} __attribute__((__packed__));
typedef struct _Player Player;

Card *deck[110];
Player *player_list[4];
int8_t level;

void menu();
void setup();
Card *deal(Card *deck[]);
void caution();
void set_player(Player *player_list[], Card *deck[]);
void set_card(Card *deck[]);

#endif