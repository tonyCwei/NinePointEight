// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinePointEightCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CustomController/GravityController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "GameManagers/NPEGameInstance.h"
#include "GameManagers/NPESaveGame.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ANinePointEightCharacter

ANinePointEightCharacter::ANinePointEightCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Timelines
	zoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
}

void ANinePointEightCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	lookForBodyDouble();

	//Game Instance
	npeGameInsRef = Cast<UNPEGameInstance>(GetWorld()->GetGameInstance());


	//BindTimeline
	if (zoomFloatCurve) {
		FOnTimelineFloat UpdateFunction;
		UpdateFunction.BindUFunction(this, FName("zoomTimelineUpdate"));
		zoomTimeline->AddInterpFloat(zoomFloatCurve, UpdateFunction);

	}


	if (GetController()) {
		FRotator startingRotation(-15, 0, 0);
		GetController()->SetControlRotation(startingRotation);
	}
	

	setGForce(npeGameInsRef->LevelInfos[npeGameInsRef->curLevel].gForce);
	UE_LOG(LogTemp, Warning, TEXT("Game Instance curLevel: %d"), npeGameInsRef->curLevel);
	UE_LOG(LogTemp, Warning, TEXT("Gravity: %f"), npeGameInsRef->LevelInfos[npeGameInsRef->curLevel].gForce);
	//Bug
	//UE_LOG(LogTemp, Warning, TEXT("Game Instance curLevel: %d"), npeGameInsRef->curLevel);
	//UE_LOG(LogTemp, Warning, TEXT("Game Save curLevel: %d"), npeGameInsRef->NPESaveGame->curLevel);

}




//////////////////////////////////////////////////////////////////////////
// Input



void ANinePointEightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);

		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANinePointEightCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANinePointEightCharacter::Look);

		// Zooming
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ANinePointEightCharacter::zoomOut);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ANinePointEightCharacter::zoomIn);

		// Cam Lock
		EnhancedInputComponent->BindAction(SelfCamLockAction, ETriggerEvent::Started, this, &ANinePointEightCharacter::lockSelfCam);
		EnhancedInputComponent->BindAction(SelfCamLockAction, ETriggerEvent::Completed, this, &ANinePointEightCharacter::unlockSelfCam);

		EnhancedInputComponent->BindAction(BDCamLockAction, ETriggerEvent::Started, this, &ANinePointEightCharacter::lockBDCam);
		EnhancedInputComponent->BindAction(BDCamLockAction, ETriggerEvent::Completed, this, &ANinePointEightCharacter::unlockBDCam);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void ANinePointEightCharacter::Move(const FInputActionValue& Value)
{
	if (bodyDouble) bodyDouble->Move(Value);
	
	if (bIsCameraShifting) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
	
			//const FRotator Rotation = Controller->GetControlRotation();
			//const FRotator YawRotation(0, Rotation.Yaw, 0);

			////// get forward vector
			//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			////// get right vector 
			//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			//// add movement 
			//AddMovementInput(ForwardDirection, MovementVector.Y);
			//AddMovementInput(RightDirection, MovementVector.X);

			FRotator controlRotation = Controller->GetControlRotation();
			FVector gravityDirection = GetCharacterMovement()->GetGravityDirection();

			FRotator gravityRelativeRotation = AGravityController::GetGravityRelativeRotation(controlRotation, gravityDirection);


			//For Left/Right
			FRotator LRWorldRotation = AGravityController::GetGravityWorldRotation(FRotator(0, gravityRelativeRotation.Yaw, gravityRelativeRotation.Roll), gravityDirection);
			FVector RightDirection = FRotationMatrix(LRWorldRotation).GetUnitAxis(EAxis::Y);

			//For FW/BW
			FRotator FBWorldRotation = AGravityController::GetGravityWorldRotation(FRotator(0, gravityRelativeRotation.Yaw, 0), gravityDirection);
			FVector ForwardDirection = FRotationMatrix(FBWorldRotation).GetUnitAxis(EAxis::X);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);

			
	}
	
}

void ANinePointEightCharacter::Look(const FInputActionValue& Value)
{
	if (bodyDouble && !bIsBodyDoubleCameraLocked) bodyDouble->Look(Value);


	if (bIsCameraShifting || bIsSelfCameraLocked) return;

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANinePointEightCharacter::Jump()
{
	if (bodyDouble) bodyDouble->Jump();
	if (bIsCameraShifting) return;
	Super::Jump();
	
}

void ANinePointEightCharacter::zoomTimelineUpdate(float Value)
{
	float newArmLength = FMath::Lerp(500, zoomTargetArmLength, Value);
	CameraBoom->TargetArmLength = newArmLength;

	if (bodyDouble) {
		bodyDouble->CameraBoom->TargetArmLength = newArmLength;
	}
}

void ANinePointEightCharacter::zoomIn()
{
	zoomTimeline->Reverse();
}

void ANinePointEightCharacter::zoomOut()
{
	zoomTimeline->Play();
}


//void ANinePointEightCharacter::disablePlayerInput()
//{
//	if (APlayerController* playerController = Cast<APlayerController>(GetController())) {
//		DisableInput(playerController);
//	}
//}
//
//void ANinePointEightCharacter::enablePlayerInput()
//{
//	if (APlayerController* playerController = Cast<APlayerController>(GetController())) {
//		EnableInput(playerController);
//	}
//}

void ANinePointEightCharacter::lookForBodyDouble()
{

	if (controllerIndex == 1) {
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANinePointEightCharacter::StaticClass(), foundActors);
		for (AActor* foundActor : foundActors) {
			//UE_LOG(LogTemp, Warning, TEXT("looing for bodyDoubleFound"));
			if (ANinePointEightCharacter* foundCharacter = Cast<ANinePointEightCharacter>(foundActor)) {
				if (foundCharacter->controllerIndex == 0) {
					foundCharacter->bodyDouble = this;
					//UE_LOG(LogTemp, Warning, TEXT("bodyDoubleFound"));
				}
			}

		}
	}
}

void ANinePointEightCharacter::setGForce(float newG)
{
	GetCharacterMovement()->GravityScale = newG;

	if (bodyDouble) {
		bodyDouble->setGForce(newG);
	}
}



void ANinePointEightCharacter::setIsCameraShifting(bool isShifting)
{

	bIsCameraShifting = isShifting;
	if (APlayerController* playerControllerRef = Cast<APlayerController>(GetController())) {
		if (AGravityController* gravityControllerRef = Cast<AGravityController>(playerControllerRef)) {
			gravityControllerRef->setIsCameraShifting(isShifting);
		}
	}
}



