#include "HUDWeaponTurretSelector.h"
#include "HUDWeaponTurretSelectorIcon.h"
#include "Components/HorizontalBox.h"
#include "HUDWeaponTurretSelectorIcon.h"
#include "DA_TurretInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Core_GameState.h"
#include "Core_PlayerController.h"

void UHUDWeaponTurretSelector::NativeConstruct()
{
	APlayerController* playerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	ACore_PlayerController* coreController = Cast<ACore_PlayerController>(playerController);
	coreController->GetTurretDAEvent.BindUObject(this, &UHUDWeaponTurretSelector::GetTurretClassFromArray);
}

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

void UHUDWeaponTurretSelector::SetCurrentTurretClass()
{
	if (!coreGameState)
	{
		AGameStateBase* gameState = UGameplayStatics::GetGameState(GetWorld());
		coreGameState = Cast<ACore_GameState>(gameState);
	}
}

UDA_TurretInfo* UHUDWeaponTurretSelector::GetTurretClassFromArray(int index)
{
	return arrayOfTurretInfo[index];
}