// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawnInterval = 0.5f; //Spawns every 0.5 seconds
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	isSpawning = false;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//When Enter is pressed in combat player controller, this function starts
void AEnemySpawner::StartSpawning()
{
	if (!isSpawning)
	{
		isSpawning = true;
		GetWorld()->GetTimerManager().SetTimer(spawnEnemyTimerHandle, this, &AEnemySpawner::SpawningFunctionForTimer, spawnInterval, true);
	}
}

//When all enemies have spawned, stop spawning
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
	SpawnEnemyActor();
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
		return amount;
	}
	else 
	{
		return 0;
	}
}

//Spawns the Enemy Actors from the Array of characters that are assigned in the BP Editor
AEnemyCharacterBase* AEnemySpawner::SpawnEnemyActor()
{
	if (FAmountOfEnemysSpawning* enemyStruct = waveAndEnemyQueue.Find(currentWaveBeingSpawned))
	{
		if (enemyStruct->enemyTypeArray.IsValidIndex(0)) {

			UE_LOG(LogTemp, Display, TEXT("Valid Index 0 In %s"), *this->GetName());
			FActorSpawnParameters spawnParams;
			spawnParams.Instigator = GetInstigator();

			AEnemyCharacterBase* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(enemyStruct->enemyTypeArray[0].Get(), this->GetActorLocation(), this->GetActorRotation(), spawnParams);
			enemyStruct->enemyTypeArray.RemoveAt(0);
			OnEnemySpawnedEvent.Broadcast(spawnedEnemy);
			return spawnedEnemy;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NO Valid Index In %s"), *this->GetName());
			StopSpawning();
			return NULL;
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO Valid Index In waveAndEnemyQueue"));
		StopSpawning();
		return NULL;
	}
}

