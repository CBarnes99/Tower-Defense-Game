#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWeaponTurretSelectorIcon.generated.h"

class UImage;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDWeaponTurretSelectorIcon : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* iconImage;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UClass> weaponOrTurretClass;

	UFUNCTION(BlueprintCallable)
	void SetWidgetDefaults(UTexture2D* iconTexture, TSubclassOf<UClass> iconClass);

};
