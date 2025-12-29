#include "SpawnerManager.h"
#include <Kismet/GameplayStatics.h>
#include "EnemySpawner.h"

ASpawnerManager::ASpawnerManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	waveActive = false;
	amountOfEnemysInRound = 0;

	SetAllSpawners();
	//PoolEnemies();

}

void ASpawnerManager::StartSpawningEnemies(int currentWave)
{
	//if the spawners haven't been set, set the spawners
	/*if (!enemySpawners.IsValidIndex(0))
	{
		SetAllSpawners();
	}*/

	//If there is a wave currently active, prevent spawning from another wave
	if (waveActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave is currently active!"));
		return;
	}

	waveActive = true;
	amountOfEnemysInRound = 0;
	UE_LOG(LogTemp, Warning, TEXT("current wave = %d"), currentWave);

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
			UE_LOG(LogTemp, Warning, TEXT("No Spawner Found when spawning calulcating amount of enemies within - %s"), *this->GetName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("There are %d enemies this round"), amountOfEnemysInRound);
};

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
			UE_LOG(LogTemp, Error, TEXT("Cast to Enemy Spawner Failed within - %s!"), *this->GetName());
		}
	}

	//Bind the spawners OnEnemySpawnedEvent Delegate for when an enemy is spawned within the spawner, then this can set the delegate within the enemy spawned
	for (AEnemySpawner* elememt : enemySpawners)
	{
		elememt->OnEnemySpawnedEvent.AddDynamic(this, &ASpawnerManager::BindDelegateOnEnemy);
		UE_LOG(LogTemp, Warning, TEXT("%s spawning delegate has been bound in the spawner manager"), *elememt->GetName());
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %d actors from class AEnemySpawner"), enemySpawners.Num());
}

//Called Within Game Mode
int ASpawnerManager::CalculateLastWave()
{
	int maxWave = 0;
	for (AEnemySpawner* spawner : enemySpawners)
	{
		for (auto element : spawner->waveAndEnemyQueue)
		{
			//UE_LOG(LogTemp, Display, TEXT("Key/Wave is %d"), element.Key);
			if (element.Key > maxWave)
			{
				maxWave = element.Key;
				//UE_LOG(LogTemp, Display, TEXT("Last Wave = % d"), maxWave);
			}
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Last Wave = % d"), maxWave);
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
		enemy->OnEnemyDeathEvent.AddDynamic(this, &ASpawnerManager::EnemyHasDied);
		UE_LOG(LogTemp, Display, TEXT("%s Delegate has been bound in Spawner Manager"), *enemy->GetName());
	}
	else if(enemy->OnEnemyDeathEvent.IsBound())
	{
		UE_LOG(LogTemp, Display, TEXT("Enemy OnEnemyDeathEvent delegate is already bound to - %s"), *this->GetName());

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Binding Delegate to spawner manager not bound correctly"));
	}
}

void ASpawnerManager::EnemyHasDied(AEnemyCharacterBase* enemy)
{
	amountOfEnemysInRound--;
	UE_LOG(LogTemp, Warning, TEXT("Amount of enemies left in the round = %d"), amountOfEnemysInRound);

	if (amountOfEnemysInRound <= 0)
	{
		WaveEndedEvent.Broadcast();
		waveActive = false;
	}
}