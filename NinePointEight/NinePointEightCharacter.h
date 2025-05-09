// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NinePointEightCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ANinePointEightCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelfCamLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BDCamLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RestartAction;

public:
	ANinePointEightCharacter();
	
//Inputs
protected:

	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);

	void Jump() override;
	

	//Zoom
	UFUNCTION(BlueprintCallable)
	void zoomIn();

	UFUNCTION(BlueprintCallable)
	void zoomOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* zoomTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* zoomFloatCurve;

	UFUNCTION()
	void zoomTimelineUpdate(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom", meta = (AllowPrivateAccess = "true"))
	float zoomTargetArmLength;

	UFUNCTION(BlueprintPure)
	float getCamArmLength();


	UFUNCTION(BlueprintCallable)
	void setCamArmLength(float newArmLength);
	
	
	//Cam Work
	UFUNCTION(BlueprintCallable)
	void lockSelfCam() { if (bCanUnlockCamera) bIsSelfCameraLocked = true; }

	UFUNCTION(BlueprintCallable)
	void unlockSelfCam() { if (bCanUnlockCamera) bIsSelfCameraLocked = false; }

	UFUNCTION(BlueprintCallable)
	void lockBDCam() { if (bCanUnlockCamera) bIsBodyDoubleCameraLocked = true; }

	UFUNCTION(BlueprintCallable)
	void unlockBDCam() { if (bCanUnlockCamera) bIsBodyDoubleCameraLocked = false; }

	UFUNCTION(BlueprintCallable)
	void disableSplitScreen();

	bool bCanUnlockCamera = true;

	UFUNCTION(BlueprintCallable)
	void setCanLockCamera(bool bCanUnlockCamera_) { bCanUnlockCamera = bCanUnlockCamera_; }

	UFUNCTION()
	void restartLevel();

public:
	//Level9 Setup
	UFUNCTION(BlueprintCallable)
	void setupMoon(float camAngle);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


//Utilities
public:
	/*UFUNCTION(BlueprintCallable)
	void disablePlayerInput();

	UFUNCTION(BlueprintCallable)
	void enablePlayerInput();*/

	UFUNCTION(BlueprintCallable)
	void lookForBodyDouble();

	UFUNCTION(BlueprintCallable)
	void setGForce(float newG);


public:
	UFUNCTION(BlueprintCallable)
	void setIsCameraShifting(bool isShifting);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bIsCameraShifting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bIsSelfCameraLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bIsBodyDoubleCameraLocked = false;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller", meta = (ExposeOnSpawn = true))
	int32 controllerIndex;

	UPROPERTY(BlueprintReadWrite)
	ANinePointEightCharacter* bodyDouble;

protected:

	class UNPEGameInstance* npeGameInsRef;

};

