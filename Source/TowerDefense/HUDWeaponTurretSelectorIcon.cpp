#include "HUDWeaponTurretSelectorIcon.h"
#include "Components/Image.h"

void UHUDWeaponTurretSelectorIcon::SetWidgetDefaults(UTexture2D* iconTexture, TSubclassOf<ATurretStatic> iconClass)
{
	iconImage->SetBrushFromTexture(iconTexture);
	turretClass = iconClass;
}

TSubclassOf<ATurretStatic> UHUDWeaponTurretSelectorIcon::GetIconClass()
{
	return turretClass;
}