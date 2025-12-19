// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_TurretInfo.h"
#include "AC_PreviewMesh.h"
#include "TurretStatic.generated.h"

UCLASS()
class TOWERDEFENSE_API ATurretStatic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretStatic();

	/**
	* @brief Sets the preview material for the turret, only used for the pooled turrets by the manager
	*/
	UFUNCTION(BlueprintCallable)
	void SetPreviewMaterial(bool isPreview);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform) override;

	/**
	* @brief The Mesh for the turret
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* turretMesh;

	/**
	* @brief The mesh that outlines the collision box to show where it can damage
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* collisionMesh;

	/**
	* @brief The size of the collsion box to match the size of the collision mesh
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector collisionBoxSize;

	/**
	* @brief The collision that damages the enemies
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* collisionBox;

	/**
	* @brief Data Asset that holds the turrets defualt values
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UDA_TurretInfo* turretStats;

	/**
	* @brief Actor Component that holds the preview material and Defualt material for the turret
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	UAC_PreviewMesh* previewMeshComponent;

	/**
	* @brief The amount of damage the turret deals, as a float
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	float damageAmount;

	/**
	* @brief How long the turret is active before it has to recharge, as a float
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	float activeTime;

	/**
	* @brief How long it takes the turret to recharge after it is active, as a float
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	float rechargeTime;

	/**
	* @brief The amount it costs to spawn the turret, as an int
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	int cost;

	/**
	* @brief The timer handle for the active and recharge times
	*/
	UPROPERTY(EditAnywhere)
	FTimerHandle activeAndRechargeTimerHandle;

	/**
	* @brief A bool to check if the turret is currently damaging enemies, as a bool
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	bool turretActive;

	/**
	* @brief A check to see if the turret is recharging, as a bool
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	bool turretRecharging;

	/**
	* @brief Turn off the damage capabilities for the turret, is called by the activeAndRechargeTimerHandle
	*/
	UFUNCTION(BlueprintCallable)
	void disableTurret();

	/**
	* @brief Turn on the damage capabilities for the turret, is called by the activeAndRechargeTimerHandle
	*/
	UFUNCTION(BlueprintCallable)
	void enableTurret();

	/**
	* @brief When the turret damages an enemy, it puts a reference to it here so that it can only damage it once per active cycle
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<AActor*> damagedActors;

	//Will set up damage types later on
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<UDamageType> damageType;*/

	/**
	* @brief Built in overlap function
	*/
	UFUNCTION(BlueprintCallable)
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult& SweepResult);

};
