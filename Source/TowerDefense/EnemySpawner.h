#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F_AmountOfEnemysSpawning.h"
#include "EnemySpawner.generated.h"

class AEnemyCharacterBase;
class USphereComponent;
class AEnemyPathSpline;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnedSigniture, AEnemyCharacterBase*, enemy);

UCLASS()
class TOWERDEFENSE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	/** A timer that is called evey x seconds to spawn an enemy */
	UPROPERTY(EditAnywhere)
	FTimerHandle spawnEnemyTimerHandle;
	
	/** A bool that checks if the spawner is currently spawning */
	UPROPERTY(VisibleAnywhere)
	bool isSpawning;

	/** A Map for the waves and enemy queue. Key int is the wave number, Value is a struct with an array of enemies that can be asigned within the editor */
	UPROPERTY(EditAnywhere)
	TMap<int, FAmountOfEnemysSpawning> waveAndEnemyQueue;

	/** This is set by the spawner manager which recives input from the game mode which holds the current wave */
	UPROPERTY(VisibleAnywhere)
	int currentWaveBeingSpawned;

	/** When Enter is pressed in combat player controller, this function starts */
	UFUNCTION(BlueprintCallable)
	void StartSpawning();

	/** Calculate the amount of enemies in the wave, is called from the game mode so when an enemy is eleminated, deduct one from the total */
	UFUNCTION(BlueprintCallable)
	int CalculateAmountOfEnemiesInWave();

	/** A Delegate for the spawner manager to keep track of all the enemies */
	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawnedSigniture OnEnemySpawnedEvent;

protected:
	virtual void BeginPlay() override;

	/** A collision compoenent to be able to check if theres space to spawn the enemy */
	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* spawnCollision;

	/** An array of splines that gets randomly selected for the enemy that is spawned to follow */
	UPROPERTY(EditAnywhere)
	TArray<AEnemyPathSpline*> enemyPathSplines;

	/** A Check to see if there are splines/paths for this spawner */
	UFUNCTION(BlueprintCallable)
	bool IsThereEnemyPathSplines();

	/** Sets a random path from enemyPathSplines for the enemy to follow */
	UFUNCTION(BlueprintCallable)
	AEnemyPathSpline* GetRandomEnemyPath();
	
	/** How frequent the spawner spawns an enemy */
	UPROPERTY(EditAnywhere)
	float spawnInterval;

	/** A function for the timer to call to start spawning enemies */
	UFUNCTION(BlueprintCallable)
	void SpawningFunctionForTimer();

	/** When all of the enemies have spawned, this is called to stop atempting to spawn a null actor */
	UFUNCTION(BlueprintCallable)
	void StopSpawning();

	/** Spawn the enemy actor */
	UFUNCTION(BlueprintCallable)
	AEnemyCharacterBase* SpawnEnemyActor();

	/** A Check to see if theres room to spawn the enemy, as a bool	*/
	UFUNCTION(BlueprintCallable)
	bool IsEnemyCollisionOverlap();

	/** The amount of enemies in wave to check if all enemies have been spawned, for debugging */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int amountOfEnemiesInWave;

	/** The amount of enemies that have spawned, for debugging */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int amountOfEnemiesSpawned;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AEnemyCharacterBase*> enemyPool;

	UFUNCTION(BlueprintCallable)
	void PoolEnemies();

};
