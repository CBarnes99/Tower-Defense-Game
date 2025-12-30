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

	///** The name of the muzzle that is within the mesh */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//FName weaponMuzzleName;

	///** How much damage the projectile this weapon spawns deals */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//float damageDelt;

	///** The amount of ammo the weapon currently has */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//int currentAmmo;

	///** The amount of ammo the weapon can hold */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//int maxAmmo;

	///** How fast you want the projectile to move */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//float projectileSpeed;

	/** A Check to see if the weapon can fire, based of the weapons fire rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanFire;

	UFUNCTION(BlueprintCallable)
	void StartWeaponFireRateCooldown();

	FTimerHandle fireRateHandle;

	UFUNCTION(BlueprintCallable)
	void EnableCanFire();

	UFUNCTION(BlueprintCallable)
	void DisableCanFire();
};
