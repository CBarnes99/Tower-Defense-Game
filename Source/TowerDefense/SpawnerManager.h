// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

DECLARE_DELEGATE(FOnEnemyUnitsDeath);

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
	bool waveActive;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> enemySpawners;

	UFUNCTION(BlueprintCallable)
	void StartSpawningEnemies(int currentWave);

	UFUNCTION(BlueprintCallable)
	int CalculateLastWave();

	UFUNCTION(BlueprintCallable)
	void SetAllSpawners();

	UFUNCTION(BlueprintCallable)
	bool IsWaveActive();

	FOnEnemyUnitsDeath DelegateEnemyDied;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	int amountOfEnemysInRound;
};
