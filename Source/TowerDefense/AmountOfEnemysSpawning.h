// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.generated.h"

USTRUCT(BlueprintType)
struct FAmountOfEnemysSpawning : public FTableRowBase
{
	GENERATED_BODY()
		
protected:

	UPROPERTY(EditAnywhere, Category = "Enemy Information")
	TArray<TSubclassOf<AEnemyCharacterBase>> enemyType;

	/*UPROPERTY(EditAnywhere, Category = "Enemy Information")
	int amount;*/

	/*UPROPERTY(EditAnywhere, Category = "Enemy Information")
	UDA_CharacterStats* enemyStats;*/
};