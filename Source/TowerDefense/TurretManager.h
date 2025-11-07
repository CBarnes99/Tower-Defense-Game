#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretStatic.h"
#include "TurretManager.generated.h"

UCLASS()
class TOWERDEFENSE_API ATurretManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ATurretStatic* SpawnTurretByRow(int rowIndex, FVector spawnLoc, FRotator spawnRot);

	//UFUNCTION(BlueprintCallable)
	//void UpdatePreviewTurretLocation(FVector previewTurretPos, int index);

	UFUNCTION(BlueprintCallable)
	void DisablePreviewTurret(ATurretStatic* turret);

	UFUNCTION(BlueprintCallable)
	void EnablePreviewTurret(ATurretStatic* turret);

	/*UFUNCTION(BlueprintCallable)
	void PlaceTurret();*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* turretDataTable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<FName> dataTableRowNames;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	//ATurretStatic* previewTurretActor;

	UFUNCTION(BlueprintCallable)
	void PoolAllPreivewTurrets();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<ATurretStatic*> pooledPreviewTurrets;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<ATurretStatic*> allPlacedTurrets;
	};
