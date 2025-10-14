// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyCharacterBase.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	
	if (AEnemyCharacterBase* const Enemy = Cast<AEnemyCharacterBase>(InPawn))
	{
		if (UBehaviorTree* const Tree = Enemy->GetBehaviourTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(Tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(Tree);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("BLACKBOARD WORKING IN ENEMY AI CONTROLLER"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, TEXT("BLACKBOARD NULL IN ENEMY AI CONTROLLER"));
		}
	}
}