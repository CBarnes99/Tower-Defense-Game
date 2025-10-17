// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TArray<TSubclassOf<AEnemyCharacterBase>> EnemyCharacters;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TMap<int, FAmountOfEnemysSpawning> waveAndEnemys;

	int spawnAmount;
	int currentWave;
	
	AActor* spawnEnemy();
};
