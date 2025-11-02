// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.h"
#include "Engine/DataTable.h"
#include "EnemySpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnedSigniture, AEnemyCharacterBase*, enemy);

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

	UPROPERTY(EditAnywhere)
	TMap<int, FAmountOfEnemysSpawning> waveAndEnemyQueue;

	//This is set by the spawner manager which recives input from the game mode which holds the current wave
	UPROPERTY(VisibleAnywhere)
	int currentWaveBeingSpawned;

	UFUNCTION(BlueprintCallable)
	void StartSpawning();

	UFUNCTION(BlueprintCallable)
	void StopSpawning();

	UFUNCTION(BlueprintCallable)
	int CalculateAmountOfEnemiesInWave();

	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawnedSigniture OnEnemySpawnedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* spawnCollision;
	
	UPROPERTY(EditAnywhere)
	float spawnInterval;

	UFUNCTION(BlueprintCallable)
	void SpawningFunctionForTimer();

	UFUNCTION(BlueprintCallable)
	AEnemyCharacterBase* SpawnEnemyActor();

	UFUNCTION(BlueprintCallable)
	bool IsEnemyCollisionOverlap();

};
