// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

protected:

	virtual void OnPossess(APawn* InPawn) override;

	FString NameOfBase = "Target";


	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* EnemyBehaviourTree;
};
