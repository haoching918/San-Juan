#include "setting.h"
#include "game.h"
#include "role.h"

void producer(uint8_t turn)
{
	uint8_t turn_counter = 0;
	while (turn_counter < 4)
	{
		// check for special card
		uint8_t well = 0, aqueduct = 0, library = 0;
		for (size_t i = 0; i < player_list[turn - 1]->builds; i++)
		{
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Well"))
				well = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Library"))
				library = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Aqueduct"))
				aqueduct = 1;
		}

		// at most good
		uint8_t n = 1;
		if (turn_counter == 0)
			n += 1;
		if (aqueduct)
			n += 1;
		if (library)
			n = 3;

		printf("\nPlayer %d's turn\n", turn);

		if ((aqueduct || library || turn_counter == 0) && well)
			draw(player_list[turn - 1], 1, 1);

		produce_good(player_list[turn - 1], n);

		turn_counter++;
		turn++;
		if (turn == 5)
			turn = 1;

		sleep(1);
	}
}
void produce_good(Player *player, int8_t n)
{
	int8_t deck_idx = rand() % 110;
	int8_t valid_produce = 0;
	int8_t real_produce = 0;
	int8_t take_count = 0;
	int8_t produce_count = 0;

	// check valid produce
	for (size_t i = 0; i < player->builds; i++)
	{
		if (player->board[i]->is_special_struct || player->board[i]->have_good)
			continue;
		valid_produce++;
	}

	// check at most produce
	if (valid_produce > n)
		real_produce = n;
	else
		real_produce = valid_produce;

	// discard card from deck & recycle
	while (take_count < real_produce)
	{
		if (deck[deck_idx])
			take_count++;
		deck_idx = rand() % 110;
	}

	// produce good
	if (valid_produce <= n || (player->is_bot && level == 1)) // no need to choose scenario or level 1 bot
	{
		for (size_t i = 0; i < player->builds && produce_count < real_produce; i++)
		{
			if (player->board[i]->is_special_struct || player->board[i]->have_good)
				continue;
			player->board[i]->have_good = true;
			produce_count++;
		}
	}
	else if (player->is_bot && level == 2) // level 2 bot
	{
		int8_t card_color = 4;
		while (1)
		{
			for (size_t i = 0; i < player->builds && produce_count < real_produce; i++)
			{
				// choose structure with the most profit
				if (player->board[i]->color == card_color && !player->board[i]->have_good)
				{
					produce_count++;
					player->board[i]->have_good = true;
				}
			}
			if (card_color == 0)
				break;
			card_color--;
		}
	}
	else
	{
		int8_t idx = 0;
		int8_t choice = 0;
		printf("Please choose %d to produce good\n", real_produce);
		// show board
		for (size_t i = 0; i < player->builds; i++)
		{
			if (player->board[i]->have_good || player->board[i]->is_special_struct)
				continue;
			printf("(%ld)|| %s ||  ", i + 1, player->board[i]->name);
		}
		printf("\n");
		// choose
		while (produce_count < real_produce)
		{
			printf("Your choice : ");
			if (scanf("%hhd", &choice) != 1 || choice < 0 || choice > player->builds)
			{
				CLEARBUF
				ERROR
				continue;
			}
			else if (!player->board[choice - 1] || player->board[choice - 1]->is_special_struct || player->board[choice - 1]->have_good)
			{
				CLEARBUF
				ERROR
				continue;
			}
			player->board[choice - 1]->have_good = true;
			produce_count++;
		}
	}
	printf("\033[1;34mPlayer %d produce %d goods\033[0m\n", player->id, produce_count);
}