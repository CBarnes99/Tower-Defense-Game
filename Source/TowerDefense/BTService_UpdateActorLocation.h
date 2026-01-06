#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_UpdateActorLocation.generated.h"

UCLASS()
class TOWERDEFENSE_API UBTService_UpdateActorLocation : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateActorLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LocationKey;

};
