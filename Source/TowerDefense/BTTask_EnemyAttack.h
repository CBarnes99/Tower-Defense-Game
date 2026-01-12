#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyAttack.generated.h"

class UAnimMontage;
class UBehaviorTreeComponent;

DECLARE_DELEGATE_TwoParams(FOnMontageEndedSigniture, UAnimMontage* /**/, bool);

UCLASS()
class TOWERDEFENSE_API UBTTask_EnemyAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EnemyAttack();

	UPROPERTY(EditAnywhere, Category = "AttackAnimation")
	TArray<UAnimMontage*> attackMontages;

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	UBehaviorTreeComponent* cachedOwnerComp;

	FOnMontageEndedSigniture EndMontageEvent;


};
