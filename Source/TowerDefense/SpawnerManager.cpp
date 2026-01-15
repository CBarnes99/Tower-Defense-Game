#include "SpawnerManager.h"
#include <Kismet/GameplayStatics.h>
#include "EnemySpawner.h"
#include "EnemyDrop.h"

ASpawnerManager::ASpawnerManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	waveActive = false;
	amountOfEnemysInRound = 0;
	amountOfDropToPool = 15;

	SetAllSpawners();
	PoolEnemyDrop();
}

void ASpawnerManager::SetAllSpawners()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), foundActors);

	enemySpawners.Empty();
	for (AActor* elements : foundActors)
	{
		if (AEnemySpawner* spawner = Cast<AEnemySpawner>(elements))
		{
			enemySpawners.Add(spawner);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SetAllSpawners: Cast to Enemy Spawner Failed within - %s!"), *this->GetName());
		}
	}

	//Bind the spawners OnEnemySpawnedEvent Delegate for when an enemy is spawned within the spawner, then this can set the delegate within the enemy spawned
	for (AEnemySpawner* elememt : enemySpawners)
	{
		elememt->OnEnemySpawnedEvent.AddDynamic(this, &ASpawnerManager::BindDelegateOnEnemy);
		UE_LOG(LogTemp, Display, TEXT("SetAllSpawners: %s spawning delegate has been bound in the spawner manager"), *elememt->GetName());
	}

	UE_LOG(LogTemp, Display, TEXT("SetAllSpawners: Found %d actors from class AEnemySpawner"), enemySpawners.Num());
}

void ASpawnerManager::StartSpawningEnemies(int currentWave)
{
	//If there is a wave currently active, prevent spawning from another wave
	if (waveActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSpawningEnemies: Wave is currently active!"));
		return;
	}

	waveActive = true;
	amountOfEnemysInRound = 0;
	UE_LOG(LogTemp, Warning, TEXT("StartSpawningEnemies: Current Wave = %d"), currentWave);

	//Adds to the total of enemies that are in this round and then starts spawning enemies
	for (AEnemySpawner* spawner : enemySpawners)
	{
		if (spawner)
		{
			spawner->currentWaveBeingSpawned = currentWave;
			amountOfEnemysInRound += spawner->CalculateAmountOfEnemiesInWave();
			spawner->StartSpawning();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StartSpawningEnemies: No Spawner Found when spawning calulcating amount of enemies within - %s"), *this->GetName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("StartSpawningEnemies: There are %d enemies this round"), amountOfEnemysInRound);
};

//Called Within Game Mode
int ASpawnerManager::CalculateLastWave()
{
	int maxWave = 0;
	for (AEnemySpawner* spawner : enemySpawners)
	{
		for (auto element : spawner->waveAndEnemyQueue)
		{
			//UE_LOG(LogTemp, Display, TEXT("CalculateLastWave: Key/Wave is %d"), element.Key);
			if (element.Key > maxWave)
			{
				maxWave = element.Key;
				//UE_LOG(LogTemp, Display, TEXT("CalculateLastWave: Last Wave = % d"), maxWave);
			}
		}
	}
	UE_LOG(LogTemp, Display, TEXT("CalculateLastWave:; Last Wave = % d"), maxWave);
	return maxWave;
}

bool ASpawnerManager::IsWaveActive()
{
	return waveActive;
}

//Bind the enemy spawned so that when it is destoryed, it reduces the count for amount of enemies in the round
void ASpawnerManager::BindDelegateOnEnemy(AEnemyCharacterBase* enemy)
{
	if (enemy && !enemy->OnEnemyDeathEvent.IsBound())
	{
		enemy->OnEnemyDeathEvent.AddUObject(this, &ASpawnerManager::EnemyHasDied);
		//UE_LOG(LogTemp, Display, TEXT("BindDelegateOnEnemy: %s On Death Delegate has been bound in Spawner Manager"), *enemy->GetName());
	}
	//else if(enemy->OnEnemyDeathEvent.IsBound())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("BindDelegateOnEnemy: Enemy OnEnemyDeathEvent delegate is already bound to - %s"), *this->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("BindDelegateOnEnemy: Binding Delegate to spawner manager not bound correctly"));
	//}

	if (enemy && !enemy->OnSpawnEnemyDropEvent.IsBound())
	{
		enemy->OnSpawnEnemyDropEvent.BindUObject(this, &ASpawnerManager::SetEnemyDrop);
		//UE_LOG(LogTemp, Display, TEXT("BindDelegateOnEnemy: %s Spawn Drop Delegate has been bound in Spawner Manager"), *enemy->GetName());
	}
}

void ASpawnerManager::EnemyHasDied(AEnemyCharacterBase* enemy)
{
	amountOfEnemysInRound--;
	UE_LOG(LogTemp, Warning, TEXT("EnemyHasDied: Amount of enemies left in the round = %d"), amountOfEnemysInRound);

	if (amountOfEnemysInRound <= 0)
	{
		WaveEndedEvent.Broadcast();
		waveActive = false;
	}

}

void ASpawnerManager::PoolEnemyDrop()
{
	if (!enemyDropBlueprintClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PoolEnemyDrop: enemyDropBlueprintClass NOT SET WITHIN SPAWNER MANAGER!"));
		return;
	}

	FActorSpawnParameters spawnParams;
	int amountPooled = 0;
	for (int i = 0; i < amountOfDropToPool; i++)
	{
		AEnemyDrop* drop = GetWorld()->SpawnActor<AEnemyDrop>(enemyDropBlueprintClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		enemyDropPool.Add(drop);
		amountPooled++;
	}
	UE_LOG(LogTemp, Display, TEXT("PoolEnemyDrop: Amount of drops pooled - %d, out of - %d"), amountPooled, amountOfDropToPool);
}

void ASpawnerManager::SetEnemyDrop(EEnemyDrop dropType, FVector spawnLocation)
{
	UE_LOG(LogTemp, Display, TEXT("SetEnemyDrop: Drop Spawn Location - %s"), *spawnLocation.ToString());
	for (AEnemyDrop* drop : enemyDropPool)
	{
		if (drop->IsDropDisabled())
		{
			drop->EnableDrop(dropType, spawnLocation);
			break;
		}
	}
}

void ASpawnerManager::ShouldEnemiesPerceptPlayer(bool bIsPlayerDefeated)
{
	for (AEnemySpawner* spawners : enemySpawners)
	{
		spawners->UpdateEnemyPerception(bIsPlayerDefeated);
	}
}
