#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerCharacterStats.generated.h"

UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_PlayerCharacterStats : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/** The name of your player character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FString name;

	/** The health of your player character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float health;

	/** The amount of shooting resource your player character has max */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float resource;

	/** How long it takes for your shooting resource to fully recharge in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float resourceChargeTime;

	/** How fast you want the base movement speed of your player character to be */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float movementSpeed;

	/** How fast you want the player character to move when running */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float runSpeed;

	/** How high you want the player character to jump */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float jumpHeight;

	/** How Long it takes to respawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float respawnTime;
};
