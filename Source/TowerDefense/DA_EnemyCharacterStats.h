#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_EnemyCharacterStats.generated.h"

class AEnemyAIController;

UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_EnemyCharacterStats : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/** The name of the enemy character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	FString name;

	/** The amount of health you want the enemy character to have */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float health;

	/** The amount of damage you want the enemy to deal to the player character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float damageDelt;

	/** How fast you want the enemy character to be */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float movementSpeed;

	/** The amount of currency the player will get when the enemy is defeated */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int currencyOnDeath;

	/** The AI Controller for the enemy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<AEnemyAIController> AIControllerClass;
};
