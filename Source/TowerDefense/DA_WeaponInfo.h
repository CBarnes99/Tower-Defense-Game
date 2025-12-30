#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_WeaponInfo.generated.h"

class AWeaponBase;

UCLASS(Blueprintable)
class TOWERDEFENSE_API UDA_WeaponInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/** The name of the Weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText weaponName;

	/** The Icon for the Weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* weaponIcon;

	/** The Class of the Weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeaponBase> weaponClass;

	/** The amout of damage you want your weapon do deal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float damageDelt;

	/** The max ammo your weapon can hold */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ammoMax;

	/** The speed of the projectile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float projectileSpeed;

	/** How many projectiles can you fire per second, 15 = 15 shots per second */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float fireRate;

	/** How long can the projectile be in the air without colliding before its deactivated */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float lifetime;

	/** Ensure this is the name of the socket that is attached to the mesh of your weapon and that it is where you want the projectile to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName muzzleName;
};
