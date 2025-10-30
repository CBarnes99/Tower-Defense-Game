// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacterBase.h"
#include "EnemySpawner.h"
#include "SpawnerManager.generated.h"

UCLASS()
class TOWERDEFENSE_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemySpawner*> enemySpawners;

	UFUNCTION(BlueprintCallable)
	void StartSpawningEnemies(int currentWave);

	UFUNCTION(BlueprintCallable)
	int CalculateLastWave();

	UFUNCTION(BlueprintCallable)
	void SetAllSpawners();

	UFUNCTION(BlueprintCallable)
	bool IsWaveActive();

	UFUNCTION(BlueprintCallable)
	void BindDelegateOnEnemy(AEnemyCharacterBase* enemy);

	UFUNCTION(BlueprintCallable)
	void EnemyHasDied(AEnemyCharacterBase* enemy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	int amountOfEnemysInRound;

	UPROPERTY(VisibleAnywhere)
	bool waveActive;



};
