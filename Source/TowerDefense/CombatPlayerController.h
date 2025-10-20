// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <EnhancedInputLibrary.h>
#include "InputActionValue.h"
#include "CombatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "References")
	class AEnemySpawner* EnemySpawner;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;


	//Player Input Mapping Variables which are editable in BP to select the button inputs for each action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefualtMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* RunActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* StartEnemyWaveActionInput;

	void MovementAction(const FInputActionValue& Value);
	void MouseLookAction(const FInputActionValue& Value);
	void RunningAction();
	void RunningActionStop();
	void HandleSpawnEnemyFromSpawner();
	void JumpAction();
	void StopJumpingAction();
};
