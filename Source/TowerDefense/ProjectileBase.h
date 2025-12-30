#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TOWERDEFENSE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	/** The direction you want the projectile to fire
	* @param shootDir The direction you want the projectile to move, as FVector& */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void FireInDirection(const FVector& shootDir);

	/** Set the default variables for the projectile
	* @param damageAmount The amount of Damage, as a float 
	* @param speed The speed of the projectile, as a float 
	* @param lifetime the lifetime of the projectile, as a float*/
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetProjectileDefaults(float damageAmount, float speed, float projectileLifetime);

	UFUNCTION(BlueprintCallable)
	bool GetDefualtsBeenSet();

	///** The amount of damage you want this projectile to deal
	//* @param damageAmount The amount of Damage, as a float */
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	//void SetDamage(float damageAmount);

	///** How fast you want the projectile to fly
	//* @param speed The speed of the projectile, as a Float */
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	//void SetProjectileSpeed(float speed);

	///** Set the lifetime of the projectile
	//* @param lifetime the lifetime of the projectile, as a Float */
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	//void SetProjectileLifetime(float projectileLifetime);

	/** A check to see if the projectile is active or in the pool
	* @return True, the projectile is active, False, the projectile is avaliable to use from the pool */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	bool IsProjectileActive();

	/** When the projectile is called to be used, activate it */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void ActivateProjectile();

	/** When the projectile impacts an actor or after the lifetime timer is called, deactivate the projectile */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void DeactivateProjectile();

	/** The projectile movement component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* projectileMovementComponent;
protected:

	virtual void BeginPlay() override;

	/** The mesh of the projectile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* projectileMesh;

	/** The collision for the projectile */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USphereComponent* collisionComponent;

	/** The timer handle for the lifetime of the projectile	*/
	FTimerHandle lifeTimeTimerHandle;

	/** How long you want the projectile to live if it didnt collide with an actor, as a float */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float lifeTime;

	/** A bool to check if the projectile is in use or in the pool */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isActive;

	/** The amount of damage you want the projectile to deal */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float damageDelt;

	/** A bool to check if the default damage and speed has been set, to avoid needing to set it each time it is called to be fired */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool hasDefaultsBeenSet;

	/** Default OnHit function */
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);
};
