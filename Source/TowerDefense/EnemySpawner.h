// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.h"
#include "Engine/DataTable.h"
#include "EnemySpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnSignature, AEnemyCharacterBase*, Enemy);


UCLASS()
class TOWERDEFENSE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//A timer that is called evey x seconds to spawn an enemy
	UPROPERTY(EditAnywhere)
	FTimerHandle spawnEnemyTimerHandle;
	
	UPROPERTY(VisibleAnywhere)
	bool isSpawning;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TMap<int, FAmountOfEnemysSpawning> waveAndEnemyQueue;

	//This is set by the spawner manager which recives input from the game mode which holds the current wave
	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	int currentWaveBeingSpawned;

	UFUNCTION(BlueprintCallable)
	void StartSpawning();

	UFUNCTION(BlueprintCallable)
	void StopSpawning();

	UFUNCTION(BlueprintCallable)
	int CalculateAmountOfEnemiesInWave();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemySpawnSignature EnemySpawnedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	float spawnInterval;

	UFUNCTION(BlueprintCallable)
	void Spawning();

	UFUNCTION(BlueprintCallable)
	AActor* SpawnEnemyActor();

	UFUNCTION(BlueprintCallable)
	void errorSpawningLog(FString log);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	class ASpawnerManager* spawnerManager;
};
