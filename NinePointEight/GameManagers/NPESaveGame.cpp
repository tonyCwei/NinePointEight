// Fill out your copyright notice in the Description page of Project Settings.


#include "NPESaveGame.h"

UNPESaveGame::UNPESaveGame()
{
	unlockedLevels.Init(false, totalLevels);
	unlockedLevels[0] = true;
}
