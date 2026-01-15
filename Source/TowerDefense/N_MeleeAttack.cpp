#include "N_MeleeAttack.h"
#include "EnemyCharacterBase.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UN_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (!MeshComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Notify: NO MESH WITHIN - %s"), *this->GetName());
		return;
	}

	if (attackSocketName.IsNone() || !MeshComp->DoesSocketExist(attackSocketName))
	{
		UE_LOG(LogTemp, Error, TEXT("Notify: ATTACK SOCKET NAME NOT SET CORRECTLY WITHIN - %s, %s, %s"), *MeshComp->GetOwner()->GetName(), *Animation->GetName(), *this->GetName());
		return;
	}

	AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(MeshComp->GetOwner());

	if (!enemy)
	{
		UE_LOG(LogTemp, Error, TEXT("Notify: CASTED TO ENEMY FAILED WITHIN - %s"), *this->GetName());
		return;
	}

	if (!enemy->HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("Notify: ENEMY DOEST NOT HAVE AUTHORITY WITHIN - %s"), *this->GetName());
		return;
	}

	if (traceRadius <= 1.f)
	{
		traceRadius = 50.f;
	}

	FVector boneLocation = MeshComp->GetSocketLocation(attackSocketName);

	TArray<FHitResult> hitResults;
	FCollisionShape sphereCollision = FCollisionShape::MakeSphere(traceRadius);

	bool bHit = enemy->GetWorld()->SweepMultiByChannel(hitResults, boneLocation, boneLocation, FQuat::Identity, ECC_Pawn, sphereCollision);
	DrawDebugSphere(GetWorld(), boneLocation, traceRadius, 12, FColor::Green, false, 1.f);


	for (auto& hit : hitResults)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(hit.GetActor());
		if (player)
		{
			UGameplayStatics::ApplyDamage(player, enemy->GetEnemyDamageAmount(), enemy->GetController(), enemy, nullptr);
			//UE_LOG(LogTemp, Warning, TEXT("Notify: Enemy Has Attacked Player!!"));
			return;
		}
	}
}
