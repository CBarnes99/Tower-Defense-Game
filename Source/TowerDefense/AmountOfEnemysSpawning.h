// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.generated.h"

USTRUCT(BlueprintType)
struct FAmountOfEnemysSpawning
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(EditAnywhere, Category = "Spawn Information")
	int amount;

	UPROPERTY(EditAnywhere, Category = "Spawn Information")
	TArray<TSubclassOf<AEnemyCharacterBase>> enemyType;
};