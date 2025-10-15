// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindBaseLocation.h"
#include <Kismet/GameplayStatics.h>
#include "DefendingBase.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_FindBaseLocation::UBTTask_FindBaseLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Base Location");
}

EBTNodeResult::Type UBTTask_FindBaseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* FoundBase = UGameplayStatics::GetActorOfClass(GetWorld(), ADefendingBase::StaticClass());

	ADefendingBase* Base = Cast<ADefendingBase>(FoundBase);
	
	if (Base)
	{
		//Get Base Location and sets Blackboard key Vector to the Base Location
		auto const BaseLocation = Base->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), BaseLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


