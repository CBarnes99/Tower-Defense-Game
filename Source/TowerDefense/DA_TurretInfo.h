#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_TurretInfo.generated.h"

class ATurretStatic;

UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_TurretInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/** The name of the Turret */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText turretName;

	/** The Icon for the Turret */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* turretIcon;

	/** The Class of the Turret */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATurretStatic> turretClass;

	/** The amount of damage this Turret does to the enemy actors */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float damageAmount;

	/** The damage type that the Turret deals */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> damageType;

	/** The amount of time the Turret is active for when it detects and enemy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float activeTime;

	/** The time it takes to recharge the Turret after it has been used */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float rechargeTime;

	/** The cost to place down the Turret */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int cost;
};
