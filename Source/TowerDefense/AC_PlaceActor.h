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

	/** Called when you want to start placing an actor
	* @param chosenActorClass The class of the actor you want to place */
	UFUNCTION(BlueprintCallable)
	void StartPlacement(TSubclassOf<AActor> chosenActorClass);

	/** Called when you need to update the location of where you want to place the actor 
	* @param traceStartLocation the starting location for the trace
	* @param forwardVector the forward vector for the trace */
	UFUNCTION(BlueprintCallable)
	void UpdatePlacementLocation(FVector traceStartLocation, FVector forwardVector);

	/** Called to confirm the palcement of the actor */
	UFUNCTION(BlueprintCallable)
	void ConfirmPlacement();

	/** Called to cancel the placement of the actor */
	UFUNCTION(BlueprintCallable)
	void CancelPlacement();

	/** Called to rotate the actor
	* @param direction direction you want to rotate, -1 is to the left, 1 is to the right */
	UFUNCTION(BlueprintCallable)
	void RotatePlacement(float direction);

	/** A Check to see if you are currently placeing an actor */
	UFUNCTION(BlueprintCallable)
	bool IsPlacing();

	/** Called when you want to add or remove an actor to an ignore list 
	* @param actor The actor you want to add or remove form the list
	* @param addToArray true = add to the ignore list, false = remove from the ignore list */
	UFUNCTION(BlueprintCallable)
	void UpdateIgnoreActors(AActor* actor, bool addToArray);

protected:
	virtual void BeginPlay() override;

	/** The distance you want to line trace to go, as a float */
	UPROPERTY(EditDefaultsOnly)
	float traceDistance;

	/** The degrees of rotation you want your actor to rotate, as a float */
	UPROPERTY(EditDefaultsOnly)
	float rotationDegrees;

	/** The actor class you want to spawn, TSubclassOf<AActor> */
	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<AActor> actorClassToSpawn;

	/** The current preview actor, AActor* */
	UPROPERTY(VisibleDefaultsOnly)
	AActor* currentPreviewActor;

	/** The pool of preview actors, as a Array<AActor*> */
	UPROPERTY(VisibleDefaultsOnly)
	TArray<AActor*> previewActorPool;

	/** A check to see if you are currently placing an actor, as a bool */
	UPROPERTY(VisibleDefaultsOnly)
	bool bIsPlacing;
	/** An array of actors for the line trace to ignore, as a TArray<AActor*> */
	UPROPERTY(VisibleDefaultsOnly)
	TArray<AActor*> actorsToIgnore;

	/** Called to get the preview actor the player wants to place from the preview pool */
	UFUNCTION(BlueprintCallable)
	AActor* GetPreviewActorFromPool(TSubclassOf<AActor> actorClass);

	/** Called when you are no longer placing a specific actor and disabled it and returns it to the pool */
	UFUNCTION(BlueprintCallable)
	void DisablePreviewActor(AActor* actor);

	/** Called when you want to place a specific actor, it enables on from the preview pool to use */
	UFUNCTION(BlueprintCallable)
	void EnablePreviewActor(AActor* actor);

	/** Gets the target location at the end of a line trace
	* @param traceStartLocation The starting location for the line trace
	* @param actorForwardVector The forward vector of the actor you want to trace from
	* @return Returns the location of impact or the end distance if no collision, as FVector */
	UFUNCTION(BlueprintCallable)
	FVector GetTraceTargetLocation(FVector traceStartLocation, FVector actorForwardVector);

	/** Set the actors you want the line trace to ignore before calling GetTraceTargetLocation
	* @param ignoredActors An array of AActor* to ignore, as an TArray<AActor*> */
	UFUNCTION(BlueprintCallable)
	void SetIgnoredActors(TArray<AActor*> ignoredActors);

	/** Set a singular actor to be ignored by the line trace before calling GetTraceTargetLocation
	* @param ignoredActor An actor you want the trace to ignore, as an AActor* */
	UFUNCTION(BlueprintCallable)
	void SetIgnoredActor(AActor* ignoredActor);

	/** A Check to see if there is an impact point from the line trace
	* @param traceStartLocation The starting location for the line trace
	* @param actorForwardVector The forward vector of the actor you want to trace from
	* @return If the line trace has an impact, as a Bool */
	UFUNCTION(BlueprintCallable)
	bool HasImpactPoint(FVector traceStartLocation, FVector actorForwardVector);

	/** The query parmas for the line trace */
	FCollisionQueryParams params;

	/** The actors you want to ignore for the line trace */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UClass* ignoredActorClass;
		
};
