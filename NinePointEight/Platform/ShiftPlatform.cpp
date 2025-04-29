// Fill out your copyright notice in the Description page of Project Settings.


#include "ShiftPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "NinePointEight/NinePointEightCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


AShiftPlatform::AShiftPlatform()
{
	inversePlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InversePlatformMesh"));
	inversePlatformMesh->SetupAttachment(RootComponent);
}

void AShiftPlatform::BeginPlay()
{
	Super::BeginPlay();

	platformMesh->OnComponentHit.AddDynamic(this, &AShiftPlatform::OnPlatformHit);
	inversePlatformMesh->OnComponentHit.AddDynamic(this, &AShiftPlatform::OnInversePlatformHit);

}

void AShiftPlatform::OnPlatformHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (ANinePointEightCharacter* playerRef = Cast<ANinePointEightCharacter>(OtherActor)) {
			FVector targetGDirection = -GetActorUpVector();
			if (playerRef->GetCharacterMovement() && !playerRef->GetCharacterMovement()->GetGravityDirection().Equals(targetGDirection, 0.01)) {
				playerRef->GetCharacterMovement()->SetGravityDirection(targetGDirection);
				playerRef->setIsCameraShifting(true);
			}
		}
	}
}

void AShiftPlatform::OnInversePlatformHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (ANinePointEightCharacter* playerRef = Cast<ANinePointEightCharacter>(OtherActor)) {
			FVector targetGDirection = GetActorUpVector();
			if (playerRef->GetCharacterMovement() && !playerRef->GetCharacterMovement()->GetGravityDirection().Equals(targetGDirection, 0.01)) {
				playerRef->GetCharacterMovement()->SetGravityDirection(targetGDirection);
				playerRef->setIsCameraShifting(true);
			}
		}
	}
}


