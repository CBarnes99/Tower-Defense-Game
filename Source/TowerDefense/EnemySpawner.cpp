#include "EnemySpawner.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyPathSpline.h"
#include "Components/StaticMeshComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	spawnCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Collision"));
	spawnCollision->SetSphereRadius(150.f);

	spawnerIdentifier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawner Identifier"));
	spawnerIdentifier->SetCanEverAffectNavigation(false);

	RootComponent = spawnCollision;
	spawnerIdentifier->SetupAttachment(RootComponent);

	spawnInterval = 0.5f; //Spawns every 0.5 seconds
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	spawnerIdentifier->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spawnerIdentifier->SetVisibility(false);
	spawnerIdentifier->SetComponentTickEnabled(false);

	isSpawning = false;

	if (!IsThereEnemyPathSplines()) 
	{
		UE_LOG(LogTemp, Fatal, TEXT("BeginPlay: THERE IS NOT ANY ENEMY PATH SPLINES ATTACHED TO THIS SPAWNER - %s"), *this->GetName());
	}

	PoolEnemies();
}

bool AEnemySpawner::IsThereEnemyPathSplines()
{
	if (enemyPathSplines[0] != nullptr)
	{
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("IsThereEnemyPathSplines: THERE IS NO ENEMY SPLINE PATHS ATTACHED TO - %s"), *this->GetName());
	return false;
}

AEnemyPathSpline* AEnemySpawner::GetRandomEnemyPath()
{
	int arraySize = enemyPathSplines.Num();
	int randomInt = FMath::RandRange(0, arraySize - 1);
	//UE_LOG(LogTemp, Warning, TEXT("GetRandomEnemyPath: Returning - %s - from GetRandomEnemyPath within - %s"), *enemyPathSplines[randomInt]->GetName(), *this->GetName());
	return enemyPathSplines[randomInt];
}

void AEnemySpawner::StartSpawning()
{
	if (!isSpawning && IsThereEnemyPathSplines())
	{
		isSpawning = true;
		GetWorld()->GetTimerManager().SetTimer(spawnEnemyTimerHandle, this, &AEnemySpawner::SpawningFunctionForTimer, spawnInterval, true);
	}
}

void AEnemySpawner::StopSpawning()
{
	if (isSpawning && IsThereEnemyPathSplines())
	{
		isSpawning = false;
		GetWorld()->GetTimerManager().ClearTimer(spawnEnemyTimerHandle);
	}
}

void AEnemySpawner::SpawningFunctionForTimer()
{
	if (IsEnemyCollisionOverlap())
	{
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

void AEnemySpawner::PoolEnemies()
{
	// A count of the max amount of enemies per type that can spawn in a level
	TMap<TSubclassOf<AEnemyCharacterBase>, int> amountOfEachEnemyClass;

	for (TPair<int, FAmountOfEnemysSpawning> element : waveAndEnemyQueue)
	{
		//The max count of enemies in a single wave
		TMap<TSubclassOf<AEnemyCharacterBase>, int> tempMaxCountMap;

		// Counting how many enemies of each type is in a wave
		for (TSubclassOf<AEnemyCharacterBase> enemyClass : element.Value.enemyTypeArray)
		{
			if (int* valuePtr = tempMaxCountMap.Find(enemyClass))
			{
				++(*valuePtr);
			}
			else
			{
				tempMaxCountMap.Add(enemyClass, 1);
			}
		}

		// Checks amountOfEachEnemyClass to see if the amount of enemies in round is greater than what is already stored
		// if so, replace the old value with the new value
		for (TPair<TSubclassOf<AEnemyCharacterBase>, int>& tempPair : tempMaxCountMap)
		{
			int* valuePtr = amountOfEachEnemyClass.Find(tempPair.Key);

			if (!valuePtr || *valuePtr < tempPair.Value)
			{
				amountOfEachEnemyClass.Add(tempPair.Key, tempPair.Value);
			}
		}
	}

	TArray<TSubclassOf<AEnemyCharacterBase>> enemyClasses;
	amountOfEachEnemyClass.GetKeys(enemyClasses);

	// Spawns and pools the amount of enemies the spawner needs for the level
	for (TSubclassOf<AEnemyCharacterBase> enemyClass : enemyClasses)
	{
		if (enemyClass == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PoolEnemies: Enemy Class was Nullptr! Not all enemies were spawned!"));
			break;
		}

		int* amountToSpawnPtr = amountOfEachEnemyClass.Find(enemyClass);

		FActorSpawnParameters spawnParams;
		spawnParams.Instigator = GetInstigator();

		for (int i = 0; i < *amountToSpawnPtr; i++)
		{
			
			AEnemyCharacterBase* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(enemyClass, this->GetActorLocation(), this->GetActorRotation(), spawnParams);
			spawnedEnemy->DisableEnemy();
			enemyPool.Add(spawnedEnemy);
		}
	}
}

void AEnemySpawner::UpdateEnemyPerception(bool bIsPlayerDefeated)
{
	for (AEnemyCharacterBase* enemy : enemyPool)
	{
		if (!enemy->GetIsEnemyDisabled())
		{
			enemy->DisableOrEnablePerceptionComponent(bIsPlayerDefeated);
		}
	}
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

			if (enemyStruct->enemyTypeArray[0] == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("SpawnEnemyActor: Enemy within spawner is NullPtr, enemies haven't all spawned"));
				return nullptr;
			}
			amountOfEnemiesSpawned++;
			//UE_LOG(LogTemp, Display, TEXT("SpawnEnemyActor: Valid Index 0 In %s"), *this->GetName());

			for (AEnemyCharacterBase* pooledEnemy : enemyPool)
			{
				if (pooledEnemy->GetIsEnemyDisabled() && pooledEnemy->IsA(enemyStruct->enemyTypeArray[0]))
				{
					pooledEnemy->SetPathNodeLocations(GetRandomEnemyPath()->GetSplinePointLocations());
					enemyStruct->enemyTypeArray.RemoveAt(0);
					OnEnemySpawnedEvent.Broadcast(pooledEnemy);
					pooledEnemy->SetActorLocation(this->GetActorLocation());
					pooledEnemy->EnableEnemy();
					return pooledEnemy;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnEnemyActor: Amount Of Enemies in %s is %d, Amount Spawned = %d"), *this->GetName(), amountOfEnemiesInWave, amountOfEnemiesSpawned);
			StopSpawning();
			return NULL;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnEnemyActor: No enemies spawning in %s for wave %d"), *this->GetName(), currentWaveBeingSpawned);
		StopSpawning();
		return NULL;
	}
	return NULL;
}



