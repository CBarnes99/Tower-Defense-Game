// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	currentWave = 1;
	//spawnAmount = waveAndEnemys.Contains(currentWave);
	spawnAmount = 5;

	for (int i = 0; i < spawnAmount; i++)
	{
		spawnEnemy();
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Spawns the Enemy Actors from the Array of characters that are assigned in the BP Editor
AActor* AEnemySpawner::spawnEnemy()
{
	FActorSpawnParameters spawnParams;
	AActor* spawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyCharacters[0].Get(), this->GetActorLocation(), this->GetActorRotation(), spawnParams);
	return spawnedEnemy;

	//GetWorld()->SpawnActor<AEnemyCharacterBase>(this->GetActorLocation(), this->GetActorRotation(), spawnParams);
}

