#include "EnemySpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	spawnCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Collision"));
	spawnCollision->SetSphereRadius(150.f);

	spawnInterval = 0.5f; //Spawns every 0.5 seconds
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	isSpawning = false;
}

void AEnemySpawner::StartSpawning()
{
	if (!isSpawning)
	{
		isSpawning = true;
		GetWorld()->GetTimerManager().SetTimer(spawnEnemyTimerHandle, this, &AEnemySpawner::SpawningFunctionForTimer, spawnInterval, true);
	}
}

void AEnemySpawner::StopSpawning()
{
	if (isSpawning)
	{
		isSpawning = false;
		GetWorld()->GetTimerManager().ClearTimer(spawnEnemyTimerHandle);
	}
}

void AEnemySpawner::SpawningFunctionForTimer()
{
	if (IsEnemyCollisionOverlap())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enemy In %s Collision Area"), *this->GetName());
		return;
	}

	SpawnEnemyActor();
}

bool AEnemySpawner::IsEnemyCollisionOverlap()
{
	TArray<AActor*> overlappingActors;
	spawnCollision->GetOverlappingActors(overlappingActors);
	
	for (AActor* actor : overlappingActors)
	{
		if (Cast<AEnemyCharacterBase>(actor))
		{
			return true;
		}
	}
	return false;
}

int AEnemySpawner::CalculateAmountOfEnemiesInWave()
{
	if (FAmountOfEnemysSpawning* enemyStruct = waveAndEnemyQueue.Find(currentWaveBeingSpawned))
	{
		int amount = 0;
		for (TSubclassOf<AEnemyCharacterBase> enemys : enemyStruct->enemyTypeArray)
		{
			amount++;
		}
		//Store how many enemies are in the wave and how many have spawned for debugging
		amountOfEnemiesInWave = amount;
		amountOfEnemiesSpawned = 0;

		return amount;
	}
	else 
	{
		return 0;
	}
}

AEnemyCharacterBase* AEnemySpawner::SpawnEnemyActor()
{
	if (FAmountOfEnemysSpawning* enemyStruct = waveAndEnemyQueue.Find(currentWaveBeingSpawned))
	{
		if (enemyStruct->enemyTypeArray.IsValidIndex(0)) {
			amountOfEnemiesSpawned++;
			//UE_LOG(LogTemp, Display, TEXT("Valid Index 0 In %s"), *this->GetName());
			FActorSpawnParameters spawnParams;
			spawnParams.Instigator = GetInstigator();

			AEnemyCharacterBase* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(enemyStruct->enemyTypeArray[0].Get(), this->GetActorLocation(), this->GetActorRotation(), spawnParams);
			enemyStruct->enemyTypeArray.RemoveAt(0);
			OnEnemySpawnedEvent.Broadcast(spawnedEnemy);
			return spawnedEnemy;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("NO Valid Index In %s"), *this->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Amount Of Enemies in %s is %d, Amount Spawned = %d"), *this->GetName(), amountOfEnemiesInWave, amountOfEnemiesSpawned);
			StopSpawning();
			return NULL;
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No enemies spawning in %s for wave %d"), *this->GetName(), currentWaveBeingSpawned);
		StopSpawning();
		return NULL;
	}
}



