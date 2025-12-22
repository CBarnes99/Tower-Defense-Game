#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacterBase.h"
#include "EnemySpawner.h"
#include "SpawnerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveEndedSignature);

UCLASS()
class TOWERDEFENSE_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnerManager();

	/**
	* @brief An array of spawners for the manager to call from
	*/
	UPROPERTY(VisibleAnywhere)
	TArray<AEnemySpawner*> enemySpawners;

	/**
	* @brief Calls the spawners to start spawning from a specific wave
	* @param currentWave The current wave you want to spawn, as an int
	*/
	UFUNCTION(BlueprintCallable)
	void StartSpawningEnemies(int currentWave);

	/**
	* @brief Calculates the last wave for the game mode
	*/
	UFUNCTION(BlueprintCallable)
	int CalculateLastWave();

	/**
	* @brief Sets all the spawners in the world to the array
	*/
	UFUNCTION(BlueprintCallable)
	void SetAllSpawners();

	/**
	* @brief A check to see if there is a wave currently active
	* @return True, the wave is active, False, there isnt a wave active and you can start a new wave
	*/
	UFUNCTION(BlueprintCallable)
	bool IsWaveActive();

	UFUNCTION(BlueprintCallable)
	void BindDelegateOnEnemy(AEnemyCharacterBase* enemy);

	UFUNCTION(BlueprintCallable)
	void EnemyHasDied(AEnemyCharacterBase* enemy);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FWaveEndedSignature WaveEndedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	int amountOfEnemysInRound;

	UPROPERTY(VisibleAnywhere)
	bool waveActive;

};
