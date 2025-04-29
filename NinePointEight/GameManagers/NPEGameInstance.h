// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NPEGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName levelName;

	UPROPERTY(BlueprintReadOnly)
	FString planetName;

	UPROPERTY(BlueprintReadOnly)
	float gForce;

	UPROPERTY(BlueprintReadOnly)
	float gAcce;

	UPROPERTY(BlueprintReadOnly)
	int32 platformsNeeded;
};


UCLASS()
class NINEPOINTEIGHT_API UNPEGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UNPEGameInstance();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNPESaveGame* NPESaveGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 curLevel = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLevelInfo> LevelInfos = {
										FLevelInfo{TEXT("Level0"), TEXT("Earth"), 1.0f, 9.8f, 1}, //Tutoril, wsad, space, shift
										FLevelInfo{TEXT("Level1"), TEXT("Earth"), 1.0f, 9.8f, 1},//turn around(isnt that easy), but have to have the same g direction; red platform intro, double sided intro
										FLevelInfo{TEXT("Level2"), TEXT("Mercury"), 0.378f, 3.7f, 1},
										FLevelInfo{TEXT("Level3"), TEXT("Venus"), 0.907f, 8.9f, 2},
										FLevelInfo{TEXT("Level4"), TEXT("Mars"), 0.377f, 3.7f, 2},
										FLevelInfo{TEXT("Level5"), TEXT("Jupiter"), 2.36f, 23.1f, 1},
										FLevelInfo{TEXT("Level6"), TEXT("Saturn"), 0.916f, 9.0f, 1},
										FLevelInfo{TEXT("Level7"), TEXT("Uranus"), 0.889f, 8.7f, 1},
										FLevelInfo{TEXT("Level8"), TEXT("Neptune"), 1.12f, 11.0f, 1},
										FLevelInfo{TEXT("Level9"), TEXT("Moon"), 0.166f, 1.6f, 2}

	};


//Utilities
	UFUNCTION(BlueprintCallable)
	FLevelInfo getLevelInfo(int32 levelIndex) { return LevelInfos[levelIndex]; }


protected:

	int32 activatedPlatforms = 0;

public:
	void activatePlatform();

	void deactivatePlatform();


//levels
public:
	UFUNCTION()
	void LoadNextLevel();


	UFUNCTION(BlueprintCallable)
	void RestartLevel();
};
