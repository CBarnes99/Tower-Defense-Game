// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnerManager.h"
#include "CombatGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	ACombatGameMode();

	virtual void BeginPlay() override;

//protected:
	/**
	* The spawner manager in the level
	*/
	UPROPERTY(VisibleAnywhere)
	ASpawnerManager* spawnerManager;

	/**
	* The current wave the level is on, starts at 0 on begin play
	*/
	UPROPERTY(VisibleAnywhere)
	int currentWave;

	/**
	* This is called when the player calls to start the next wave
	*/
	UFUNCTION(BlueprintCallable)
	void StartEnemyWave();

	/**
	* The final wave in the level, is set in StartEnemyWave()
	*/
	UPROPERTY(VisibleAnywhere)
	int lastWave;

	UFUNCTION(BlueprintCallable)
	void PrepareNewWave();
};
