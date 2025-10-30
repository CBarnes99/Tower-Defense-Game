// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <EnhancedInputLibrary.h>
#include "PlayerCharacter.h"
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
	class AEnemySpawner* enemySpawner;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerCharacter* myPlayerCharacter;

	//Player Input Mapping Variables which are editable in BP to select the button inputs for each action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* defualtMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* moveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* lookActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* jumpActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* runActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* startEnemyWaveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* attackActionInput;



	//Input Actions Functions
	UFUNCTION(BlueprintCallable)
	void MovementAction(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void MouseLookAction(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void RunningAction();

	UFUNCTION(BlueprintCallable)
	void RunningActionStop();

	UFUNCTION(BlueprintCallable)
	void CallGameModeToStartSpawningEnemies();

	UFUNCTION(BlueprintCallable)
	void JumpAction();

	UFUNCTION(BlueprintCallable)
	void StopJumpingAction();

	UFUNCTION(BlueprintCallable)
	void AttackAction();
};
