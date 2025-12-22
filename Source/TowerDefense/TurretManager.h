#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretManager.generated.h"

class UAC_PlaceActor;
class ATurretStatic;

UCLASS()
class TOWERDEFENSE_API ATurretManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurretManager();

	/**
	* @brief Spawns the turret from the turret Data Table
	* @param rowIndex The index of the turret within the data table, as an int
	*/
	UFUNCTION(BlueprintCallable)
	ATurretStatic* SpawnTurretByRow(int rowIndex);

	/**
	* @brief Updates where the location of the preview turret is located
	* @param placeTurretLoc The location the player is looking, as a FVector
	* @param turretIndex The turret you want to spawn from the Data Table, as an int
	*/
	UFUNCTION(BlueprintCallable)
	void UpdatePreviewTurretLocation(FVector placeTurretLoc, int turretIndex);

	/**
	* @brief Disables the preview turret in the pool
	* @param turret The preview turret you want to disable, as a ATurretStatic*
	*/
	UFUNCTION(BlueprintCallable)
	void DisablePreviewTurret(ATurretStatic* turret);

	/**
	* @brief Enables the preview turret in the pool
	* @param turret The preview turret you want to enable, as a ATurretStatic*
	*/
	UFUNCTION(BlueprintCallable)
	void EnablePreviewTurret(ATurretStatic* turret);

	/**
	* @brief When you're not placing turrets anymore, this is called
	*/
	UFUNCTION(BlueprintCallable)
	void NoLongerPlacingTurrets();

	/**
	* @brief A Chcek to see if you are currently placing turrets
	* @return True, you are currently placing turrets, False, You are no longer placing turrets
	*/
	UFUNCTION(BlueprintCallable)
	bool GetisPreviewTurretActive();

	/**
	* @brief Rotate the preview turrets by 45 degrees
	* @param dir 1 or -1 recieved from key inputs, as a float
	*/
	UFUNCTION(BlueprintCallable)
	void RotatePreviewTurret(float dir);

	/**
	* @brief The data table that holds soft pointers to the turrets
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* turretDataTable;

	UPROPERTY(EditDefaultsOnly)
	UAC_PlaceActor* placeActorComponent;








	UFUNCTION(BlueprintCallable)
	void UpdateTurretPlacementLocation(FVector traceStartLocation, FVector forwardVector);


	UFUNCTION(BlueprintCallable)
	void StartTurretPlacement(TSubclassOf<AActor> actorClass);

	UFUNCTION(BlueprintCallable)
	void ConfirmTurretPlacement();

	UFUNCTION(BlueprintCallable)
	void CancelTurretPlacement();

	UFUNCTION(BlueprintCallable)
	void RotateTurretPlacement(float directon);

	UFUNCTION(BlueprintCallable)
	bool IsPlacingTurret();

	UFUNCTION(BlueprintCallable)
	void UpdateIgnoreActors(AActor* actor, bool addToArray);







protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


	/**
	* @brief An array of the data table row names, allowing the use of an index to get the get the rows in the data table
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<FName> dataTableRowNames;

	/**
	* @brief Pooling all the preview turrets, called on begin play
	*/
	UFUNCTION(BlueprintCallable)
	void PoolAllPreivewTurrets();

	/**
	* @brief An array of the pooled turrets
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<ATurretStatic*> pooledPreviewTurrets;

	/**
	* @brief The current index for the preview turrets and a check to see if you need to change to a different preview turret
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int currentPreviewTurretIndex;

	/**
	* @brief A check to see if you are currently placing turrets
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool isPreviewTurretActive;

	/**
	* @brief An array of all the placed turrets, not the preview turrets
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<ATurretStatic*> allPlacedTurrets;
	};
