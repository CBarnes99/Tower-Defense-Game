#include "HUDWeaponTurretSelector.h"
#include "HUDWeaponTurretSelectorIcon.h"
#include "Components/HorizontalBox.h"
#include "HUDWeaponTurretSelectorIcon.h"
#include "DA_TurretInfo.h"


void UHUDWeaponTurretSelector::GetInfoFromTurretMenu(bool bIsChecked, UDA_TurretInfo* turretInformation)
{
	UE_LOG(LogTemp, Warning, TEXT("WITHIN WEAPON TURRET SELECTOR!!"));

	if (bIsChecked)
	{
		arrayOfTurretInfo.Add(turretInformation);
	}
	else
	{
		arrayOfTurretInfo.Remove(turretInformation);
	}

	UpdateWeaponTurretSelector(turretInformation);

}

void UHUDWeaponTurretSelector::UpdateWeaponTurretSelector(UDA_TurretInfo* turretInformation)
{
	for (UWidget* icon : IconHBox->GetAllChildren())
	{
		if (!iconClass)
		{
			UE_LOG(LogTemp, Error, TEXT("SET ICON CLASS WITHIN HUD WEAPON TURRET SELECTOR BP!!!"));
			return;
		}

		UHUDWeaponTurretSelectorIcon* currentIcon = Cast<UHUDWeaponTurretSelectorIcon>(icon);

		if (!currentIcon)
		{
			UE_LOG(LogTemp, Error, TEXT("%s IS NOT A SELECTOR ICON!!"), *icon->GetName());
			return;
		}

		if (currentIcon->GetIconClass() == turretInformation->turretClass)
		{
			icon->RemoveFromParent();
			return;
		}
	}

	UHUDWeaponTurretSelectorIcon* turretIcon = CreateWidget<UHUDWeaponTurretSelectorIcon>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), iconClass);
	turretIcon->SetWidgetDefaults(turretInformation->turretIcon, turretInformation->turretClass);
	IconHBox->AddChildToHorizontalBox(turretIcon);

}

