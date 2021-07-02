#include "setting.h"

void menu()
{
	uint8_t choice;
	level = 1; // default
	printf("\033[3;33mWelcome to San Juan!\033[0m\n");
	while (1)
	{
		printf("\nPress \n1)start 2)help 3)level choose\n");
		if (scanf("%hhd", &choice) != 1 || (choice != 1 && choice != 2 && choice != 3))
		{
			CLEARBUF
			ERROR
			continue;
		}
		if (choice == 2)
		{
			caution();
			continue;
		}
		if (choice == 3)
		{
			printf("Please choose a level : 1 or 2\n");
			printf("Your choice : ");
			if (scanf("%hhd", &level) != 1 || (level != 1 && level != 2))
			{
				CLEARBUF
				ERROR
			}
			continue;
		}
		break;
	}
	CLEARBUF
}

void setup()
{
	srand(time(NULL));

	// create deck
	set_card(deck);

	// create 4 player
	set_player(player_list, deck);
}

// deal card
Card *deal(Card *deck[])
{
	uint8_t idx;
	Card *result = calloc(1, sizeof(Card));
	while (1)
	{
		idx = rand() % 110;
		if (deck[idx] == NULL)
			continue;
		result = deck[idx];
		deck[idx] = NULL;
		return result;
	}
}

void caution()
{
	printf("\033[1;32m");
	printf("1. All selection using number\n");
	printf("2. Better have a reference of all the card & rule beside you\n");
	printf("   because there will be no information for the card nor the rule\n");
	printf("3. You can check every player's board & your hand at the beginning of the round and when you're choosing a role\n");
	printf("\033[0m");
}

// init player
void set_player(Player *player_list[], Card *deck[])
{
	printf("Setting Card...");
	for (size_t i = 0; i < 4; i++)
		player_list[i] = calloc(1, sizeof(Player));

	// init 4 player status
	for (size_t i = 0; i < 4; i++)
	{
		player_list[i]->id = i + 1;
		player_list[i]->cards = 4;
		player_list[i]->builds = 1;
		player_list[i]->points = 1;
		player_list[i]->is_bot = true;
		player_list[i]->board[0] = deck[i];
		if (player_list[i]->id == 1)
			player_list[i]->is_bot = false;
		deck[i] = NULL;
	}

	// deal cards
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
			player_list[i]->hand[j] = deal(deck);
	}
	sleep(1);
	printf("done\n");
}

void set_card(Card *deck[])
{
	printf("Dealing Card...");
	for (size_t i = 0; i < 110; i++)
		deck[i] = calloc(1, sizeof(Card));

	// init 110 cards
	uint32_t idx = 0;
	for (idx; idx < 10; idx++)
	{
		deck[idx]->name = "Indigo plant";
		deck[idx]->cost = 1;
		deck[idx]->points = 1;
		deck[idx]->color = BLUE;
		deck[idx]->is_special_struct = false;
	}
	for (idx; idx < 18; idx++)
	{
		deck[idx]->name = "Sugar mill";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = WHITE;
		deck[idx]->is_special_struct = false;
	}
	for (idx; idx < 26; idx++)
	{
		deck[idx]->name = "Tobacco storage";
		deck[idx]->cost = 3;
		deck[idx]->points = 2;
		deck[idx]->color = BROWN;
		deck[idx]->is_special_struct = false;
	}
	for (idx; idx < 34; idx++)
	{
		deck[idx]->name = "Coffee roaster";
		deck[idx]->cost = 4;
		deck[idx]->points = 2;
		deck[idx]->color = COFFEE;
		deck[idx]->is_special_struct = false;
	}
	for (idx; idx < 42; idx++)
	{
		deck[idx]->name = "Silver smelter";
		deck[idx]->cost = 5;
		deck[idx]->points = 3;
		deck[idx]->color = GRAY;
		deck[idx]->is_special_struct = false;
	}
	for (idx; idx < 45; idx++)
	{
		deck[idx]->name = "Tower";
		deck[idx]->cost = 3;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 48; idx++)
	{
		deck[idx]->name = "Chapel";
		deck[idx]->cost = 3;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 51; idx++)
	{
		deck[idx]->name = "Smithy";
		deck[idx]->cost = 1;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 54; idx++)
	{
		deck[idx]->name = "Poor house";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 57; idx++)
	{
		deck[idx]->name = "Black market";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 60; idx++)
	{
		deck[idx]->name = "Crane";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 63; idx++)
	{
		deck[idx]->name = "Carpenter";
		deck[idx]->cost = 3;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 66; idx++)
	{
		deck[idx]->name = "Quarry";
		deck[idx]->cost = 4;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 69; idx++)
	{
		deck[idx]->name = "Well";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 72; idx++)
	{
		deck[idx]->name = "Aqueduct";
		deck[idx]->cost = 3;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 75; idx++)
	{
		deck[idx]->name = "Market stand";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 78; idx++)
	{
		deck[idx]->name = "Market hall";
		deck[idx]->cost = 4;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 81; idx++)
	{
		deck[idx]->name = "Trading post";
		deck[idx]->cost = 2;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 84; idx++)
	{
		deck[idx]->name = "Archive";
		deck[idx]->cost = 1;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 87; idx++)
	{
		deck[idx]->name = "Prefecture";
		deck[idx]->cost = 3;
		deck[idx]->points = 2;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 90; idx++)
	{
		deck[idx]->name = "Gold mine";
		deck[idx]->cost = 1;
		deck[idx]->points = 1;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 93; idx++)
	{
		deck[idx]->name = "Library";
		deck[idx]->cost = 5;
		deck[idx]->points = 3;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 96; idx++)
	{
		deck[idx]->name = "Statue";
		deck[idx]->cost = 3;
		deck[idx]->points = 3;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 99; idx++)
	{
		deck[idx]->name = "Victory column";
		deck[idx]->cost = 4;
		deck[idx]->points = 4;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 102; idx++)
	{
		deck[idx]->name = "Hero";
		deck[idx]->cost = 5;
		deck[idx]->points = 5;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 104; idx++)
	{
		deck[idx]->name = "Guild hall";
		deck[idx]->cost = 6;
		deck[idx]->points = 0;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 106; idx++)
	{
		deck[idx]->name = "City hall";
		deck[idx]->cost = 6;
		deck[idx]->points = 0;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 108; idx++)
	{
		deck[idx]->name = "Triumhal arch";
		deck[idx]->cost = 6;
		deck[idx]->points = 0;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	for (idx; idx < 110; idx++)
	{
		deck[idx]->name = "Palace";
		deck[idx]->cost = 6;
		deck[idx]->points = 0;
		deck[idx]->color = PURPLE;
		deck[idx]->is_special_struct = true;
	}
	sleep(1);
	printf("done\n");
}