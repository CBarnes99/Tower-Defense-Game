// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.h"
//#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
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

	FTimerHandle SpawnCheckTimerHandle;
	
	bool isSpawning;

	UFUNCTION(BlueprintCallable)
	void StartSpawning();

	UFUNCTION(BlueprintCallable)
	void StopSpawning();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TArray<TSubclassOf<AEnemyCharacterBase>> enemyCharacters;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TMap<int, FAmountOfEnemysSpawning> waveAndEnemyQueue;


	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TArray<AEnemyCharacterBase*> enemyQueue;

	int spawnAmount;
	int currentWave;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	float spawnInterval;

	void Spawning();
	AActor* SpawnEnemyActor();
};
