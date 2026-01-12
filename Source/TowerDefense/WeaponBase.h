#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "WeaponBase.generated.h"

class UAC_SpawnProjectile;
class UStaticMeshComponent;
class UDA_WeaponInfo;

UCLASS()
class TOWERDEFENSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	/** Actor Component that handles spawning the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UAC_SpawnProjectile* spawnProjectileComponent;

	/** Gets the weapons muzzle location for where to spawn the projectile
	* @return The location of the muzzle, as a FVector */
	UFUNCTION(BlueprintCallable)
	FVector GetWeaponMuzzleLocation();

	/** Gets the damage delt from the weapon
	* @return The damage delt, as a float */
	UFUNCTION(BlueprintCallable)
	float GetDamageDelt();

	/** Gets the speed of the projectile
	* @return The speed of the projectile, as a float */
	UFUNCTION(BlueprintCallable)
	float GetProjectileSpeed();

	/** Gets the lifetime of the projectile
	* @return The lifetime of the projectile, as a float */
	UFUNCTION(BlueprintCallable)
	float GetProjectileLifetime();

	/** Called from the player character to fire the projectile */
	UFUNCTION(BlueprintCallable)
	void FireProjectile(FVector fireStartLoc, FVector forwardVector);

protected:
	virtual void BeginPlay() override;

	/** The mesh for the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* weaponMesh;

	/** A Data Asset that holds the default values for the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_WeaponInfo* weaponStats;

	/** A Check to see if the weapon can fire, based of the weapons fire rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanFire;

	/** A function that starts the fireRateHandle timer and at the timers end, it calls EnableCanFire */
	UFUNCTION(BlueprintCallable)
	void StartWeaponFireRateCooldown();

	/** The timer handle for the fire rate of the weapon */
	FTimerHandle fireRateHandle;

	/** A Function called by the fireRateHandle Timer to enable the weapon to fire */
	UFUNCTION(BlueprintCallable)
	void EnableCanFire();

	/** Called after a projectile has been fired to disable the ability to fire again until the fireRateHandle Timer has been called */
	UFUNCTION(BlueprintCallable)
	void DisableCanFire();
};
