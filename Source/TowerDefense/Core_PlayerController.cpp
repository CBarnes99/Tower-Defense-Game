#include "Core_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "Core_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Core_GameState.h"
#include "TurretManager.h"
#include "PlayerCharacter.h"

void ACore_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("%s has initilized!"), *this->GetName());

	HaveMappingContextsBeenAsigned();

	enhancedInputSubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (enhancedInputSubSystem)
	{
		enhancedInputSubSystem->AddMappingContext(defaultMappingContext, 0);
		enhancedInputSubSystem->AddMappingContext(movementMappingContext, 0);
		enhancedInputSubSystem->AddMappingContext(gameMenuWidgetsActionsMappingContext, 0);
		enhancedInputSubSystem->AddMappingContext(combatMappingContext, 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ENHANCED INPUT SUBSYSTEM WAS NOT ASSIGNED CORRECTLY!!!!"));
	}

	AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
	coreGameState = Cast<ACore_GameState>(gameState);
	if (!coreGameState)
	{
		UE_LOG(LogTemp, Error, TEXT("NO GAME STATE FOUND IN PLAYER CONTROLLER!!!"));
	}

	hotbarSelectionIndex = 0;
	previousHotbarSelectionIndex = 0;
}

void ACore_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//Sets myPlayerCharacter to the possesed player
	myPlayerCharacter = Cast<APlayerCharacter>(InPawn);
	if (!myPlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Character Not Assigned In Player Controller!"));
	}

	coreHUD = Cast<ACore_HUD>(GetHUD());
	if (!coreHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("Core Hud Not Assigned In Player Controller!"));
	}
}

void ACore_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Binds the Input Actions assigned in the editor to the corisponding functions
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(moveActionInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::MovementAction);

		Input->BindAction(lookActionInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::MouseLookAction);

		Input->BindAction(jumpActionInput, ETriggerEvent::Started, this, &ACore_PlayerController::JumpAction);
		Input->BindAction(jumpActionInput, ETriggerEvent::Completed, this, &ACore_PlayerController::StopJumpingAction);

		Input->BindAction(runActionInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::RunningAction);
		Input->BindAction(runActionInput, ETriggerEvent::Completed, this, &ACore_PlayerController::RunningActionStop);

		Input->BindAction(startEnemyWaveActionInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::CallGameModeToStartSpawningEnemies);

		Input->BindAction(attackActionInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::AttackAction);

		Input->BindAction(scrollWheelSelectionInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::ScrollWheelSelectionAction);

		Input->BindAction(confirmTurretPlacementInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::ConfirmTurretPlacementAction);

		Input->BindAction(cancelTurretPlacement, ETriggerEvent::Triggered, this, &ACore_PlayerController::CancelTurretPlacementAction);

		Input->BindAction(rotateTurretRightInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::RotateTurret);
		Input->BindAction(rotateTurretLeftInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::RotateTurret);

		Input->BindAction(openTurretSelectionMenu, ETriggerEvent::Triggered, this, &ACore_PlayerController::OpenTurretSelectionMenu);
	}
}

void ACore_PlayerController::MovementAction(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("WSAD"));
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector Forward = myPlayerCharacter->GetActorForwardVector();
	const FVector Right = myPlayerCharacter->GetActorRightVector();

	myPlayerCharacter->AddMovementInput(Forward, MovementVector.Y);
	myPlayerCharacter->AddMovementInput(Right, MovementVector.X);

}

void ACore_PlayerController::MouseLookAction(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Mouse"));
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	myPlayerCharacter->AddControllerPitchInput(-LookAxisVector.Y);
	myPlayerCharacter->AddControllerYawInput(LookAxisVector.X);


	if (enhancedInputSubSystem->HasMappingContext(turretPlacingMappingContext))
	{
		turretManager->UpdateTurretPlacementLocation(myPlayerCharacter->GetCameraLocation(), myPlayerCharacter->GetCameraForwardVector());
	}
}

void ACore_PlayerController::RunningAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shift"));
	myPlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = myPlayerCharacter->GetRunSpeed();
}

void ACore_PlayerController::RunningActionStop()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shift"));
	myPlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = myPlayerCharacter->GetMovementSpeed();
}

void ACore_PlayerController::JumpAction()
{
	myPlayerCharacter->Jump();
}

void ACore_PlayerController::StopJumpingAction()
{
	myPlayerCharacter->StopJumping();
}

void ACore_PlayerController::AttackAction()
{
	myPlayerCharacter->AttackAction();
}

void ACore_PlayerController::CallGameModeToStartSpawningEnemies()
{
	UE_LOG(LogTemp, Display, TEXT("ENTER PRESSED"));
	StartWaveEvent.Broadcast();
};

void ACore_PlayerController::OpenTurretSelectionMenu()
{
	UE_LOG(LogTemp, Display, TEXT("OpenTurretButtonPressed"));

	hotbarSelectionIndex = 0;
	UpdateHotbarSelection();

	coreHUD->ToggleTurretSelectionWidget();
	if (coreHUD->GetIsTurretSelectionMenuVisable())
	{
		UpdateMappingContext(combatMappingContext, false, 0);
		UpdateMappingContext(movementMappingContext, false, 0);
	}
	else
	{
		UpdateMappingContext(combatMappingContext, true, 0);
		UpdateMappingContext(movementMappingContext, true, 0);
	}
}

void ACore_PlayerController::ScrollWheelSelectionAction(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Value.ToString());

	if (Value.Get<float>() > 0)
	{
		hotbarSelectionIndex = FMath::Clamp(hotbarSelectionIndex + 1, 0, coreGameState->GetCurrentListSizeInWeaponTurretHud());
	}
	else
	{
		hotbarSelectionIndex = FMath::Clamp(hotbarSelectionIndex - 1, 0, coreGameState->GetCurrentListSizeInWeaponTurretHud());
	}
	UpdateHotbarSelection();
}

void ACore_PlayerController::ConfirmTurretPlacementAction()
{
	if (hotbarSelectionIndex > 0)
	{
		turretManager->ConfirmTurretPlacement();
	}
}

void ACore_PlayerController::CancelTurretPlacementAction()
{
	hotbarSelectionIndex = 0;
	UpdateHotbarSelection();
}

void ACore_PlayerController::RotateTurret(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Value = %s"), *Value.ToString());
	turretManager->RotateTurretPlacement(Value.Get<float>());
}

void ACore_PlayerController::UpdateHotbarSelection()
{
	if (!turretManager)
	{
		AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATurretManager::StaticClass());

		if (!actor)
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't find turret manager"));
			return;
		}
		turretManager = Cast<ATurretManager>(actor);
		if (!turretManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Cast Failed for turret manager in player character!"));
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Hotbar Index is %d"), hotbarSelectionIndex);

	if (hotbarSelectionIndex > 0 && hotbarSelectionIndex != previousHotbarSelectionIndex && GetTurretDAEvent.IsBound())
	{
		UpdateMappingContext(combatMappingContext, false, 0);
		UpdateMappingContext(turretPlacingMappingContext, true, 1);

		previousHotbarSelectionIndex = hotbarSelectionIndex;

		UDA_TurretInfo* turretDA = GetTurretDAEvent.Execute(hotbarSelectionIndex - 1);

		turretManager->StartTurretPlacement(turretDA);
		turretManager->UpdateTurretPlacementLocation(myPlayerCharacter->GetCameraLocation(), myPlayerCharacter->GetCameraForwardVector());

	}
	else if (hotbarSelectionIndex == 0 && turretManager->IsPlacingTurret())
	{
		turretManager->CancelTurretPlacement();
		UpdateMappingContext(combatMappingContext, true, 1);
		UpdateMappingContext(turretPlacingMappingContext, false, 0);

		previousHotbarSelectionIndex = 0;
	}
}
void ACore_PlayerController::UpdateMappingContext(UInputMappingContext* mappingContext, bool addContext, int priority)
{
	if (addContext && !enhancedInputSubSystem->HasMappingContext(mappingContext))
	{
		enhancedInputSubSystem->AddMappingContext(mappingContext, priority);
		return;
	}
	else if (!addContext && enhancedInputSubSystem->HasMappingContext(mappingContext))
	{
		enhancedInputSubSystem->RemoveMappingContext(mappingContext);
		return;
	}
}
void ACore_PlayerController::HaveMappingContextsBeenAsigned()
{
	if (!defaultMappingContext)
	{
		UE_LOG(LogTemp, Fatal, TEXT("NO DEFUALT MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
	if (!combatMappingContext)
	{
		UE_LOG(LogTemp, Fatal, TEXT("NO COMBAT MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
	if (!turretPlacingMappingContext)
	{
		UE_LOG(LogTemp, Fatal, TEXT("NO TURRET MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
	if (!movementMappingContext)
	{
		UE_LOG(LogTemp, Fatal, TEXT("NO MOVEMENT MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
	if (!gameMenuWidgetsActionsMappingContext)
	{
		UE_LOG(LogTemp, Fatal, TEXT("NO GAME MENUS MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
}
;