#include "EnemyCharacterBase.h"
#include "Containers/Map.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyDrop.h"
#include "Components/CapsuleComponent.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	healthComponent = CreateDefaultSubobject<UAC_Health>(TEXT("Health Componenet"));
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	healthComponent->SetHealth(enemyInfo->health);

	GetCharacterMovement()->MaxWalkSpeed = enemyInfo->movementSpeed;
}

UBehaviorTree* AEnemyCharacterBase::GetBehaviourTree() const
{
	return enemyBehaviorTree;
}
// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	healthComponent->RecieveDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (healthComponent->GetHealth() <= 0)
	{
		OnDeath();
	}
	return DamageAmount;
}

void AEnemyCharacterBase::OnDeath()
{
	OnEnemyDeathEvent.Broadcast(this);

	SpawnDrop();

	Destroy();
	/*
	UE_LOG(LogTemp, Warning, TEXT("Damage Amount = %f"), DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("EventInstigator = %s"), *EventInstigator->GetName())
	UE_LOG(LogTemp, Warning, TEXT("DamageCauser = %s"), *DamageCauser->GetName());
	*/
}

void AEnemyCharacterBase::SpawnDrop()
{
	/*if (!enemyDrop)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy Drop Isn't Set!"));
		return;
	}*/

	float dropWeightTotal = 0.f;

	for (const TPair<EEnemyDrop, float>& element : dropWeight)
	{
		dropWeightTotal += element.Value;
	}

	float randomWeight = FMath::FRandRange(0.f, dropWeightTotal);
	float accumulatedWeight = 0.f;

	for (const TPair<EEnemyDrop, float>& element : dropWeight)
	{
		accumulatedWeight += element.Value;
		if (randomWeight <= accumulatedWeight)
		{
			if (element.Key == EEnemyDrop::NONE) break;

			FActorSpawnParameters spawnParams;
			FVector spawnLoc = GetActorLocation();
			float capsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			spawnLoc.Z -= capsuleHalfHeight;

			AEnemyDrop* drop = GetWorld()->SpawnActor<AEnemyDrop>(AEnemyDrop::StaticClass(), spawnLoc, FRotator::ZeroRotator, spawnParams);
			drop->SetDrop(element.Key);
			break;
		}
	}
}
