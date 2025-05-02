// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitPlatform.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NinePointEight/NinePointEightCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NinePointEight/GameManagers/NPEGameInstance.h"
#include "Sound/SoundCue.h"

AExitPlatform::AExitPlatform()
{
	triggerArea = CreateDefaultSubobject<UBoxComponent>("TriggerArea");
	triggerArea->SetupAttachment(platformMesh);
}

void AExitPlatform::BeginPlay()
{
	Super::BeginPlay();

	triggerArea->OnComponentBeginOverlap.AddDynamic(this, &AExitPlatform::OnBeginOverlap);
	triggerArea->OnComponentEndOverlap.AddDynamic(this, &AExitPlatform::OnEndOverlap);

	ogMat = platformMesh->GetMaterial(0);

}

void AExitPlatform::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) {
		numPeopleOnPlatform++;
		if (ANinePointEightCharacter* playerRef = Cast<ANinePointEightCharacter>(OtherActor)) {
			if (playerRef->GetCharacterMovement() && playerRef->GetCharacterMovement()->GetGravityDirection().Equals(-GetActorUpVector(), 0.01)) {
				//UE_LOG(LogTemp, Display, TEXT("Light UP"));
				if (litMat) platformMesh->SetMaterial(0, litMat);
				activatePlatform();
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("numPeopleOnPlatform %d"), numPeopleOnPlatform);
}

void AExitPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player")) {
		//UE_LOG(LogTemp, Display, TEXT("Reset material"));
		numPeopleOnPlatform--;
		if (numPeopleOnPlatform == 0) {
			platformMesh->SetMaterial(0, ogMat);
			deactivatePlatform();
		}
	}

	UE_LOG(LogTemp, Display, TEXT("numPeopleOnPlatform %d"), numPeopleOnPlatform);
}

void AExitPlatform::activatePlatform()
{
	if (isActivated) return;

	if (UNPEGameInstance* npeGameInsRef = Cast<UNPEGameInstance>(GetWorld()->GetGameInstance())) {
		npeGameInsRef->activatePlatform();
		isActivated = true;


		if (activatedCue)
		{
			UGameplayStatics::PlaySound2D(this, activatedCue);
		}
	}
}

void AExitPlatform::deactivatePlatform()
{
	if (!isActivated) return;
	if (UNPEGameInstance* npeGameInsRef = Cast<UNPEGameInstance>(GetWorld()->GetGameInstance())) {
		npeGameInsRef->deactivatePlatform();
		isActivated = false;
	}
}
