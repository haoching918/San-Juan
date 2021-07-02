#include "setting.h"
#include "game.h"
#include "role.h"

uint8_t end = 0;

void builder(uint8_t turn)
{
	uint8_t turn_counter = 0;

	// Action
	while (turn_counter < 4)
	{
		int8_t card_idx = 0;
		uint8_t cost_discount = 0;
		int8_t crane_choice = 0;

		// check special card
		uint8_t smithy = 0, poor_house = 0, carpenter = 0, quarry = 0, library = 0, black_market = 0, crane = 0;
		for (size_t i = 0; i < player_list[turn - 1]->builds; i++)
		{
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Smithy"))
				smithy = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Poor house"))
				poor_house = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Carpenter"))
				carpenter = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Quarry"))
				quarry = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Library"))
				library = 2;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Black market"))
				black_market = 1;
			if (!strcmp(player_list[turn - 1]->board[i]->name, "Crane"))
				crane = 1;
		}

		// discount
		if (turn_counter == 0)
			cost_discount++;
		if (library)
			cost_discount += 2;

		printf("\nPlayer %d's turn\n", turn);
		// Black market
		if (black_market && !player_list[turn - 1]->is_bot)
			cost_discount += blackMarket(player_list[turn - 1]);
		// Crane
		if (crane && !player_list[turn - 1]->is_bot)
		{
			printf("Do you want to use crane? 1)YES 2)NO\n");
			while (1)
			{
				printf("Your choice : ");
				if (scanf("%hhd", &crane_choice) != 1 || (crane_choice != 1 && crane_choice != 2))
				{
					CLEARBUF
					ERROR
					continue;
				}
				break;
			}
		}
		if (crane_choice == 1)
			Crane(player_list[turn - 1], smithy, quarry, carpenter, cost_discount);
		else
		{
			// select a card to build or don't
			card_idx = select_card(player_list[turn - 1], smithy, quarry, cost_discount);
			if (card_idx == -1)
			{
				printf("Player %d build nothing\n", turn);
				turn_counter++;
				turn++;
				if (turn == 5)
					turn = 1;
				sleep(1);
				continue;
			}
			// add discount
			if (player_list[turn - 1]->hand[card_idx]->is_special_struct && quarry)
				cost_discount++;
			else if ((!player_list[turn - 1]->hand[card_idx]->is_special_struct) && smithy)
				cost_discount++;
			// carpenter
			if (carpenter && player_list[turn - 1]->hand[card_idx]->is_special_struct)
				draw(player_list[turn - 1], 1, 1);

			// start building
			build(player_list[turn - 1], card_idx, cost_discount);
		}

		// if end
		if (end)
			gameover(player_list);

		// poor house
		if (poor_house && player_list[turn - 1]->cards <= 1)
			draw(player_list[turn - 1], 1, 1);

		turn_counter++;
		turn++;
		// 4 -> 1
		if (turn == 5)
			turn = 1;
		printf("\n");
		sleep(1);
	}
}
// return -1 if don't
int8_t select_card(Player *player, uint8_t smithy, uint8_t quarry, uint8_t cost_discount)
{
	uint8_t valid_card_count = 0;

	// player
	if (!player->is_bot)
	{
		uint8_t idx = 0;
		while (1)
		{
			int8_t choice = 0;
			printf("\nPlease choose a card you want to build (enter 0 to exit)\n");
			for (size_t i = 0; valid_card_count < player->cards; i++)
			{
				if (!player->hand[i])
					continue;
				else
				{
					printf("(%ld)| %s |  ", i + 1, player->hand[i]->name);
					valid_card_count++;
				}
			}
			printf("\n");
			while (1)
			{
				printf("Your choice : ");
				if (scanf("%hhd", &choice) != 1)
				{
					ERROR
					CLEARBUF
					continue;
				}

				// check selection
				idx = choice - 1;
				if (choice == 0)
					return -1;
				if (check_card(player, idx, cost_discount, smithy, quarry) == -1)
					continue;
				return idx;
			}
		}
	}
	// computer
	else
	{
		for (size_t i = 0; valid_card_count < player->cards; i++)
		{
			if (!player->hand[i])
				continue;
			if (check_card(player, i, cost_discount, smithy, quarry) == 1)
				return i;
			else
				valid_card_count++;
		}
		return -1;
	}
}
// return 1 if sucess -1 if fail
int8_t check_card(Player *player, int8_t idx, uint8_t cost_discount, uint8_t smithy, uint8_t quarry)
{
	if (!player->hand[idx])
	{
		if (player->is_bot)
			return -1;
		printf("\033[1;31mCard not found\033[0m\n");
		return -1;
	}

	for (size_t i = 0; i < player->builds; i++)
	{
		if (!strcmp(player->hand[idx]->name, player->board[i]->name) && player->board[i]->is_special_struct)
		{
			if (player->is_bot)
				return -1;
			printf("\033[1;31mStructure already exist\033[0m\n");
			return -1;
		}
	}

	if (player->hand[idx]->is_special_struct)
	{
		if ((player->hand[idx]->cost - cost_discount - quarry) > player->cards - 1)
		{
			if (player->is_bot)
				return -1;
			printf("\033[1;31mToo expensive\033[0m\n");
			return -1;
		}
	}
	else
	{
		if ((player->hand[idx]->cost - cost_discount - smithy) > player->cards - 1)
		{
			if (player->is_bot)
				return -1;
			printf("\033[1;31mToo expensive\033[0m\n");
			return -1;
		}
	}
	return 1;
}
void build(Player *player, int8_t card_idx, uint8_t cost_discount)
{
	uint8_t idx = 0;
	uint8_t paid_count = 0;
	int8_t final_cost = player->hand[card_idx]->cost - cost_discount;

	// bot
	if (player->is_bot)
	{
		if (final_cost > 0)
		{
			// paying
			for (size_t i = 0; paid_count < final_cost; i++)
			{
				if (!player->hand[i] || i == card_idx)
					continue;
				recycle(player->hand[i]);
				player->hand[i] = NULL;
				player->cards--;
				paid_count++;
			}
		}

		// building
		player->board[player->builds] = player->hand[card_idx];
		printf("\033[1;34mPlayer %d build %s\033[0m\n", player->id, player->board[player->builds]->name);
		player->hand[card_idx] = NULL;
		player->cards--;
		player->builds++;
	}
	// player
	else
	{
		if (final_cost > 0)
		{
			// paying
			int8_t card_count = 0;
			printf("Please choose %d cards to pay\n", final_cost);
			for (size_t i = 0; card_count < player->cards; i++)
			{
				if (!player->hand[i])
					continue;
				if (i == card_idx)
				{
					card_count++;
					continue;
				}
				else
				{
					printf("(%ld)| %s |  ", i + 1, player->hand[i]->name);
					card_count++;
				}
			}
			printf("\n");
			card_count = 0;
			while (card_count < final_cost)
			{
				int8_t choice = 0;
				printf("Your choice : ");
				scanf("%hhd", &choice);
				CLEARBUF
				if (discard(player, choice - 1, card_idx) == -1)
					continue;
				card_count++;
			}
		}

		// building
		player->board[player->builds] = player->hand[card_idx];
		printf("\033[1;34mYou build %s\033[0m\n", player->board[player->builds]->name);
		player->hand[card_idx] = NULL;
		player->cards--;
		player->builds++;
	}
	// if gameover
	if (player->builds == 12)
		end = 1;
}
// return -1 if failed 1 if success
int8_t discard(Player *player, int8_t idx, int8_t card_idx)
{
	if (!player->hand[idx] || idx == card_idx)
	{
		ERROR
		return -1;
	}
	recycle(player->hand[idx]);
	player->hand[idx] = NULL;
	player->cards--;
	return 1;
}
uint8_t blackMarket(Player *player)
{
	int8_t pay_count = 0;
	int8_t choice = 0;
	printf("Please select up to 2 goods to pay for building (enter 0 to exit)\n");
	for (size_t i = 0; i < player->builds; i++)
	{
		if (player->board[i]->have_good)
			printf("(%ld)|| %s ||  ", i + 1, player->board[i]->name);
	}
	printf("\n");
	while (pay_count < 2)
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
			ERROR
			continue;
		}
		player->board[choice - 1]->have_good = false;
		pay_count++;
	}
	printf("\033[1;34mYou have paid %d good\033[0m\n", pay_count);
	return pay_count;
}
void Crane(Player *player, uint8_t smithy, uint8_t quarry, uint8_t carpenter, uint8_t cost_discount)
{
	int8_t choice = 0;
	printf("Please choose a structure to build over (enter 0 to exit)\n");
	for (size_t i = 0; i < player->builds; i++)
		printf("(%ld)| %s |  ", i + 1, player->board[i]->name);
	printf("\n");
	while (1)
	{
		printf("Your choice : ");
		if (scanf("%hhd", &choice) != 1 || choice < 0 || choice > player->builds)
		{
			CLEARBUF
			ERROR
			continue;
		}
		if (!choice)
			break;
		else if (!strcmp(player->board[choice - 1]->name, "Crane"))
		{
			CLEARBUF
			ERROR
			continue;
		}
		else
		{
			craneBuild(player, choice - 1, smithy, quarry, carpenter, cost_discount);
			break;
		}
	}
}

void craneBuild(Player *player, int8_t target, uint8_t smithy, uint8_t quarry, uint8_t carpenter, uint8_t cost_discount)
{
	int8_t choice = 0;
	int8_t src = 0; // card index you want to build
	int8_t final_cost = 0;
	printf("\nPlease choose a card to builde (enter 0 to exit)");
	show_hand(player);
	while (1)
	{
		printf("Your choice : ");
		if (scanf("%hhd", &choice) != 1 || choice < 0 || choice > 20)
		{
			CLEARBUF
			ERROR
			continue;
		}
		src = choice - 1;
		if (!choice)
			break;
		else if (!player->hand[choice - 1] || (final_cost = craneCheck(player, src, target, smithy, quarry, cost_discount)) == -1)
		{
			CLEARBUF
			ERROR
			continue;
		}
		// paying
		else if (final_cost > 0)
		{
			// carpenter
			if (carpenter && player->hand[src]->is_special_struct)
				draw(player, 1, 1);

			int8_t card_count = 0;
			printf("\nPlease choose %d cards to pay\n", final_cost);
			// show hand
			for (size_t i = 0; card_count < player->cards; i++)
			{
				if (!player->hand[i])
					continue;
				if (i == choice - 1)
				{
					card_count++;
					continue;
				}
				else
				{
					printf("(%ld)| %s |  ", i + 1, player->hand[i]->name);
					card_count++;
				}
			}
			printf("\n");
			card_count = 0;
			while (card_count < final_cost)
			{
				printf("Your choice : ");
				if (scanf("%hhd", &choice) != 1 || choice < 0)
					CLEARBUF
				if (discard(player, choice - 1, src) == -1)
					continue;
				card_count++;
			}
		}
		// building
		free(player->board[target]);
		player->board[target] = player->hand[src];
		printf("\033[1;34mYou build %s\033[0m\n", player->board[target]->name);
		player->hand[src] = NULL;
		player->cards--;
		break;
	}
}
//return -1 if invalid
int8_t craneCheck(Player *player, int8_t src, int8_t target, uint8_t smithy, uint8_t quarry, uint8_t cost_discount)
{
	int final_cost = 0;
	if (player->hand[src]->is_special_struct)
	{
		final_cost = player->hand[src]->cost - player->board[target]->cost - quarry - cost_discount;
		if (final_cost > player->cards - 1)
			return -1;
	}
	else
	{
		final_cost = player->hand[src]->cost - player->board[target]->cost - smithy - cost_discount;
		if (final_cost > player->cards - 1)
			return -1;
	}

	if (final_cost < 0)
		final_cost = 0;
	return final_cost;
}