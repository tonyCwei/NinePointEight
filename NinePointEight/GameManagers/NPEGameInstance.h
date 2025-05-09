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

	virtual void Init() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNPESaveGame* NPESaveGame;

	UFUNCTION(BlueprintCallable)
	class UNPESaveGame* getSaveGame();


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
										FLevelInfo{TEXT("Level6"), TEXT("Saturn"), 0.916f, 9.0f, 2},
										FLevelInfo{TEXT("Level7"), TEXT("Uranus"), 0.889f, 8.7f, 1},
										FLevelInfo{TEXT("Level8"), TEXT("Neptune"), 1.12f, 11.0f, 2},
										FLevelInfo{TEXT("Level9"), TEXT("Moon"), 0.166f, 1.6f, 2}

	};


//Utilities
	UFUNCTION(BlueprintCallable)
	FLevelInfo getLevelInfo(int32 levelIndex) { return LevelInfos[levelIndex]; }

	UFUNCTION(BlueprintCallable)
	bool getIsLevelUnlocked(int32 levelIndex);

	UFUNCTION(BlueprintCallable)
	void unlockLevel(int32 levelIndex);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 restartTimes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldDisplayTip = true;

protected:

	int32 activatedPlatforms = 0;

	UClass* LevelEndWidgetClass;

	UClass* MoonLevelEndWidgetClass;

public:
	void activatePlatform();

	void deactivatePlatform();


//levels
public:
	UFUNCTION(BlueprintCallable)
	void LoadNextLevel();


	UFUNCTION(BlueprintCallable)
	void RestartLevel();

//UserSettings
public:
	UFUNCTION(BlueprintCallable)
	void applyUserSettings();


protected:
	void SetOverallScalabilityLevel(int32 Level);



//Audio

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BGM")
	class UAudioComponent* BGMAudioComponent;

	//UPROPERTY(BlueprintReadOnly)
	//USoundBase* CurrentBGM;




public:


	UFUNCTION(BlueprintCallable)
	void PlayBGM(UAudioComponent* newAudioComponent, float FadeInDuration = 1.0f, float Volume = 1.0f);

	UFUNCTION(BlueprintCallable)
	void StopBGM(float FadeOutDuration = 1.0f);

	UFUNCTION(BlueprintCallable)
	void PauseBGM();

	UFUNCTION(BlueprintCallable)
	void ResumeBGM();

	UFUNCTION(BlueprintCallable)
	void setVolume(float newVolume);

	UFUNCTION(BlueprintCallable)
	UAudioComponent* getAudioComponent() { return BGMAudioComponent; };
};
