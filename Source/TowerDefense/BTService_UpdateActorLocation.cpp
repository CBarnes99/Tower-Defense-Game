#include "BTService_UpdateActorLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateActorLocation::UBTService_UpdateActorLocation()
{
	NodeName = TEXT("Update Actor Location");
	Interval = 0.2f;
}

void UBTService_UpdateActorLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* actor = Cast<AActor>(blackboardComp->GetValueAsObject(ActorKey.SelectedKeyName));
		if (actor)
		{
			blackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, actor->GetActorLocation());
		}
	}
}
