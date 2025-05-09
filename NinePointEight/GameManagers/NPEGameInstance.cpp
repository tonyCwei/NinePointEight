// Fill out your copyright notice in the Description page of Project Settings.


#include "NPEGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NPESaveGame.h"
#include "Blueprint/UserWidget.h"
#include "Scalability.h"
#include "Components/AudioComponent.h"

UNPEGameInstance::UNPEGameInstance()
{
    NPESaveGame = Cast<UNPESaveGame>(UGameplayStatics::LoadGameFromSlot("SavingsSlot", 0));
    if (!NPESaveGame)
    {



        NPESaveGame = Cast<UNPESaveGame>(UGameplayStatics::CreateSaveGameObject(UNPESaveGame::StaticClass()));
        if (NPESaveGame) {
            UGameplayStatics::SaveGameToSlot(NPESaveGame, "SavingsSlot", 0);
        }


    }


    //unlock all levels
   /* for (int i = 0; i < 10; i++) {
        NPESaveGame->unlockLevel(i);
    }
    */



    static ConstructorHelpers::FClassFinder<UUserWidget> LevelEndWidgetClassFinder(TEXT("/Game/Blueprint/Widget/WBP_LevelEnd"));
    if (LevelEndWidgetClassFinder.Succeeded())
    {
        LevelEndWidgetClass = LevelEndWidgetClassFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> MoonLevelEndWidgetClassFinder(TEXT("/Game/Blueprint/Widget/WBP_MoonLevelEnd"));
    if (MoonLevelEndWidgetClassFinder.Succeeded())
    {
        MoonLevelEndWidgetClass = MoonLevelEndWidgetClassFinder.Class;
    }

    /*BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
    BGMAudioComponent->bAutoActivate = false;
    BGMAudioComponent->RegisterComponent();*/
}

void UNPEGameInstance::Init()
{
    Super::Init();

    applyUserSettings();
}

UNPESaveGame* UNPEGameInstance::getSaveGame()
{
    return NPESaveGame;
}

bool UNPEGameInstance::getIsLevelUnlocked(int32 levelIndex)
{
    
    if (NPESaveGame && levelIndex < NPESaveGame->getunlockedLevelsSize()) {
        return NPESaveGame->unlockedLevels[levelIndex];
    }
 
    return false;
}

void UNPEGameInstance::unlockLevel(int32 levelIndex)
{
    NPESaveGame->unlockLevel(levelIndex);
    UGameplayStatics::SaveGameToSlot(NPESaveGame, "SavingsSlot", 0);
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
        unlockLevel(curLevel);

        if (LevelEndWidgetClass){
            UUserWidget* LevelEndWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LevelEndWidgetClass);
            if (LevelEndWidgetInstance)
            {
                LevelEndWidgetInstance->AddToViewport(2);
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load WBP_LevelEnd widget!"));
        }

        
        /*UE_LOG(LogTemp, Warning, TEXT("Next Level Name: %s, Planet: %s, Gravity Force: %.2f, Gravity Acceleration: %.2f"),
            *LevelInfos[curLevel].levelName.ToString(), *LevelInfos[curLevel].planetName, LevelInfos[curLevel].gForce, LevelInfos[curLevel].gAcce);

        for (int32 i = 0; i < NPESaveGame->unlockedLevels.Num(); ++i)
        {
            bool isLevelUnlocked = NPESaveGame->unlockedLevels[i];
            UE_LOG(LogTemp, Warning, TEXT("Level %d unlocked: %s"), i, isLevelUnlocked ? TEXT("True") : TEXT("False"));
        }*/

    }
    else if (curLevel == 9){ //Finish Last Level
        if (MoonLevelEndWidgetClass) {
            UUserWidget* MoonLevelEndWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MoonLevelEndWidgetClass);
            if (MoonLevelEndWidgetInstance)
            {
                curLevel++;
                MoonLevelEndWidgetInstance->AddToViewport(2);
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load WBP_LevelEnd widget!"));
        }
    
    }

    restartTimes = 0;
}

void UNPEGameInstance::RestartLevel()
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 1))
    {
        UGameplayStatics::RemovePlayer(PlayerController, true);
    }

    if (UGameViewportClient* Viewport = GetWorld()->GetGameViewport())
    {
        Viewport->SetForceDisableSplitscreen(false);
    }



    UGameplayStatics::OpenLevel(GetWorld(), LevelInfos[curLevel].levelName);

    restartTimes++;

    
}

void UNPEGameInstance::applyUserSettings()
{
    if (NPESaveGame) {
        SetOverallScalabilityLevel(NPESaveGame->ScalabilityLevel);
    }
    else {
        SetOverallScalabilityLevel(3);
    }
}

void UNPEGameInstance::SetOverallScalabilityLevel(int32 Level)
{
    Level = FMath::Clamp(Level, 0, 3);

    Scalability::FQualityLevels QualityLevels;
    QualityLevels.SetFromSingleQualityLevel(Level);

    Scalability::SetQualityLevels(QualityLevels);
}

void UNPEGameInstance::PlayBGM(UAudioComponent* newAudioComponent, float FadeInDuration, float Volume)
{
    if (!newAudioComponent) return;
    
    if (!BGMAudioComponent) {
        BGMAudioComponent = newAudioComponent;
        BGMAudioComponent->FadeIn(FadeInDuration, Volume);
    
    }

    if (BGMAudioComponent->GetSound() != newAudioComponent->GetSound()) { //Different BGM
        StopBGM(FadeInDuration);
        BGMAudioComponent = newAudioComponent;
    }
 




}

void UNPEGameInstance::StopBGM(float FadeOutDuration)
{
    if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
    {
        BGMAudioComponent->FadeOut(FadeOutDuration, 0.0f);
    }
    //CurrentBGM = nullptr;
}

void UNPEGameInstance::PauseBGM()
{
    if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
    {
        BGMAudioComponent->SetPaused(true);
    }
}

void UNPEGameInstance::ResumeBGM()
{
    if (BGMAudioComponent && BGMAudioComponent->bIsPaused)
    {
        BGMAudioComponent->SetPaused(false);
    }
}

void UNPEGameInstance::setVolume(float newVolume)
{
    if (BGMAudioComponent) BGMAudioComponent->SetVolumeMultiplier(newVolume);
}
