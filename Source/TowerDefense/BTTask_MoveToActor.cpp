#include "BTTask_MoveToActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UBTTask_MoveToActor::UBTTask_MoveToActor()
{
	NodeName = TEXT("Move To Actor");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_MoveToActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();

	if (!blackboardComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BLACKBOARD COMP NOT FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	//AActor* cachedTargetActor = Cast<AActor>(blackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	cachedTargetActor = Cast<AActor>(blackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!cachedTargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("cachedTargetActor NOT FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	//AAIController* cachedAIController = OwnerComp.GetAIOwner();
	cachedAIController = OwnerComp.GetAIOwner();
	if (!cachedAIController)
	{
		UE_LOG(LogTemp, Error, TEXT("cachedAIController NOT FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	//APawn* cachedControlledPawn = cachedAIController->GetPawn();
	cachedControlledPawn = cachedAIController->GetPawn();
	if (!cachedControlledPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("cachedControlledPawn NOT FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	cachedOwnerComp = &OwnerComp;
	LastTargetLocation = cachedTargetActor->GetActorLocation();
	cachedAIController->MoveToActor(cachedTargetActor, acceptanceRadius);

	cachedControlledPawn->GetWorldTimerManager().ClearTimer(MoveUpdateTimerHandle);
	cachedControlledPawn->GetWorldTimerManager().SetTimer(MoveUpdateTimerHandle, this, &UBTTask_MoveToActor::UpdateMove, UpdateInterval, true);

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToActor::UpdateMove()
{
	//UE_LOG(LogTemp, Warning, TEXT("ExecuteTask called, setting timer with UpdateInterval = %f"), UpdateInterval);
	//UE_LOG(LogTemp, Warning, TEXT("UpdateMove called at time: %f"), FPlatformTime::Seconds());


	if (!cachedTargetActor || !cachedControlledPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("cachedTargetActor OR cachedControlledPawn NOT FOUND WITHIN UPDATE MOVE FUNCTION - %s"), *this->GetName());
		FinishLatentTask(*cachedOwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector currentTargetLocation = cachedTargetActor->GetActorLocation();

	// Update move only if target moved more than 10 units
	if (!currentTargetLocation.Equals(LastTargetLocation, 10.f))
	{
		cachedAIController->MoveToActor(cachedTargetActor, acceptanceRadius);
		LastTargetLocation = currentTargetLocation;
	}

	float distanceFromActor = FVector::Dist(cachedControlledPawn->GetActorLocation(), currentTargetLocation);

	ACharacter* enemy = Cast<ACharacter>(cachedControlledPawn);
	float adjustedAcceptanceRadius = enemy->GetCapsuleComponent()->GetScaledCapsuleRadius() + acceptanceRadius + 3;
	//UE_LOG(LogTemp, Error, TEXT("distanceFromActor: %f, acceptanceRadius: %f, adjustedAcceptanceRadius: %f"), distanceFromActor, acceptanceRadius, adjustedAcceptanceRadius);

	if (distanceFromActor <= adjustedAcceptanceRadius)
	{
		FinishLatentTask(*cachedOwnerComp, EBTNodeResult::Succeeded);
		StopMovingTowardsActor();
		//cachedAIController->StopMovement();
		//// Reached target, stop timer and finish task successfully
		//cachedControlledPawn->GetWorldTimerManager().ClearTimer(MoveUpdateTimerHandle);
		//FinishLatentTask(*cachedOwnerComp, EBTNodeResult::Succeeded);
	
		//cachedOwnerComp = nullptr;
	}
}

void UBTTask_MoveToActor::StopMovingTowardsActor()
{
	cachedAIController->StopMovement();
	if (cachedControlledPawn)
	{
		cachedControlledPawn->GetWorldTimerManager().ClearTimer(MoveUpdateTimerHandle);
	}
	cachedOwnerComp = nullptr;
}

void UBTTask_MoveToActor::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	StopMovingTowardsActor();
}

EBTNodeResult::Type UBTTask_MoveToActor::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	StopMovingTowardsActor();
	return EBTNodeResult::Failed;
}
