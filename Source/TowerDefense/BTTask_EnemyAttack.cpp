#include "BTTask_EnemyAttack.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
	NodeName = "Enemy Attack Montage";
	bCreateNodeInstance = true;

}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (!attackMontages.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Error, TEXT("NO VALID ATTACK MONTAGES WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	cachedOwnerComp = &OwnerComp;

	APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PAWN FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	ACharacter* character = Cast<ACharacter>(pawn);

	if (!character)
	{
		UE_LOG(LogTemp, Error, TEXT("NO CHARACTER ACTOR FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	UAnimInstance* animInstance = character->GetMesh()->GetAnimInstance();

	if (!animInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("NO ANIM INSTANCE FOUND WITHIN - %s"), *this->GetName());
		return EBTNodeResult::Failed;
	}

	int ranNum = FMath::RandRange(0, attackMontages.Num() - 1);
	UAnimMontage* randomMontage = attackMontages[ranNum];

	EndMontageEvent.BindUObject(this, &UBTTask_EnemyAttack::OnMontageEnded);
	animInstance->Montage_SetEndDelegate(EndMontageEvent, randomMontage);


	animInstance->Montage_Play(randomMontage, 1);


	return EBTNodeResult::InProgress;
}

void UBTTask_EnemyAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
	if (!cachedOwnerComp)
	{
		UE_LOG(LogTemp, Error, TEXT("NO cachedOwnerComp FOUND WITHIN - %s"), *this->GetName());
		return;
	}

	EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Aborted : EBTNodeResult::Succeeded;
	FinishLatentTask(*cachedOwnerComp, Result);
}
