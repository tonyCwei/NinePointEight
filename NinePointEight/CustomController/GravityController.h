// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GravityController.generated.h"

/**
 * 
 */
UCLASS()
class NINEPOINTEIGHT_API AGravityController : public APlayerController
{
	GENERATED_BODY()




public:

	virtual void UpdateRotation(float DeltaTime) override;

	// Converts a rotation from world space to gravity relative space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);

	// Converts a rotation from gravity relative space to world space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);

private:
	FVector LastFrameGravity = FVector::ZeroVector;
	

//Utilities
protected:
	class ANinePointEightCharacter* getOwnerCharacter();


//Camera Shifting when gravity changes
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsCameraShifting = false;

	FRotator preShiftRotation;


	

public:
	UFUNCTION(BlueprintCallable)
	void setIsCameraShifting(bool isShifting);


//For body double
//public:
//	UPROPERTY(BlueprintReadWrite)
//	class ANinePointEightCharacter* bodyDouble;
};
