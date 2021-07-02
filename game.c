#include "setting.h"
#include "game.h"
#include "role.h"

uint8_t chapel_count[4] = {0};

// start game
void game()
{
	uint8_t Gov_player = 1;
	uint8_t turn = 1;
	uint8_t round = 1;
	uint8_t role = 0;

	sleep(1);
	printf("\nLet's start the game\n");
	sleep(1);
	while (1)
	{
		printf("\n\033[1;33mROUND %d\033[0m\n", round);
		sleep(1);
		printf("Player %d is the Governor\n", Gov_player);
		sleep(1);
		show_supply();
		show_hand(player_list[0]);
		check_status();

		uint8_t role_list[5] = {0};
		int8_t role_count = 0;

		while (role_count < 4)
		{
			role = select_role(turn, role_list);
			switch (role)
			{
			case 1:
				printf("Player %d choose \033[0;36mBuilder\033[0m\n", turn);
				sleep(1);
				builder(turn);
				role_list[0] = 1;
				break;
			case 2:
				printf("Player %d choose \033[0;34mCouncillor\033[0m\n", turn);
				sleep(1);
				councillor(turn);
				role_list[1] = 1;
				break;
			case 3:
				printf("Player %d choose \033[0;32mProducer\033[0m\n", turn);
				sleep(1);
				producer(turn);
				role_list[2] = 1;
				break;
			case 4:
				printf("Player %d choose \033[0;33mProspector\033[0m\n", turn);
				sleep(1);
				prospector(turn);
				role_list[3] = 1;
				break;
			case 5:
				printf("Player %d choose \033[0;35mTrader\033[0m\n", turn);
				sleep(1);
				trader(turn);
				role_list[4] = 1;
				break;
			default:
				break;
			}
			role_count++;
			turn++;
			if (turn == 5)
				turn = 1;
		}
		Gov_player++;
		round++;
		if (Gov_player == 5)
			Gov_player = 1;
		turn = Gov_player;
		update(player_list);
	}
}
uint8_t select_role(uint8_t turn, uint8_t role_list[])
{
	uint8_t role = 0;
	printf("\n");
	// player
	if (turn == 1)
	{
		// show status
		show_hand(player_list[0]);
		printf("Your board :\n");
		for (size_t i = 0; i < player_list[0]->builds; i++)
		{
			if (player_list[0]->board[i]->have_good)
				printf("|| \033[0;33m%s\033[0m ||  ", player_list[0]->board[i]->name);
			else
				printf("|| %s ||  ", player_list[0]->board[i]->name);
		}
		printf("\n\n");

		// select
		printf("Choose a role\n");
		printf("1) Builder 2) Councillor 3) Producer 4) Prospector 5) Trader\n");
		for (size_t i = 0; i < 5; i++)
		{
			if (!role_list[i])
				printf("(%ld)", i + 1);
		}
		printf("\n");
		while (scanf("%hhd", &role) != 1 || role < 1 || role > 5)
		{
			ERROR
			CLEARBUF
		}
		CLEARBUF
	}
	// bot
	else
	{
		while (role = rand() % 5 + 1)
		{
			if (!role_list[role - 1])
				break;
		}
	}

	return role;
}
void show_hand(Player *player)
{
	if (!player->is_bot)
		printf("\nYour current hand:\n");
	int8_t card_count = 0;
	for (size_t i = 0; card_count < player->cards; i++)
	{
		if (!player->hand[i])
			continue;
		printf("(%ld)| %s |  ", i + 1, player->hand[i]->name);
		card_count++;
	}
	printf("\n");
}
void check_status()
{
	printf("\n");
	uint8_t choice;
	while (1)
	{
		printf("Check Player's board & status(enter 0 to exit) : ");
		if (scanf("%hhd", &choice) != 1 || choice > 4 || choice < 0)
		{
			CLEARBUF
			ERROR
			continue;
		}
		if (!choice)
			return;

		printf("Player %d\n", player_list[choice - 1]->id);
		printf("Builds : %d\n", player_list[choice-1]->builds);
		printf("Points : %d\n", player_list[choice - 1]->points);
		printf("Board :\n");
		for (size_t i = 0; i < player_list[choice - 1]->builds; i++)
		{
			if (player_list[choice - 1]->board[i]->have_good)
				printf("|| \033[0;33m%s\033[0m ||  ", player_list[choice - 1]->board[i]->name);
			else
				printf("|| %s ||  ", player_list[choice - 1]->board[i]->name);
		}
		printf("\n\n");
	}
	printf("\n");
}
uint8_t update()
{
	for (size_t i = 0; i < 4; i++)
	{
		uint8_t points = 0;
		uint8_t tower = 0;

		// Chapel
		chapel(player_list[i]);

		if (i == 0)
		{
			// check Tower
			for (size_t j = 0; j < player_list[0]->builds; j++)
			{
				if (!strcmp(player_list[0]->board[j]->name, "Tower"))
				{
					tower = 1;
					break;
				}
			}
			// Card overload
			if (player_list[0]->cards > 7 && !tower)
				overflow(player_list[0]);
		}

		// count points
		for (size_t j = 0; j < player_list[i]->builds; j++)
			points += player_list[i]->board[j]->points;
		player_list[i]->points = points;
	}

	return 0;
}
void overflow(Player *player)
{
	int8_t card_count = 0;
	int8_t card_discard = player->cards - 7;
	printf("Your have over 7 cards, please choose %d to discard", card_discard);
	show_hand(player);
	while (card_count < card_discard)
	{
		int8_t choice = 0;
		printf("Your choice : ");
		scanf("%hhd", &choice);
		CLEARBUF

		// recycle
		for (size_t i = 0; i < 110; i++)
		{
			if (!deck[i])
			{
				deck[i] = player->hand[choice - 1];
				break;
			}
		}
		player->hand[choice - 1] = NULL;
		player->cards--;

		card_count++;
	}
}
void draw(Player *player, int8_t take, int8_t keep)
{
	Card *tmp_deck[10] = {NULL};
	int8_t take_count = 0;
	int8_t keep_count = 0;
	int8_t idx = rand() % 110;
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
	if (keep == take)
	{
		int8_t i = 0;
		while (keep_count < keep)
		{
			if (!player->hand[i])
			{
				player->hand[i] = tmp_deck[keep_count];
				keep_count++;
			}
			i++;
		}
	}
	else
	{
		if (player->is_bot)
		{
			for (size_t i = 0; i < keep; i++)
			{
				idx = 0;
				while (1)
				{
					if (!player->hand[idx]) // position has no card
					{
						player->hand[idx] = tmp_deck[i];
						break;
					}
					idx++;
				}
			}
		}
		else
		{
			printf("Please choose %d to keep\n", keep);
			for (size_t i = 0; i < take; i++)
				printf("(%ld)| %s |  ", i + 1, tmp_deck[i]->name);
			printf("\n");
			for (size_t i = 0; i < keep; i++)
			{
				int8_t choice = 0;
				printf("Choose a card : ");
				if (scanf("%hhd", &choice) != 1 || choice < 1 || choice > take)
				{
					CLEARBUF
					ERROR
					i--;
					continue;
				}
				idx = 0;
				while (1)
				{
					if (!player->hand[idx])
					{
						player->hand[idx] = tmp_deck[choice - 1];
						break;
					}
					idx++;
				}
			}
		}
	}

	// recycle
	int8_t recycle_count = 0;
	int8_t tmp_idx = 0;
	for (size_t i = 0; i < 110 && recycle_count < take - keep; i++)
	{
		if (!tmp_deck[tmp_idx]) // null
		{
			i--;
			tmp_idx++;
			continue;
		}
		if (!deck[i]) // null
		{
			deck[i] = tmp_deck[tmp_idx];
			tmp_idx++;
			recycle_count++;
		}
	}

	player->cards += keep;
	printf("\033[1;34mPlayer %d draws %d and keep %d\033[0m\n", player->id, take, keep);
}
void chapel(Player *player)
{
	int8_t found = 0;
	if (player->is_bot)
		return;
	for (size_t i = 0; i < player->builds; i++)
	{
		if (!strcmp(player->board[i]->name, "Chapel"))
		{
			found = 1;
			break;
		}
	}

	if (found)
	{
		printf("\nPlease choose one card to put under Chapel (enter 0 to exit)\n");
		show_hand(player);

		while (1)
		{
			int8_t choice = 0;
			printf("Your choice : ");
			if (scanf("%hhd", &choice) != 1 || choice < 0)
			{
				CLEARBUF
				ERROR
				continue;
			}
			if (!choice)
				break;

			free(player->hand[choice - 1]);
			player->hand[choice - 1] = NULL;
			player->cards--;
			chapel_count[player->id - 1]++;
			break;
		}
	}
}
void gameover()
{
	for (size_t i = 0; i < 4; i++)
	{
		// check for special card
		int8_t guild_hall = 0, city_hall = 0, triumhal_arch = 0, palace = 0;
		for (size_t j = 0; j < player_list[i]->builds; j++)
		{
			if (!strcmp(player_list[i]->board[j]->name, "Guild hall"))
				guild_hall = 1;
			if (!strcmp(player_list[i]->board[j]->name, "City hall"))
				city_hall = 1;
			if (!strcmp(player_list[i]->board[j]->name, "Triumhal arch"))
				triumhal_arch = 1;
			if (!strcmp(player_list[i]->board[j]->name, "Palace"))
				palace = 1;
		}

		// total score without special card
		player_list[i]->points = 0;
		for (size_t j = 0; j < player_list[i]->builds; j++)
			player_list[i]->points += player_list[i]->board[j]->points;

		if (guild_hall)
		{
			for (size_t j = 0; j < player_list[i]->builds; j++)
			{
				if (!player_list[i]->board[j]->is_special_struct)
					player_list[i]->points += 2;
			}
		}
		if (city_hall)
		{
			for (size_t j = 0; j < player_list[i]->builds; j++)
			{
				if (player_list[i]->board[j]->is_special_struct)
					player_list[i]->points += 1;
			}
		}
		if (triumhal_arch)
		{
			int8_t monuments_count = 0;
			for (size_t j = 0; j < player_list[i]->builds; j++)
			{
				if (!strcmp(player_list[i]->board[j]->name, "Hero") || !strcmp(player_list[i]->board[j]->name, "Victory column") || !strcmp(player_list[i]->board[j]->name, "Statue"))
					monuments_count++;
			}
			if (monuments_count == 1)
				player_list[i]->points += 4;
			else if (monuments_count == 2)
				player_list[i]->points += 6;
			else if (monuments_count == 3)
				player_list[i]->points += 8;
		}
		if (palace)
			player_list[i]->points *= 1.25;
	}

	// sort
	int8_t max = player_list[0]->points;
	int8_t max_idx = 0;
	for (size_t i = 1; i < 4; i++)
	{
		if (max < player_list[i]->points)
		{
			max = player_list[i]->points;
			max_idx = i;
		}
	}
	printf("\033[1;33mPlayer %d is the WINNER\033[0m\n", player_list[max_idx]->id);
	check_status(player_list);
	exit(0);
}
void show_supply()
{
	// check supply
	int8_t supply = 0;
	for (size_t i = 0; i < 110; i++)
	{
		if (deck[i])
			supply++;
	}
	printf("Supply : %d\n", supply);
}
void recycle(Card *card)
{
	for (size_t i = 0; i < 110; i++)
	{
		if (!deck[i])
		{
			deck[i] = card;
			return;
		}
	}
}
