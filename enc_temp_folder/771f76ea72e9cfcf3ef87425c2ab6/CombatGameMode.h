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

	UPROPERTY(VisibleAnywhere)
	ASpawnerManager* spawnerManager;

	UFUNCTION(BlueprintCallable)
	void startEnemyWave();
};
