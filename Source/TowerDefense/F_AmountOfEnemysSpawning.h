// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacterBase.h"
#include "F_AmountOfEnemysSpawning.generated.h"

USTRUCT(BlueprintType)
struct FAmountOfEnemysSpawning
{
	GENERATED_BODY()
		
public:

	/**
	* An array of enemy characters that'll spawn one by one from index 0 to the end
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy Information")
	TArray<TSubclassOf<AEnemyCharacterBase>> enemyTypeArray;
};