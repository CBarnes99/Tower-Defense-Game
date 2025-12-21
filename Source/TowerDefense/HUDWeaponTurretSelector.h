#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWeaponTurretSelector.generated.h"

class UDA_TurretInfo;
class UHorizontalBox;
class UHUDWeaponTurretSelectorIcon;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDWeaponTurretSelector : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void GetInfoFromTurretMenu(bool bIsChecked, UDA_TurretInfo* turretInformation);

protected:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UHorizontalBox* IconHBox;

	UPROPERTY(VisibleAnywhere)
	TArray<UDA_TurretInfo*> arrayOfTurretInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDWeaponTurretSelectorIcon> iconClass;

	UFUNCTION(BlueprintCallable)
	void UpdateWeaponTurretSelector(UDA_TurretInfo* turretInformation);

};
