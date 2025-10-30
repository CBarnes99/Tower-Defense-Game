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
	isSpawning = true;
	GetWorld()->GetTimerManager().SetTimer(spawnEnemyTimerHandle, this, &AEnemySpawner::Spawning, spawnInterval, true);
}

//When all enemies have spawned, stop spawning
void AEnemySpawner::StopSpawning()
{
	isSpawning = false;
	GetWorld()->GetTimerManager().ClearTimer(spawnEnemyTimerHandle);
}

void AEnemySpawner::Spawning()
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

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Valid Index In Spawner "));
			FActorSpawnParameters spawnParams;
			spawnParams.Instigator = GetInstigator();

			AEnemyCharacterBase* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(enemyStruct->enemyTypeArray[0].Get(), this->GetActorLocation(), this->GetActorRotation(), spawnParams);
			enemyStruct->enemyTypeArray.RemoveAt(0);
			//OnEnemySpawnedEvent.Broadcast(Cast<AEnemyCharacterBase>(spawnedEnemy));
			OnEnemySpawnedEvent.Broadcast(spawnedEnemy);
			return spawnedEnemy;
		}
		else
		{
			errorSpawningLog("NO Valid Index In Spawner");
			return NULL;
		}
	}

	else
	{
		errorSpawningLog("NO Valid Index In waveAndEnemyQueue");
		return NULL;
	}
}

void AEnemySpawner::errorSpawningLog(FString log)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, log);
	StopSpawning();
	isSpawning = false;
}

