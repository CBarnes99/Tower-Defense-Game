// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "EngineUtils.h" 
#include "EnemySpawner.h"
//#include <Kismet/GameplayStatics.h>

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Set defualt mapping context
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefualtMappingContext)
			{
				Subsystem->AddMappingContext(DefualtMappingContext, 0);
			}
		}
	}
}

void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(MoveActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::MovementAction);

		Input->BindAction(LookActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::MouseLookAction);

		Input->BindAction(JumpActionInput, ETriggerEvent::Started, this, &ACombatPlayerController::JumpAction);
		Input->BindAction(JumpActionInput, ETriggerEvent::Completed, this, &ACombatPlayerController::StopJumpingAction);

		Input->BindAction(RunActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::RunningAction);
		Input->BindAction(RunActionInput, ETriggerEvent::Completed, this, &ACombatPlayerController::RunningActionStop);

		Input->BindAction(StartEnemyWaveActionInput, ETriggerEvent::Triggered, this, &ACombatPlayerController::HandleSpawnEnemyFromSpawner);

	}
}

void ACombatPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACombatPlayerController::MovementAction(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("WSAD"));

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FVector Forward = MyCharacter->GetActorForwardVector();
		const FVector Right = MyCharacter->GetActorRightVector();

		MyCharacter->AddMovementInput(Forward, MovementVector.Y);
		MyCharacter->AddMovementInput(Right, MovementVector.X);
	}

}

void ACombatPlayerController::MouseLookAction(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Mouse"));

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		MyCharacter->AddControllerPitchInput(LookAxisVector.Y);
		MyCharacter->AddControllerYawInput(LookAxisVector.X);
	}
}

void ACombatPlayerController::RunningAction()
{
	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shift"));
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MyCharacter->MaxSpeed * 3;
	}
	
}

void ACombatPlayerController::RunningActionStop()
{

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shift"));
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MyCharacter->MaxSpeed;
	}
}

void ACombatPlayerController::JumpAction()
{
	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		MyCharacter->Jump();
	}
}

void ACombatPlayerController::StopJumpingAction()
{
	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		MyCharacter->StopJumping();
	}
}


void ACombatPlayerController::HandleSpawnEnemyFromSpawner()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("ENTER PRESSED"));

	//Get the enemy spawner from the world
	if (!EnemySpawner)
	{
		for (TActorIterator<AEnemySpawner> i(GetWorld()); i; ++i)
		{
			EnemySpawner = *i;
			if (EnemySpawner)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reference to Spawner Found"));
				break; // Stop after first found
			}
		}
	}

	if (!EnemySpawner->isSpawning)
	{
		EnemySpawner->StartSpawning();
	}
	else
	{
		EnemySpawner->StopSpawning();
	}
}


