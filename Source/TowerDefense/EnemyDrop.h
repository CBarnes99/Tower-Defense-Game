#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "E_EnemyDrop.h"
#include "EnemyDrop.generated.h"


UCLASS()
class TOWERDEFENSE_API AEnemyDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyDrop();


	/**
	* @brief Sets the type of drop the enemy drops
	* @param dropType The type of pickup you want the enemy to drop, as an EEnemyDrop
	*/
	UFUNCTION(BlueprintCallable)
	void SetDrop(EEnemyDrop dropType);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* dropMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* collisionComponent;

	/**
	* @brief The Drop type is set when SetDrop is called, and is used for OnPickUp to do a task depending on the value of dropType
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EEnemyDrop dropType;

	/**
	* @brief Built in overlap function
	*/
	UFUNCTION(BlueprintCallable)
	void OnPickUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle MovementTimerHandle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	float rotationSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	float bobbingAmplitude;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	float bobbingFrequency;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	float timerInterval;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	float runningTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	FVector initialLocation;

	UFUNCTION(BlueprintCallable)
	void UpdateMotion();


};
