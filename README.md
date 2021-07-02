# README

A C program for my final project in programming-ii 2021

## Introduction

San Juan is a board game base on another board game call Puerto Rico, and this project is the C version of San Juan.

It's a cyber board game **without UI**, only text performance with instruction for player to follow. So if you're not familiar with the game, better place a playbook next to you.

## How to play

* The game consist of 1 player and 3 bot.
* The player is always Player 1, the rest of them are bot.
* All the selection in the game is done by entering number taged in the instruction, so don't enter things other than nunmber.
* You're allow to check other player's board(including yours) at the start of every round, you will also see your hand card and board every time you pick a role.
* Player can choose level at the start of the game(default is 1).
* There will be a 1 sec sleep for every player move to create the atmosphere that the player is thinking.
* [rule reference](https://www.fgbradleys.com/rules/rules4/San%20Juan%20-%20rules.pdf)

## compile & run

```shell
$ make
$ ./main
```



