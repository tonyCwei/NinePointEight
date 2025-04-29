// Fill out your copyright notice in the Description page of Project Settings.


#include "RedPlatform.h"
#include "NinePointEight/GameManagers/NPEGameInstance.h"

void ARedPlatform::BeginPlay()
{
	Super::BeginPlay();

	platformMesh->OnComponentHit.AddDynamic(this, &ARedPlatform::OnPlatformHit);
}

void ARedPlatform::OnPlatformHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UNPEGameInstance* npeGameInsRef = Cast<UNPEGameInstance>(GetWorld()->GetGameInstance())) {
		npeGameInsRef->RestartLevel();
	}
}
