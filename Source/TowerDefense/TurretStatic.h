#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretStatic.generated.h"

class UDA_TurretInfo;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TOWERDEFENSE_API ATurretStatic : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurretStatic();

protected:
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform) override;

	/** The Mesh for the turret	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* turretMesh;
	
	/** A Mesh that will be the root and be invisable */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* invisableRootMesh;

	/** The mesh that outlines the collision box to show where it can damage */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* collisionMesh;

	/** The size of the collsion box to match the size of the collision mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector collisionBoxSize;

	/** The collision that damages the enemies */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* collisionBox;

	/** Data Asset that holds the turrets defualt values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UDA_TurretInfo* turretStats;

	/** The timer handle for the active and recharge times */
	UPROPERTY(EditAnywhere)
	FTimerHandle activeAndRechargeTimerHandle;

	/** A bool to check if the turret is currently damaging enemies, as a bool */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	bool turretActive;

	/** A check to see if the turret is recharging, as a bool */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	bool turretRecharging;

	/** Turn off the damage capabilities for the turret, is called by the activeAndRechargeTimerHandle */
	UFUNCTION(BlueprintCallable)
	void disableTurret();

	/** Turn on the damage capabilities for the turret, is called by the activeAndRechargeTimerHandle */
	UFUNCTION(BlueprintCallable)
	void enableTurret();

	/** When the turret damages an enemy, it puts a reference to it here so that it can only damage it once per active cycle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<AActor*> damagedActors;

	/** Built in overlap function */
	UFUNCTION(BlueprintCallable)
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult& SweepResult);

};
