#include "EnemyCharacterBase.h"
#include "Containers/Map.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyDrop.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core_GameState.h"
#include "DA_EnemyCharacterStats.h"
#include "AC_Health.h"
#include "E_EnemyDrop.h"
#include "EnemyAIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source Component"));
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->RegisterWithPerceptionSystem();

	healthComponent = CreateDefaultSubobject<UAC_Health>(TEXT("Health Componenet"));

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.f;
	GetCharacterMovement()->SetAvoidanceGroup(1 << 0);
	GetCharacterMovement()->SetGroupsToAvoid(1 << 0);

}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	pathNodeIndex = -1;

	healthComponent->SetHealth(enemyInfo->health);
	GetCharacterMovement()->MaxWalkSpeed = enemyInfo->movementSpeed;

	//Spawns An AIController and uses this to possess and unposesss the actor when they are disabled and moved to the pool
	enemyAIController = GetWorld()->SpawnActor<AAIController>(enemyInfo->AIControllerClass);
}

UBehaviorTree* AEnemyCharacterBase::GetBehaviourTree() const
{
	return enemyBehaviorTree;
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	healthComponent->RecieveDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (healthComponent->GetCurrentHealth() <= 0)
	{
		OnDeath();
	}
	return DamageAmount;
}

float AEnemyCharacterBase::GetEnemyDamageAmount()
{
	return enemyInfo->damageDelt;
}

void AEnemyCharacterBase::OnDeath()
{
	OnEnemyDeathEvent.Broadcast(this);
	OnEnemyDeathBlueprintEvent.Broadcast(this);

	//If enemy was defeated and not just reached the end of the level
	if (healthComponent->GetCurrentHealth() <= 0)
	{
		SpawnDrop();

		AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
		ACore_GameState* coreGameState = Cast<ACore_GameState>(gameState);
		if (coreGameState)
		{
			//UE_LOG(LogTemp, Display, TEXT("OnDeath: Currency Added - %d"), enemyInfo->currencyOnDeath);
			coreGameState->UpdatePlayerCurrencyAmount(true, enemyInfo->currencyOnDeath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnDeath: CORE GMAE STATE NOT CASTED CORRECTLY WITHIN - %s"), *this->GetName());
		}
	}
	else 
	{
		DisableEnemy();
	}
}

//This is called when the death animation has started playing so that you cant interupt the animation
void AEnemyCharacterBase::OnDeathAnimationStarted()
{
	if (GetController() && enemyAIController)
	{
		OnDisableAIControllerEvent.ExecuteIfBound();
		enemyAIController->UnPossess();
	}
	SetActorEnableCollision(false);
	GetCharacterMovement()->SetAvoidanceGroup(1 << 1);
}
void AEnemyCharacterBase::DisableEnemy()
{
	GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnEnemyDisabledBlueprintEvent.Broadcast();
	GetMesh()->Deactivate();

	StimuliSourceComponent->Deactivate();
	StimuliSourceComponent->UnregisterFromPerceptionSystem();

	if (GetController() && enemyAIController)
	{
		OnDisableAIControllerEvent.ExecuteIfBound();
		enemyAIController->UnPossess();
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	bIsDisabled = true;
}

void AEnemyCharacterBase::EnableEnemy()
{

	GetCharacterMovement()->Activate();
	GetCharacterMovement()->SetAvoidanceGroup(1 << 0);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->Activate(true);

	StimuliSourceComponent->Activate();
	StimuliSourceComponent->RegisterWithPerceptionSystem();

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	healthComponent->ResetHealth();
	pathNodeIndex = -1;

	if (!GetController() && enemyAIController)
	{
		enemyAIController->Possess(this);
		enemyAIController->RunBehaviorTree(enemyBehaviorTree);
	}

	bIsDisabled = false;

}

bool AEnemyCharacterBase::GetIsEnemyDisabled()
{
	return bIsDisabled;
}

void AEnemyCharacterBase::SpawnDrop()
{
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

			FVector spawnLoc = GetActorLocation();
			float capsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			spawnLoc.Z -= capsuleHalfHeight -50;

			OnSpawnEnemyDropEvent.ExecuteIfBound(element.Key, spawnLoc);
			break;
		}
	}
}

void AEnemyCharacterBase::SetPathNodeLocations(TArray<FVector> nodeLocations)
{
	pathNodeLocations = nodeLocations;
}

FVector AEnemyCharacterBase::GetNextPathNodeLocation()
{
	IncreasePathNodeIndex();
	if (pathNodeLocations.IsValidIndex(pathNodeIndex))
	{
		return pathNodeLocations[pathNodeIndex];
	}
	else
	{
		return FVector::ZeroVector;
	}

}

FVector AEnemyCharacterBase::GetClosestPathNodeLocation()
{
	float distanceFromClosestNode = -1.f;
	FVector closestPathNode = FVector::ZeroVector;
	int nodeIndex = -1;
	for (int i = 0; i < pathNodeLocations.Num() - 1; i++)
	{
		if (FVector::Dist(GetActorLocation(), pathNodeLocations[i]) < distanceFromClosestNode || distanceFromClosestNode < 0)
		{
			distanceFromClosestNode = FVector::Dist(GetActorLocation(), pathNodeLocations[i]);
			closestPathNode = pathNodeLocations[i];
			nodeIndex = i;
		}
	}
	pathNodeIndex = nodeIndex;
	UE_LOG(LogTemp, Display, TEXT("GetClosestPathNodeLocation: New closest node is - %d, within - %s"), pathNodeIndex, *this->GetName());
	return closestPathNode;
}

void AEnemyCharacterBase::IncreasePathNodeIndex()
{
	pathNodeIndex++;
}