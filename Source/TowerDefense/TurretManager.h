#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretManager.generated.h"

class UAC_PlaceActor;
class ATurretStatic;
class UDA_TurretInfo;
class ACore_GameState;

UCLASS()
class TOWERDEFENSE_API ATurretManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurretManager();

	/** An Actor Component that allows the placeing of actors, turrets in this case*/
	UPROPERTY(EditDefaultsOnly)
	UAC_PlaceActor* placeActorComponent;

	/** The data asset of the turret that is currently being placed */
	UPROPERTY(VisibleAnywhere)
	UDA_TurretInfo* currentTurretInfo;

	//Functions that are called then forwarded into the placeActorComponent
	UFUNCTION(BlueprintCallable)
	void UpdateTurretPlacementLocation(FVector traceStartLocation, FVector forwardVector);

	UFUNCTION(BlueprintCallable)
	void StartTurretPlacement(UDA_TurretInfo* turretInfo);

	UFUNCTION(BlueprintCallable)
	void ConfirmTurretPlacement();

	UFUNCTION(BlueprintCallable)
	bool IsAbleToPlaceTurret();

	UFUNCTION(BlueprintCallable)
	void CancelTurretPlacement();

	UFUNCTION(BlueprintCallable)
	void RotateTurretPlacement(float directon);

	UFUNCTION(BlueprintCallable)
	bool IsPlacingTurret();

	UFUNCTION(BlueprintCallable)
	void UpdateIgnoreActors(AActor* actor, bool addToArray);

protected:
	virtual void BeginPlay() override;

	/** A Pointer to the core game state */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	ACore_GameState* coreGameState;

	};
