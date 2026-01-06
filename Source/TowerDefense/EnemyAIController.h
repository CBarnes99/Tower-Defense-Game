#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;

UCLASS()
class TOWERDEFENSE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:

	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBehaviorTree* cachedBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBlackboardComponent* cachedAIBlackboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* senseSightConfig;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus);

	UFUNCTION(BlueprintCallable)
	void DisableAIController();
};
