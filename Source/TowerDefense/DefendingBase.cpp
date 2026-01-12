#include "DefendingBase.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacterBase.h"

ADefendingBase::ADefendingBase()
{
	PrimaryActorTick.bCanEverTick = false;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = baseMesh;

	enemyCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	enemyCollisionDetection->SetupAttachment(baseMesh);

	enemyCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ADefendingBase::OnOverlap);


	baseHealth = 20;

}

void ADefendingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADefendingBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemyCharacterBase::StaticClass()))
	{
		baseHealth--;
		BaseHealthDecreasedEvent.ExecuteIfBound(baseHealth);
		UE_LOG(LogTemp, Display, TEXT("Base Health Decreaced to - %d"), baseHealth);
		
		AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(OtherActor);
		enemy->OnDeath();
	}
	
	if (baseHealth <= 0)
	{
		BaseHealthReachedZeroEvent.ExecuteIfBound();
		UE_LOG(LogTemp, Warning, TEXT("Base Health Has Reached 0!!"));
	}
}

int ADefendingBase::GetBaseHealth()
{
	return baseHealth;
}

