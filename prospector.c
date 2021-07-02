#include "setting.h"
#include "game.h"
#include "role.h"

void prospector(uint8_t turn)
{
	uint8_t turn_counter = 0;
	while (turn_counter < 4)
	{
		// check for Gold mine
		uint8_t gold_mine = 0, library = 0;
		for (size_t i = 0; i < player_list[turn - 1]->builds; i++)
		{
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Gold mine"))
				gold_mine = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Library"))
				library = 1;
		}

		if (library)
		{
			draw(player_list[turn - 1], 2, 2);
			sleep(1);
		}
		else if (turn_counter == 0)
		{
			draw(player_list[turn - 1], 1, 1);
			sleep(1);
		}

		if (gold_mine)
		{
			goldMine(player_list[turn - 1]);
			sleep(1);
		}

		turn_counter++;
		turn++;
		if (turn == 5)
			turn = 1;
	}
}
void goldMine(Player *player)
{
	Card *tmp_deck[10] = {NULL};
	int8_t take_count = 0;
	int8_t idx = rand() % 110;
	int8_t found = 1;

	while (take_count < 4)
	{
		if (deck[idx])
		{
			tmp_deck[take_count] = deck[idx];
			deck[idx] = NULL;
			take_count++;
		}
		idx = rand() % 110;
	}

	// if found
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = i + 1; j < 4; j++)
		{
			if (tmp_deck[i]->cost == tmp_deck[j]->cost)
				found = 0;
		}
	}

	// choose card
	if (found)
	{
		if (player->is_bot)
		{
			uint8_t idx = 0;
			while (1)
			{
				if (!player->hand[idx]) // position has no card
				{
					player->hand[idx] = tmp_deck[0];
					player->cards++;
					printf("\n\033[1;33mPlayer %d found gold\033[0m\n", player->id);
					break;
				}
				idx++;
			}
		}
		else
		{
			printf("\n\033[1;33mYou found gold\033[0m\n");
			for (size_t i = 0; i < 4; i++)
				printf("(%ld)| %s |  ", i + 1, tmp_deck[i]->name);
			printf("\n");

			int8_t choice = 0;
			while (1)
			{
				printf("Choose a card : ");
				if (scanf("%hhd", &choice) != 1 || choice < 1 || choice > 4)
				{
					CLEARBUF
					ERROR
					continue;
				}
				break;
			}

			idx = 0;
			while (1)
			{
				if (!player->hand[idx])
				{
					player->hand[idx] = tmp_deck[choice - 1];
					player->cards++;
					break;
				}
				idx++;
			}
		}
	}
	else
		printf("Player %d found no gold\n", player->id);

	// recycle
	int8_t recycle_count = 0;
	int8_t tmp_idx = 0;
	for (size_t i = 0; i < 110 && recycle_count < 4; i++)
	{
		if (!tmp_deck[tmp_idx]) // tmp_deck is empty
		{
			i--;
			tmp_idx++;
			continue;
		}
		if (!deck[i]) // deck is empty
		{
			deck[i] = tmp_deck[tmp_idx];
			tmp_idx++;
			recycle_count++;
		}
	}
}
