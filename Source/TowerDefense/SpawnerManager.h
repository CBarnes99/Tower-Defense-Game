#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "E_EnemyDrop.h"
#include "SpawnerManager.generated.h"

class AEnemySpawner;
class AEnemyDrop;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveEndedSignature);

UCLASS()
class TOWERDEFENSE_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnerManager();

	/** A delegate that is called when all the enemies in the wave have been defeated */
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FWaveEndedSignature WaveEndedEvent;

	/** An array of spawners for the manager to call from */
	UPROPERTY(VisibleAnywhere)
	TArray<AEnemySpawner*> enemySpawners;

	/** Calls the spawners to start spawning from a specific wave
	* @param currentWave The current wave you want to spawn, as an int */
	UFUNCTION(BlueprintCallable)
	void StartSpawningEnemies(int currentWave);

	/** Calculates the last wave for the game mode */
	UFUNCTION(BlueprintCallable)
	int CalculateLastWave();

	/** Sets all the spawners in the world to the array */
	UFUNCTION(BlueprintCallable)
	void SetAllSpawners();

	/** A check to see if there is a wave currently active
	* @return True, the wave is active, False, there isnt a wave active and you can start a new wave */
	UFUNCTION(BlueprintCallable)
	bool IsWaveActive();

	/** The ability to turn on and off the enemies perception when the player has been defeated or respawned 
	* @param bIsPlayerDefeated true = Turns off the enemies perception, false = Enables the enemies perception */
	UFUNCTION(BlueprintCallable)
	void ShouldEnemiesPerceptPlayer(bool bIsPlayerDefeated);

protected:
	virtual void BeginPlay() override;

	/** The amount of enemies that are in round, as an int */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int amountOfEnemysInRound;

	/** A check to see if the wave is currently active, as a bool */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool waveActive;

	/** When an enemy is spawned from a spawner, the enemys on death delegate is bound to EnemyHasDied() */
	UFUNCTION(BlueprintCallable)
	void BindDelegateOnEnemy(AEnemyCharacterBase* enemy);

	/** When an enemy has been defeated, this is called */
	UFUNCTION(BlueprintCallable)
	void EnemyHasDied(AEnemyCharacterBase* enemy);

	UPROPERTY(EditDefaultsOnly, Category = "DropPooling")
	TSubclassOf<AEnemyDrop> enemyDropBlueprintClass;

	/** The pool of Enemy Drops */
	UPROPERTY(VisibleAnywhere, Category = "DropPooling")
	TArray<AEnemyDrop*> enemyDropPool;

	/** The amount of Enemy Drop within the pool */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropPooling")
	int amountOfDropToPool;

	/** Called on begin play to pool the Enemy Drops */
	UFUNCTION(BlueprintCallable, Category = "DropPooling")
	void PoolEnemyDrop();

	/** When Enemy deklegate OnSpawnEnemyDropEvent is called, this function is called to set up the drop to the correct type and location */
	UFUNCTION(BlueprintCallable, Category = "DropPooling")
	void SetEnemyDrop(EEnemyDrop dropType, FVector spawnLocation);
};
