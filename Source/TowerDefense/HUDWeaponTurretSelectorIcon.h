#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWeaponTurretSelectorIcon.generated.h"

class UImage;
class ATurretStatic;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDWeaponTurretSelectorIcon : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetDefaults(UTexture2D* iconTexture, TSubclassOf<ATurretStatic> iconClass);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ATurretStatic> GetIconClass();

protected:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* iconImage;

	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<ATurretStatic> turretClass;
};
