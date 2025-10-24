// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EngineUtils.h" 
#include "EnemySpawner.h"
#include "CombatGameMode.h"
#include "ProjectileBase.h"

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Set defualt mapping context
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (defualtMappingContext)
			{
				Subsystem->AddMappingContext(defualtMappingContext, 0);
			}
		}
	}
}

void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Binds the Input Actions assigned in the editor to the corisponding functions
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(moveActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::MovementAction);

		Input->BindAction(lookActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::MouseLookAction);

		Input->BindAction(jumpActionInput, ETriggerEvent::Started, this, &ACombatPlayerController::JumpAction);
		Input->BindAction(jumpActionInput, ETriggerEvent::Completed, this, &ACombatPlayerController::StopJumpingAction);

		Input->BindAction(runActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::RunningAction);
		Input->BindAction(runActionInput, ETriggerEvent::Completed, this, &ACombatPlayerController::RunningActionStop);

		Input->BindAction(startEnemyWaveActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallGameModeToStartSpawningEnemies);

		Input->BindAction(attackActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::AttackAction);

	}
}

void ACombatPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACombatPlayerController::MovementAction(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("WSAD"));

	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FVector Forward = myCharacter->GetActorForwardVector();
		const FVector Right = myCharacter->GetActorRightVector();

		myCharacter->AddMovementInput(Forward, MovementVector.Y);
		myCharacter->AddMovementInput(Right, MovementVector.X);
	}

}

void ACombatPlayerController::MouseLookAction(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Mouse"));

	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		myCharacter->AddControllerPitchInput(LookAxisVector.Y);
		myCharacter->AddControllerYawInput(LookAxisVector.X);
	}
}

void ACombatPlayerController::RunningAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shift"));

	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		myCharacter->GetCharacterMovement()->MaxWalkSpeed = myCharacter->runSpeed;
	}
	
}

void ACombatPlayerController::RunningActionStop()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shift"));

	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		myCharacter->GetCharacterMovement()->MaxWalkSpeed = myCharacter->movementSpeed;
	}
}

void ACombatPlayerController::JumpAction()
{
	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		myCharacter->Jump();
	}
}

void ACombatPlayerController::StopJumpingAction()
{
	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		myCharacter->StopJumping();
	}
}

void ACombatPlayerController::AttackAction()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Left Mouse Click"));

	if (APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (!myCharacter->projectile)
		{
			UE_LOG(LogTemp, Error, TEXT("Projectile class is not set on player character!"));
			return;
		}

		FVector spawnLocation = myCharacter->GetActorLocation() + myCharacter->GetActorForwardVector() * 100.f;
		FRotator spawnRotation = myCharacter->GetActorRotation();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = myCharacter;
		spawnParams.Instigator = myCharacter;

		AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(myCharacter->projectile, spawnLocation, spawnRotation, spawnParams);

		if (projectile)
		{
			projectile->FireInDirection(myCharacter->GetActorForwardVector());
			//UE_LOG(LogTemp, Warning, TEXT("Projectile spawned at %s"), *spawnLocation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile."));
		}
	}
}


void ACombatPlayerController::CallGameModeToStartSpawningEnemies()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("ENTER PRESSED"));

	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();

	ACombatGameMode* MyGameMode = Cast<ACombatGameMode>(GameModeBase);

	MyGameMode->StartEnemyWave();
};
