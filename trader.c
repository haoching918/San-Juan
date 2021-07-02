#include "setting.h"
#include "game.h"
#include "role.h"

uint8_t price_title[5][5] =
	{
		{1, 2, 2, 3, 3},
		{1, 1, 2, 2, 3},
		{1, 2, 2, 2, 3},
		{1, 1, 1, 2, 2},
		{1, 1, 2, 2, 2}};
int8_t title_idx;

void trader(uint8_t turn)
{
	uint8_t turn_counter = 0;
	title_idx = rand() % 5; // decide which price title

	// show price title
	printf("Price title : ");
	for (size_t i = 0; i < 5; i++)
		printf("|%d", price_title[title_idx][i]);
	printf("|\n");

	while (turn_counter < 4)
	{
		// check for special card
		uint8_t market_stand = 0, market_hall = 0, trading_post = 0, library = 0;
		for (size_t i = 0; i < player_list[turn - 1]->builds; i++)
		{
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Market stand"))
				market_stand = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Library"))
				library = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Market hall"))
				market_hall = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Trading post"))
				trading_post = 1;
		}

		uint8_t n = 1; // goods one allow to sell
		if (turn_counter == 0)
			n += 1;
		if (trading_post)
			n += 1;
		if (library)
			n = 3;

		printf("\nPlayer %d's turn\n", turn);

		// selling
		sell(player_list[turn - 1], n, market_stand);

		// Market hall
		if (market_hall)
			draw(player_list[turn - 1], 1, 1);

		turn_counter++;
		turn++;
		if (turn == 5)
			turn = 1;
		printf("\n");
		sleep(1);
	}
}
void sell(Player *player, uint8_t n, uint8_t market_hall)
{
	uint8_t earn = 0; // counting * price
	uint8_t sell_count = 0;

	// bot
	if (player->is_bot)
	{
		if (level == 1)
		{
			for (size_t i = 0; i < player->builds && sell_count < n; i++)
			{
				if (player->board[i]->have_good)
				{
					player->board[i]->have_good = false;
					sell_count++;
					earn += price_title[title_idx][player->board[i]->color];
				}
			}
		}
		else
		{
			int8_t card_color = 4;
			while (1)
			{
				for (size_t i = 0; i < player->builds && sell_count < n; i++)
				{
					// choose structure with the most profit
					if (player->board[i]->color == card_color && player->board[i]->have_good)
					{
						player->board[i]->have_good = false;
						sell_count++;
						earn += price_title[title_idx][player->board[i]->color];
					}
				}
				if (card_color == 0)
					break;
				card_color--;
			}
		}
	}
	else
	{
		printf("Please choose up to %d goods to sell (enter 0 to stop)\n", n);
		// show board
		for (size_t i = 0; i < player->builds; i++)
		{
			if (player->board[i]->have_good)
				printf("(%ld)| %s |  ", i + 1, player->board[i]->name);
		}
		printf("\n");
		// choosing
		int8_t choice = 0;
		while (sell_count < n)
		{
			printf("Your choice : ");
			if (scanf("%hhd", &choice) != 1 || choice > player->builds || choice < 0)
			{
				CLEARBUF
				ERROR
				continue;
			}
			if (!choice)
				break;
			else if (player->board[choice - 1]->is_special_struct || !player->board[choice - 1]->have_good)
			{
				CLEARBUF
				ERROR
				continue;
			}
			player->board[choice - 1]->have_good = false;
			sell_count++;
			earn += price_title[title_idx][player->board[choice - 1]->color];
		}
	}
	printf("\033[1;34mPlayer %d sell %d goods get %d cards\033[0m\n", player->id, sell_count, earn);
	draw(player, earn, earn);

	// Market hall
	if (sell_count >= 2 && market_hall)
		draw(player, 1, 1);
}