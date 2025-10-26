// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacterBase.h"
#include "ProjectileBase.generated.h"

UCLASS()
class TOWERDEFENSE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void FireInDirection(const FVector& shootDir);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetDamage(float damageAmount);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetProjectileSpeed(float speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* projectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* collisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* projectileMovementComponent;

	FTimerHandle lifeTimeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float lifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float damageDelt;

	UFUNCTION(BlueprintCallable)
	void destoryProjectileActor();

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacterBase> targetClassToDestroy;
};
