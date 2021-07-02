#include "setting.h"
#include "game.h"
#include "role.h"

void councillor(uint8_t turn)
{
	uint8_t turn_counter = 0;

	while (turn_counter < 4)
	{
		// check for special card
		uint8_t prefecture = 0, library = 0, archive = 0;
		for (size_t i = 0; i < player_list[turn - 1]->builds; i++)
		{
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Prefecture"))
				prefecture = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Library"))
				library = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Archive"))
				archive = 1;
		}

		printf("\nPlayer %d's turn\n", turn);
		if (library)
		{
			if (archive && !player_list[turn-1]->is_bot)
				Archive(player_list[turn - 1], 8, 1 + prefecture);
			else
				draw(player_list[turn - 1], 8, 1 + prefecture);
		}
		else if (turn_counter == 0)
		{
			if (archive && !player_list[turn-1]->is_bot)
				Archive(player_list[turn - 1], 5, 1 + prefecture);
			else
				draw(player_list[turn - 1], 5, 1 + prefecture);
		}
		else
		{
			if (archive && !player_list[turn-1]->is_bot)
				Archive(player_list[turn - 1], 2, 1 + prefecture);
			else	
				draw(player_list[turn - 1], 2, 1 + prefecture);
		}

		turn_counter++;
		turn++;
		if (turn == 5)
			turn = 1;
		printf("\n");
		sleep(1);
	}
}
void Archive(Player *player, int8_t take, int8_t keep)
{
	if (player->is_bot)
		return;

	Card *tmp_deck[10] = {NULL};
	int8_t take_count = 0;
	int8_t keep_count = 0;
	int8_t idx = rand() % 110;
	int8_t listCard_count = 0;

	while (take_count < take)
	{
		if (deck[idx])
		{
			tmp_deck[take_count] = deck[idx];
			deck[idx] = NULL;
			take_count++;
		}
		idx = rand() % 110;
	}

	// add to hand first
	idx = 0;
	while (keep_count < take_count)
	{
		if (!player->hand[idx])
		{
			player->hand[idx] = tmp_deck[keep_count];
			keep_count++;
		}
		idx++;
	}

	keep_count = 0;
	printf("Please choose %d to discard\n", take - keep);
	for (size_t i = 0; listCard_count < player->cards + take; i++)
	{
		if (player->hand[i])
		{
			printf("(%ld)| %s |  ", i + 1, player->hand[i]->name);
			listCard_count++;
		}
	}
	printf("\n");
	for (size_t i = 0; i < take - keep; i++)
	{
		int8_t choice = 0;
		printf("Your choice : ");
		if (scanf("%hhd", &choice) != 1 || choice < 1)
		{
			CLEARBUF
			ERROR
			i--;
			continue;
		}
		else if (!player->hand[choice - 1])
		{
			CLEARBUF
			ERROR
			i--;
			continue;
		}

		// recycle & discard
		for (size_t i = 0; i < 110; i++)
		{
			if (!deck[i])
			{
				deck[i] = player->hand[choice - 1];
				player->hand[choice - 1] = NULL;
				break;
			}
		}
	}
	player->cards += keep;
}