// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Spawning Collision"));
	SpawningCollision->InitSphereRadius(400.f);

	checkInterval = 0.5f; //Checks every 0.5 seconds

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//Start periodic check and spawn timer
	



	isSpawning = true;

	currentWave = 1;
	//spawnAmount = waveAndEnemys.Contains(currentWave);
	//spawnAmount = 5;

	/*for (int i = 0; i < spawnAmount; i++)
	{
		SpawnEnemyActor();
	}*/
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::StartSpawning()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnCheckTimerHandle, this, &AEnemySpawner::Spawning, checkInterval, true);
}


void AEnemySpawner::Spawning()
{
	SpawnEnemyActor();
}

//Spawns the Enemy Actors from the Array of characters that are assigned in the BP Editor
AActor* AEnemySpawner::SpawnEnemyActor()
{
	//if (enemyQueue.IsValidIndex(0))
	//{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Valid Index In Spawner"));

		FActorSpawnParameters spawnParams;
		AActor* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(enemyCharacters[0].Get(), this->GetActorLocation(), this->GetActorRotation(), spawnParams);
		//enemyQueue.RemoveAt(0);
		return spawnedEnemy;

	//}
	//else
	//{
		/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("NO Valid Index In Spawner"));
		isSpawning = false;
		return NULL;*/
	//}

	/*FActorSpawnParameters spawnParams;
	AActor* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(enemyCharacters[0].Get(), this->GetActorLocation(), this->GetActorRotation(), spawnParams);
	return spawnedEnemy;*/

	//GetWorld()->SpawnActor<AEnemyCharacterBase>(this->GetActorLocation(), this->GetActorRotation(), spawnParams);
}



bool AEnemySpawner::IsSpawnAreaClear()
{
	return true;
}

