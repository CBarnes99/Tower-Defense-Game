#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_PlaceActor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_PlaceActor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_PlaceActor();

	UFUNCTION(BlueprintCallable)
	void StartPlacement(TSubclassOf<AActor> chosenActorClass);

	UFUNCTION(BlueprintCallable)
	void UpdatePlacementLocation(FVector traceStartLocation, FVector forwardVector);

	UFUNCTION(BlueprintCallable)
	void ConfirmPlacement();

	UFUNCTION(BlueprintCallable)
	void CancelPlacement();

	UFUNCTION(BlueprintCallable)
	void RotatePlacement(float directon);

	UFUNCTION(BlueprintCallable)
	bool IsPlacing();

	UFUNCTION(BlueprintCallable)
	void UpdateIgnoreActors(AActor* actor, bool addToArray);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> actorClassToSpawn;

	UPROPERTY(EditDefaultsOnly)
	AActor* previewActor;

	UPROPERTY(EditDefaultsOnly)
	bool bIsPlacing;

	UPROPERTY(EditDefaultsOnly)
	float traceDistance;

	UPROPERTY(EditDefaultsOnly)
	float rotationDegrees;

	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> actorsToIgnore;

	/**
	* @brief Gets the target location at the end of a line trace
	* @param traceStartLocation The starting location for the line trace
	* @param actorForwardVector The forward vector of the actor you want to trace from
	* @return Returns the location of impact or the end distance if no collision, as FVector
	*/
	UFUNCTION(BlueprintCallable)
	FVector GetTraceTargetLocation(FVector traceStartLocation, FVector actorForwardVector);

	/**
	* @brief Set the actors you want the line trace to ignore before calling GetTraceTargetLocation
	* @param ignoredActors An array of AActor* to ignore, as an TArray<AActor*>
	*/
	UFUNCTION(BlueprintCallable)
	void SetIgnoredActors(TArray<AActor*> ignoredActors);

	/**
	* @brief Set a singular actor to be ignored by the line trace before calling GetTraceTargetLocation
	* @param ignoredActor An actor you want the trace to ignore, as an AActor*
	*/
	UFUNCTION(BlueprintCallable)
	void SetIgnoredActor(AActor* ignoredActor);

	/**
	* @brief A Check to see if there is an impact point from the line trace
	* @param traceStartLocation The starting location for the line trace
	* @param actorForwardVector The forward vector of the actor you want to trace from
	* @return If the line trace has an impact, as a Bool
	*/
	UFUNCTION(BlueprintCallable)
	bool HasImpactPoint(FVector traceStartLocation, FVector actorForwardVector);

	/**
	* @brief The query parmas for the line trace
	*/
	FCollisionQueryParams params;

	/**
	* @brief The actors you want to ignore for the line trace
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UClass* ignoredActorClass;
		
};
