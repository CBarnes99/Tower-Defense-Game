// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_TurretStats.h"
#include "AC_PreviewMesh.h"
#include "TurretStatic.generated.h"

UCLASS()
class TOWERDEFENSE_API ATurretStatic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretStatic();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMaterial(bool isPreview);

	UFUNCTION(BlueprintCallable)
	void pooledTurretDisable();

	UFUNCTION(BlueprintCallable)
	void pooledTurretEnable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* turretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* collisionMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector collisionBoxSize;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* collisionBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* arrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UDA_TurretStats* turretStats;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	UAC_PreviewMesh* previewMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	float damageAmount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	float activeTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	float rechargeTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	int cost;

	UPROPERTY(EditAnywhere)
	FTimerHandle activeAndRechargeTimerHandle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	bool turretActive;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	bool turretRecharging;

	UFUNCTION(BlueprintCallable)
	void disableTurret();

	UFUNCTION(BlueprintCallable)
	void enableTurret();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<AActor*> damagedActors;

	//Will set up damage types later on
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<UDamageType> damageType;*/

	UFUNCTION(BlueprintCallable)
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult& SweepResult);

};
