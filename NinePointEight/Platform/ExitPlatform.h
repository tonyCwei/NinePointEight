// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "ExitPlatform.generated.h"

/**
 * 
 */
UCLASS()
class NINEPOINTEIGHT_API AExitPlatform : public ABasePlatform
{
	GENERATED_BODY()
	

public:
	AExitPlatform();


protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* triggerArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundCue* activatedCue;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UMaterialInterface* ogMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* litMat;

	int32 numPeopleOnPlatform = 0;

	bool isActivated = false;

	void activatePlatform();

	void deactivatePlatform();

};
