// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerCharacterStats.generated.h"

UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_PlayerCharacterStats : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Mana;

	//probably be in the weapon?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float DamageDelt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float JumpHeight;
	
};
