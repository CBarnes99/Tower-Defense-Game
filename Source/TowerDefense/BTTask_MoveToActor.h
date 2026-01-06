#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToActor.generated.h"

UCLASS()
class TOWERDEFENSE_API UBTTask_MoveToActor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToActor();

	/** The actor you want to move towards, set in the behaviour tree */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** When you reached that target actor, what is the acceptance radius from that actor */
	UPROPERTY(EditAnywhere, Category = "Move")
	float acceptanceRadius = 150.f;

	/** How often you want the timer to update the target actors location */
	UPROPERTY(EditAnywhere, Category = "Move")
	float UpdateInterval = 0.2f;

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	/** Timer handle to update the target actors location */
	UPROPERTY(VisibleAnywhere)
	FTimerHandle MoveUpdateTimerHandle;

	/** The last location the target actor was before the UpdateMove function was called*/
	UPROPERTY(VisibleAnywhere)
	FVector LastTargetLocation;

	/** The behaviour tree component from the owner */
	UPROPERTY(VisibleAnywhere)
	UBehaviorTreeComponent* cachedOwnerComp;

	/** The actor you want to move towards, set in the behaviour tree */
	UFUNCTION()
	void UpdateMove();

	/** The cached controlled pawn */
	UPROPERTY(VisibleAnywhere)
	APawn* cachedControlledPawn;

	/** The cached AIController */
	UPROPERTY(VisibleAnywhere)
	AAIController* cachedAIController;

	/** The cached TargetActor */
	UPROPERTY(VisibleAnywhere)
	AActor* cachedTargetActor;

	/** Called when Task is finished or aborted */
	UFUNCTION(BlueprintCallable)
	void StopMovingTowardsActor();

};
