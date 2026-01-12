#include "HUDTurretSelectionMenu.h"
#include "HUDTurretInfo.h"
#include "DA_TurretInfo.h"

void UHUDTurretSelectionMenu::SetUpWidget(TArray<UDA_TurretInfo*> turretInfoArray)
{
	int xTurretGrid = 0;
	int yTurretGrid = 0;

	if (turretInfoClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TURRET INFO CLASS IS NOT SET WITHIN HUDTurretSelectionMenu!!!"));
		return;
	}
	for (UDA_TurretInfo* dataAsset : turretInfoArray)
	{
		UHUDTurretInfo* turretWidget = CreateWidget<UHUDTurretInfo>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), turretInfoClass);
		TurretSelectionGrid->AddChildToUniformGrid(turretWidget, yTurretGrid, xTurretGrid);
		xTurretGrid++;
		if (xTurretGrid > 3)
		{
			xTurretGrid = 0;
			yTurretGrid++;
		}
		turretWidget->OnCheckboxStateChangedEvent.AddDynamic(this, &UHUDTurretSelectionMenu::TurretHasBeenSelected);
		turretWidget->SetWidgetDefaults(dataAsset);
	}
}


void UHUDTurretSelectionMenu::TurretHasBeenSelected(bool bIsChecked, UDA_TurretInfo* turretInformation)
{
	UE_LOG(LogTemp, Warning, TEXT("Checkbox is doing things"));

	OnMenuSelectionEvent.Broadcast(bIsChecked, turretInformation);
}