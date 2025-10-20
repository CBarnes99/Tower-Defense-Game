// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/DataTable.h"
#include "EnemyCharacterBase.h"
#include "AmountOfEnemysSpawning.generated.h"

USTRUCT(BlueprintType)
struct FAmountOfEnemysSpawning //: public FTableRowBase
{
	GENERATED_BODY()
		
public:

	UPROPERTY(EditAnywhere, Category = "Enemy Information")
	TArray<TSubclassOf<AEnemyCharacterBase>> enemyTypeArray;
};