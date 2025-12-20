#include "HUDWeaponTurretSelectorIcon.h"
#include "Components/Image.h"

void UHUDWeaponTurretSelectorIcon::SetWidgetDefaults(UTexture2D* iconTexture, TSubclassOf<UClass> iconClass)
{
	iconImage->SetBrushFromTexture(iconTexture);
	weaponOrTurretClass = iconClass;
}
