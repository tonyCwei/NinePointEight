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

	int totalLevels = 10;

	UPROPERTY()//Must Mark as UPROPERTY(), otherwise SaveGameToSlot() won't save this data
	TArray<bool>unlockedLevels;

	UFUNCTION(BlueprintCallable)
	int32 getunlockedLevelsSize() { return unlockedLevels.Num(); }

	UFUNCTION(BlueprintCallable)
	void unlockLevel(int32 levelIndex) { if (levelIndex < unlockedLevels.Num()) unlockedLevels[levelIndex] = true; }

	UFUNCTION(BlueprintCallable)
	bool getUnlockedLevels(int32 levelIndex) { return (levelIndex < unlockedLevels.Num()) ? unlockedLevels[levelIndex] : false; }


//User Settings
public:

	int32 ScalabilityLevel = 3;
	
};
