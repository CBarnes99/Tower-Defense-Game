// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_EnemyCharacterStats.generated.h"

UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_EnemyCharacterStats : public UDataAsset
{
	GENERATED_BODY()
	
protected:

	/**
	* The name of the enemy character
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FString name;

	/**
	* The amount of health you want the enemy character to have
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float health;

	/**
	* The amount of damage you want the enemy to deal to the player character
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float damageDelt;

	/**
	* How fast you want the enemy character to be
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float movementSpeed;

};
