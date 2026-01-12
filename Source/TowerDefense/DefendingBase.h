#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefendingBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;

DECLARE_DELEGATE_OneParam(FBaseHealthDecreasedSigniture, int);
DECLARE_DELEGATE(FBaseHealthReachedZeroSigniture);

UCLASS()
class TOWERDEFENSE_API ADefendingBase : public AActor
{
	GENERATED_BODY()
	
public:

	ADefendingBase();

	FBaseHealthDecreasedSigniture BaseHealthDecreasedEvent;

	FBaseHealthReachedZeroSigniture BaseHealthReachedZeroEvent;

	UFUNCTION(BlueprintCallable)
	int GetBaseHealth();

protected:
	virtual void BeginPlay() override;

	/** The mesh of hte base */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* baseMesh;

	/** The collision component that detects if an enemy has reached the base */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* enemyCollisionDetection;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** The amount of health the base has */
	UPROPERTY(VisibleAnywhere)
	int baseHealth;
	
};
