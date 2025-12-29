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
//#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	healthComponent = CreateDefaultSubobject<UAC_Health>(TEXT("Health Componenet"));
	pathNodeIndex = -1;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	healthComponent->SetHealth(enemyInfo->health);

	GetCharacterMovement()->MaxWalkSpeed = enemyInfo->movementSpeed;

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

	if (healthComponent->GetHealth() <= 0)
	{
		OnDeath();
	}
	return DamageAmount;
}

void AEnemyCharacterBase::OnDeath()
{
	OnEnemyDeathEvent.Broadcast(this);

	//If enemy was defeated
	if (healthComponent->GetHealth() <= 0)
	{
		SpawnDrop();

		AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
		ACore_GameState* coreGameState = Cast<ACore_GameState>(gameState);
		if (coreGameState)
		{
			UE_LOG(LogTemp, Display, TEXT("The amount of currency addded should be - %f"), enemyInfo->currencyOnDeath);
			coreGameState->UpdatePlayerCurrencyAmount(true, enemyInfo->currencyOnDeath);

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CORE GMAE STATE NOT CASTED CORRECTLY WITHIN - %s"), *this->GetName());
		}
	}

	DisableEnemy();
	//Destroy();
	/*
	UE_LOG(LogTemp, Warning, TEXT("Damage Amount = %f"), DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("EventInstigator = %s"), *EventInstigator->GetName())
	UE_LOG(LogTemp, Warning, TEXT("DamageCauser = %s"), *DamageCauser->GetName());
	*/
}

void AEnemyCharacterBase::DisableEnemy()
{
	GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->Deactivate();

	//AAIController* AIController = Cast<AAIController>(GetController());
	//AIController->UnPossess();

	if (GetController() && enemyAIController)
	{
		enemyAIController->UnPossess();
	}

	//SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	bIsDisabled = true;
}

void AEnemyCharacterBase::EnableEnemy()
{
	//GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	GetCharacterMovement()->Activate();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->Activate();

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

			FActorSpawnParameters spawnParams;
			FVector spawnLoc = GetActorLocation();
			float capsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			spawnLoc.Z -= capsuleHalfHeight - 50;

			AEnemyDrop* drop = GetWorld()->SpawnActor<AEnemyDrop>(AEnemyDrop::StaticClass(), spawnLoc, FRotator::ZeroRotator, spawnParams);
			drop->SetDrop(element.Key);
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

//int AEnemyCharacterBase::GetPathNodeIndex()
//{
//	return pathNodeIndex;
//}

void AEnemyCharacterBase::IncreasePathNodeIndex()
{
	pathNodeIndex++;
}