// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileBase.h"
#include "DA_WeaponStats.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_WeaponStats : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/**
	* The amout of damage you want your weapon do deal
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float damageDelt;

	/**
	* The max ammo your weapon can hold 
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	int ammoMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float projectileSpeed;

	/** 
	* Ensure this is the name of the socket that is attached to the mesh of your weapon and that it is where you want the projectile to spawn
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FName muzzleName;
};
