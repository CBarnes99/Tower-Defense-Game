#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_WeaponInfo.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "AC_SpawnProjectile.h"
#include "WeaponBase.generated.h"

UCLASS()
class TOWERDEFENSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	/**
	* @brief Actor Component that handles spawning the projectile
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UAC_SpawnProjectile* spawnProjectileComponent;

	/**
	* @brief Gets the weapons muzzle location for where to spawn the projectile
	* @return The location of the muzzle, as a FVector
	*/
	UFUNCTION(BlueprintCallable)
	FVector GetWeaponMuzzleLocation();

	/**
	* @brief Gets the damage delt from the weapon
	* @return The damage delt, as a float
	*/
	UFUNCTION(BlueprintCallable)
	float GetDamageDelt();

	/**
	* @brief Gets the speed of the projectile
	* @return The speed of the projectile, as a float
	*/
	UFUNCTION(BlueprintCallable)
	float GetProjectileSpeed();

protected:
	virtual void BeginPlay() override;

	/**
	* @brief The mesh for the weapon
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* weaponMesh;

	/**
	* @brief A Data Asset that holds the default values for the weapon
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_WeaponInfo* weaponStats;

	/**
	* @brief The name of the muzzle that is within the mesh
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName weaponMuzzleName;

	/**
	* @brief how much damage the projectile this weapon spawns deals
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float damageDelt;

	/**
	* @brief The amount of ammo the weapon currently has
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int currentAmmo;

	/**
	* @brief The amount of ammo the weapon can hold
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int maxAmmo;

	/**
	* @brief How fast you want the projectile to move
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float projectileSpeed;
};
