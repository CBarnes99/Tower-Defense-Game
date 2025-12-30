#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileBase.h"
#include "AC_SpawnProjectile.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_SpawnProjectile : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_SpawnProjectile();

	/** Creates the pool of projectiles, called from the weapon */
	UFUNCTION(BlueprintCallable)
	void InitializePool(float fireRate, float lifetime);

	/** Spawns a projectile, and sends it towards a target location
	* @param traceStartLocation A FVector of the starting position where you want to trace a line from, usually the player camera position.
	* @param weaponMuzzleLocation A FVector of the location you want the projectile to spawn from. Use mesh->GetSocketLocation(FName Socket Name).
	* @param actorForwardVector A FVector of the forward vector of the actor you want this to spawn from.
	* @param damageDelt A Float of the amount of damage you want the projectile to deal.
	* @param projectileSpeed A Float of the speed you want the projectile to travel. 
	* @param projectileLifetime The lifetime of the projectile, how long it take to deactivate itself if it hasnt collided with anything */
	UFUNCTION(BlueprintCallable)
	void FireProjectile(FVector traceStartLocation, FVector weaponMuzzleLocation, FVector actorForwardVector, float damageDelt, float projectileSpeed, float projectileLifetime);

protected:

	virtual void BeginPlay() override;

	/** The projectile class you want to fire from the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	class TSubclassOf<AProjectileBase> projectile;

	/** Returns the projectile that isn't currently in use within the pool
	* @return The projectile that is currently inactive within the pool, as a AProjectileBase* */
	UFUNCTION(BlueprintCallable)
	AProjectileBase* GetInactiveProjectile();

	/** Gets the target location at the end of a line trace
	* @param traceStartLocation The starting location for the line trace
	* @param actorForwardVector The forward vector of the actor you want to trace from
	* @return Returns the location of impact or the end distance if no collision, as FVector */
	UFUNCTION(BlueprintCallable)
	FVector GetTraceTargetLocation(FVector traceStartLocation, FVector actorForwardVector);
	/** Spawns a projectile and adds it to the pool, gets called by initialize pool */
	UFUNCTION(BlueprintCallable)
	void spawnProjectile();

	/** An array of all the pooled projectiles */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TArray<AProjectileBase*> projectilePool;

	/** The size of the projectile pool */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	float poolSize;

	/** Amount of projectiles in the pool, used for logging and if another pojectile is needed to spawn in the pool */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	int amountInPool;

};


