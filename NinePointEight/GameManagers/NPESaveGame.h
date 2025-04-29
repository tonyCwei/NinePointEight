// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NPESaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NINEPOINTEIGHT_API UNPESaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UNPESaveGame();


public:

	//int32 curLevel = 0;

	int totalLevels = 10;

	TArray<bool>unlockedLevels;

	UFUNCTION(BlueprintCallable)
	int32 getunlockedLevelsSize() { return unlockedLevels.Num(); }
	
};
