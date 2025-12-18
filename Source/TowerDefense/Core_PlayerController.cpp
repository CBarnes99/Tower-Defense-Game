#include "Core_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EngineUtils.h" 
#include "EnemySpawner.h"
#include "Core_GameMode.h"
#include "ProjectileBase.h"
#include "DrawDebugHelpers.h"

void ACore_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	HaveMappingContextsBeenAsigned();

	//Set defualt mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (defaultMappingContext)
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
			Subsystem->AddMappingContext(combatMappingContext, 1);
		}
	}

	if (!turretDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("No Data Table Assigned in Player Controller"));
	}
	else
	{
		TArray<FName> rowNames = turretDataTable->GetRowNames();
		dataTableSize = rowNames.Num();
		UE_LOG(LogTemp, Display, TEXT("Amount Of Turrets In DataTable = %d"), dataTableSize);
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

		Input->BindAction(rotateTurretRightInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::RotateTurret);
		Input->BindAction(rotateTurretLeftInput, ETriggerEvent::Triggered, this, &ACore_PlayerController::RotateTurret);

		Input->BindAction(openTurretSelectionMenu, ETriggerEvent::Triggered, this, &ACore_PlayerController::OpenTurretSelectionMenu);
	}
}

void ACore_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	myPlayerCharacter = Cast<APlayerCharacter>(InPawn);
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

	myPlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
	myPlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	UpdateHotbarSelection();
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
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Left Mouse Click"));

	myPlayerCharacter->equippedWeapon->spawnProjectileComponent->FireProjectile
	(
		myPlayerCharacter->GetCameraLocation(),
		myPlayerCharacter->equippedWeapon->GetWeaponMuzzleLocation(),
		myPlayerCharacter->GetCameraForwardVector(),
		myPlayerCharacter->equippedWeapon->GetDamageDelt(),
		myPlayerCharacter->equippedWeapon->GetProjectileSpeed()	
	);	

}

void ACore_PlayerController::CallGameModeToStartSpawningEnemies()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("ENTER PRESSED"));

	StartWaveEvent.Broadcast();
};

void ACore_PlayerController::OpenTurretSelectionMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenTurretButtonPressed"));
}

void ACore_PlayerController::ScrollWheelSelectionAction(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Value.ToString());

	if (Value.Get<float>() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Scroll Up %s"), *Value.ToString());
		myPlayerCharacter->hotbarSelectionIndex = FMath::Clamp(myPlayerCharacter->hotbarSelectionIndex + 1, 1, dataTableSize + 1);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Scroll Down %s"), *Value.ToString());
		myPlayerCharacter->hotbarSelectionIndex = FMath::Clamp(myPlayerCharacter->hotbarSelectionIndex - 1, 1, dataTableSize + 1);

	}
	UE_LOG(LogTemp, Warning, TEXT("Hotbar Index is %d"), myPlayerCharacter->hotbarSelectionIndex);
	UpdateHotbarSelection();

}
void ACore_PlayerController::ConfirmTurretPlacementAction()
{
	if (myPlayerCharacter->hotbarSelectionIndex > 1)
	{
		myPlayerCharacter->PlaceTurret();
	}

}
void ACore_PlayerController::RotateTurret(const FInputActionValue& Value)
{

	UE_LOG(LogTemp, Warning, TEXT("Value = %s"), *Value.ToString());
	myPlayerCharacter->RotateTurret(Value.Get<float>());
}


void ACore_PlayerController::UpdateHotbarSelection()
{
	if (myPlayerCharacter->hotbarSelectionIndex > 1)
	{
		UseCombatMappingContext(false);
		myPlayerCharacter->UpdateTurretPlacement();
	}
	else
	{
		UseCombatMappingContext(true);

		if (!myPlayerCharacter->turretManager)
		{
			myPlayerCharacter->SetTurretManager();
		}

		if (myPlayerCharacter->turretManager->GetisPreviewTurretActive())
		{
			myPlayerCharacter->turretManager->NoLongerPlacingTurrets();
		}
	}
}
void ACore_PlayerController::UseCombatMappingContext(bool confirm)
{
	UEnhancedInputLocalPlayerSubsystem* inputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (confirm)
	{
		if (!inputSubsystem->HasMappingContext(combatMappingContext) && inputSubsystem->HasMappingContext(turretPlacingMappingContext))
		{
			inputSubsystem->RemoveMappingContext(turretPlacingMappingContext);
			inputSubsystem->AddMappingContext(combatMappingContext, 1);
			return;
		}
	}
	else if (!inputSubsystem->HasMappingContext(turretPlacingMappingContext) && inputSubsystem->HasMappingContext(combatMappingContext))
	{
		inputSubsystem->RemoveMappingContext(combatMappingContext);
		inputSubsystem->AddMappingContext(turretPlacingMappingContext, 1);
		return;
	}
}
void ACore_PlayerController::HaveMappingContextsBeenAsigned()
{
	if (!defaultMappingContext)
	{
		UE_LOG(LogTemp, Error, TEXT("NO DEFUALT MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
	if (!combatMappingContext)
	{
		UE_LOG(LogTemp, Error, TEXT("NO COMBAT MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}if (!turretPlacingMappingContext)
	{
		UE_LOG(LogTemp, Error, TEXT("NO TURRET MAPPING CONTEXT WITHIN %s"), *this->GetName());
	}
}
;