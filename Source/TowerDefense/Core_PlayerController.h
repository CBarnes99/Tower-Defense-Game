#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <EnhancedInputLibrary.h>
#include "PlayerCharacter.h"
#include "InputActionValue.h"
#include "Core_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartWaveSigniture);

UCLASS()
class TOWERDEFENSE_API ACore_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/**
	* @brief A pointer to the enemy spawner
	*/
	UPROPERTY(EditAnywhere, Category = "References")
	class AEnemySpawner* enemySpawner;

	/**
	* @brief A delegate to start a new wave
	*/
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FStartWaveSigniture StartWaveEvent;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	/**
	* @brief A pointer to the player character
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerCharacter* myPlayerCharacter;

	/**
	* @brief A Data Table pointer to the Data Table that holds a soft ptr to the turrets, assigned in the editor
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDataTable* turretDataTable;

	/**
	* @brief The size of the data table, set on Begin Play
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int dataTableSize;

	/**
	* @brief When scroll wheel is used, this function is called
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateHotbarSelection();

	/**
	* @brief Change between combat and turret mapping context
	* @param confirm True changes to combat context, false changes to turret context
	*/
	UFUNCTION(BlueprintCallable)
	void UseCombatMappingContext(bool confirm);

	/**
	* @brief A check to see if the mapping context has been assigned in the editor
	*/
	UFUNCTION(BlueprintCallable)
	void HaveMappingContextsBeenAsigned();

	//Player Input Mapping Variables which are editable in BP to select the button inputs for each action
	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Combat")
	class UInputMappingContext* combatMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	class UInputMappingContext* turretPlacingMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* moveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* lookActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* jumpActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* runActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* startEnemyWaveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* openTurretSelectionMenu;

	UPROPERTY(EditAnywhere, Category = "Input_Combat")
	class UInputAction* attackActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	class UInputAction* scrollWheelSelectionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	class UInputAction* confirmTurretPlacementInput;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	class UInputAction* rotateTurretRightInput; 
	
	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	class UInputAction* rotateTurretLeftInput;


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

	UFUNCTION(BlueprintCallable)
	void ScrollWheelSelectionAction(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void ConfirmTurretPlacementAction();

	UFUNCTION(BlueprintCallable)
	void RotateTurret(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OpenTurretSelectionMenu();
};
