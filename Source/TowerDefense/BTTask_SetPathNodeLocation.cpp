#include "BTTask_SetPathNodeLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "EnemyCharacterBase.h"

UBTTask_SetPathNodeLocation::UBTTask_SetPathNodeLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Next Path Node Location");
}

EBTNodeResult::Type UBTTask_SetPathNodeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("No AI Controller Found within - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	APawn* enemyAsPawn = AIController->GetPawn();
	if (!enemyAsPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No Controlled Pawn Failed within - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	AEnemyCharacterBase* enemyCharacter = Cast<AEnemyCharacterBase>(enemyAsPawn);
	if (!enemyCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn isn't of type AEnemyCharacterBase within - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	FVector nextPathNodeLocation = enemyCharacter->GetNextPathNodeLocation();
	if (nextPathNodeLocation == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("No more node locations within - %s"), *this->GetName());
		FName pathNodesEnded = "PathNodesEnded";
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(pathNodesEnded, true);
		return EBTNodeResult::Failed;
	}

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("bHasReachedNode"))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), nextPathNodeLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("bHasReachedNode", false);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
