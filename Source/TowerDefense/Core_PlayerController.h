#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <EnhancedInputLibrary.h>
#include "InputActionValue.h"
#include "Core_PlayerController.generated.h"

class ATurretManager;
class ATurretStatic;
class APlayerCharacter;
class ACore_GameState;
class ACore_HUD;
class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class UInputAction;
class UDA_TurretInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartWaveSigniture);
DECLARE_DELEGATE_RetVal_OneParam(UDA_TurretInfo* /*Turret Info To Place Turret*/, FGetTurretDASigniture, int /*Index for Array within UHUDWeaponTurretSelector*/)

UCLASS()
class TOWERDEFENSE_API ACore_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** A delegate to start a new wave, called to the game mode */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FStartWaveSigniture StartWaveEvent;

	/** A delegate to get the current turrets Data Asset the player has selected from the HUDWeaponTurretSelector */
	FGetTurretDASigniture GetTurretDAEvent;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	/** A pointer to the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* myPlayerCharacter;

	/** A pointer to the Core HUD */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACore_HUD* coreHUD;

	/** A pointer to the coreGameState */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACore_GameState* coreGameState;

	/** The index value to get the turret the player wants to select, 0 is the weapon 1+ is the turrets. Use -1 when getting turrets from an array */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int hotbarSelectionIndex;

	/** A check to see if the player has selected a differet turret to use */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int previousHotbarSelectionIndex;

	/** The turret manager within the level which handles the placing of the turrets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATurretManager* turretManager;

	/** The current turret the player wants to spawn, the value is gotten from the GetTurretClassEvent delegate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> currentTurretClass;

	/** The Enhahnced Input Subsystem which is given its value a begin play */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubSystem;

	/** When scroll wheel is used, this function is called */
	UFUNCTION(BlueprintCallable)
	void UpdateHotbarSelection();

	/**
	* A function to add or remove a mapping context
	* @param mappingContext The Mapping Context you want to add or remove to the subsystem
	* @param addContext True adds the mapping context, false removes the mapping context
	* @param priority The priority you want the context to have. If addContext is false, does not matter this param equals.
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateMappingContext(UInputMappingContext* mappingContext, bool addContext, int priority);

	/** A check to see if the mapping context has been assigned in the editor */
	UFUNCTION(BlueprintCallable)
	void HaveMappingContextsBeenAsigned();

	//Player Input Mapping Variables which are editable in BP to select the button inputs for each action
	UPROPERTY(EditAnywhere, Category = "Input_Default")
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	UInputAction* startEnemyWaveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Default")
	UInputAction* scrollWheelSelectionInput;

	//-------------------------------------------------//

	UPROPERTY(EditAnywhere, Category = "Input_Combat")
	UInputMappingContext* combatMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Combat")
	UInputAction* attackActionInput;

	//-------------------------------------------------//

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	UInputMappingContext* turretPlacingMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	UInputAction* confirmTurretPlacementInput;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	UInputAction* cancelTurretPlacement;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	UInputAction* rotateTurretRightInput;

	UPROPERTY(EditAnywhere, Category = "Input_Turret")
	UInputAction* rotateTurretLeftInput;

	//-------------------------------------------------//

	UPROPERTY(EditAnywhere, Category = "Input_Movement")
	UInputMappingContext* movementMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_Movement")
	UInputAction* moveActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Movement")
	UInputAction* lookActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Movement")
	UInputAction* jumpActionInput;

	UPROPERTY(EditAnywhere, Category = "Input_Movement")
	UInputAction* runActionInput;

	//-------------------------------------------------//

	UPROPERTY(EditAnywhere, Category = "Input_GameMenus")
	UInputMappingContext* gameMenuWidgetsActionsMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input_GameMenus")
	UInputAction* openTurretSelectionMenu;

	//-------------------------------------------------//

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
	void CancelTurretPlacementAction();

	UFUNCTION(BlueprintCallable)
	void RotateTurret(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OpenTurretSelectionMenu();
};
