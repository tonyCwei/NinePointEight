// Fill out your copyright notice in the Description page of Project Settings.


#include "NPEGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NPESaveGame.h"
#include "Blueprint/UserWidget.h"

UNPEGameInstance::UNPEGameInstance()
{
    NPESaveGame = Cast<UNPESaveGame>(UGameplayStatics::LoadGameFromSlot("SavingsSlot", 0));
    if (!NPESaveGame)
    {
       // UE_LOG(LogTemp, Warning, TEXT("CreatingSave"));
        NPESaveGame = Cast<UNPESaveGame>(UGameplayStatics::CreateSaveGameObject(UNPESaveGame::StaticClass()));
        if (NPESaveGame) {
            UGameplayStatics::SaveGameToSlot(NPESaveGame, "SavingsSlot", 0);          
        }

        
    }


   /* if (NPESaveGame) {
        curLevel = NPESaveGame->curLevel;
    }*/
}

void UNPEGameInstance::activatePlatform()
{
    activatedPlatforms++;
    LoadNextLevel();

    //UE_LOG(LogTemp, Display, TEXT("%d activatedPlatforms"), activatedPlatforms);
}

void UNPEGameInstance::deactivatePlatform()
{
    activatedPlatforms--;

    //UE_LOG(LogTemp, Display, TEXT("%d activatedPlatforms"), activatedPlatforms);
}

void UNPEGameInstance::LoadNextLevel()
{
   // UE_LOG(LogTemp, Display, TEXT("%d activatedPlatforms"), activatedPlatforms);
    if (activatedPlatforms != LevelInfos[curLevel].platformsNeeded) {
        UE_LOG(LogTemp, Display, TEXT("activatedPlatforms %d"), activatedPlatforms);
        UE_LOG(LogTemp, Display, TEXT("platformsNeeded %d"), LevelInfos[curLevel].platformsNeeded);
        return;
    }

    if (LevelInfos.Num() == NPESaveGame->unlockedLevels.Num() && curLevel + 1 < LevelInfos.Num()) {
        curLevel++;
        //NPESaveGame->curLevel = curLevel;
        NPESaveGame->unlockedLevels[curLevel] = true;

        UClass* LevelEndWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/Blueprint/Widget/WBP_LevelEnd.WBP_LevelEnd_C"));

        if (LevelEndWidgetClass)
        {
            UUserWidget* LevelEndWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LevelEndWidgetClass);
            if (LevelEndWidgetInstance)
            {
                LevelEndWidgetInstance->AddToViewport();
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load WBP_LevelEnd widget!"));
        }

        
        //UGameplayStatics::OpenLevel(this, FName(TEXT("TransitionLevel")));


        /*UE_LOG(LogTemp, Warning, TEXT("Next Level Name: %s, Planet: %s, Gravity Force: %.2f, Gravity Acceleration: %.2f"),
            *LevelInfos[curLevel].levelName.ToString(), *LevelInfos[curLevel].planetName, LevelInfos[curLevel].gForce, LevelInfos[curLevel].gAcce);

        for (int32 i = 0; i < NPESaveGame->unlockedLevels.Num(); ++i)
        {
            bool isLevelUnlocked = NPESaveGame->unlockedLevels[i];
            UE_LOG(LogTemp, Warning, TEXT("Level %d unlocked: %s"), i, isLevelUnlocked ? TEXT("True") : TEXT("False"));
        }*/

    }
    else { //Finish Last Level
        UE_LOG(LogTemp, Display, TEXT("Load GameOver"));
    
    }

    
}

void UNPEGameInstance::RestartLevel()
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 1))
    {
        UGameplayStatics::RemovePlayer(PlayerController, false);
    }

    UGameplayStatics::OpenLevel(GetWorld(), LevelInfos[curLevel].levelName);


}
