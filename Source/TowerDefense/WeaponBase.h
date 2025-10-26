// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_WeaponStats.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class TOWERDEFENSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void spawnProjectile(const UCameraComponent* playerCamera);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* weaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class TSubclassOf<AProjectileBase> projectile;

	//Weapon stats below are set from the Data Asset at begin play
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_WeaponStats* weaponStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName weaponMuzzleName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float damageDelt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int currentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int maxAmmo;

	//Gets the target location to where the projectile will be aimed towards
	UFUNCTION (BlueprintCallable)
	FVector GetTraceTargetLocation(const UCameraComponent* playerCamera);
};
