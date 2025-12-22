#include "HUDTurretSelectionMenu.h"
#include "HUDTurretInfo.h"
#include "DA_TurretInfo.h"

void UHUDTurretSelectionMenu::SetUpWidget(TArray<UDA_TurretInfo*> turretInfoArray)
{
	//TArray<UWidget*> gridChildren = TurretSelectionGrid->GetAllChildren();
	//int arrayIndex = 0;
	int xTurretGrid = 0;
	int yTurretGrid = 0;

	if (turretInfoClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TURRET INFO CLASS IS NOT SET WITHIN HUDTurretSelectionMenu!!!"));
		return;
	}

	//for (UWidget* widget : gridChildren)
	//for (int i = 0; i < turretInfoArray.Num(); i++)
	for (UDA_TurretInfo* dataAsset : turretInfoArray)
	{
		/*if (!turretInfoArray.IsValidIndex(arrayIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("No Valid Index (%d) for Turret Widgets Using DA_TurretInfo inside - %s"), arrayIndex, *this->GetName());
			return;
		}*/

		UHUDTurretInfo* turretWidget = CreateWidget<UHUDTurretInfo>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), turretInfoClass);
		TurretSelectionGrid->AddChildToUniformGrid(turretWidget, yTurretGrid, xTurretGrid);
		xTurretGrid++;
		if (xTurretGrid > 3)
		{
			xTurretGrid = 0;
			yTurretGrid++;
		}

		//if (UHUDTurretInfo* turretWidget = Cast<UHUDTurretInfo>(widget))
		//{
		turretWidget->OnCheckboxStateChangedEvent.AddDynamic(this, &UHUDTurretSelectionMenu::TurretHasBeenSelected);

		/*FText turretName = turretInfoArray[arrayIndex]->turretName;
		UTexture2D* turretIcon = turretInfoArray[arrayIndex]->turretIcon;
		TSubclassOf<ATurretStatic> turretClass = turretInfoArray[arrayIndex]->turretClass;
		int cost = turretInfoArray[arrayIndex]->cost;*/

		turretWidget->SetWidgetDefaults(dataAsset);
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Widget At Index (%d) is not a Valid UHUDTurretInfo Widget!!!"), arrayIndex);
		//}

		//arrayIndex++;
	}
}


void UHUDTurretSelectionMenu::TurretHasBeenSelected(bool bIsChecked, UDA_TurretInfo* turretInformation)
{
	UE_LOG(LogTemp, Warning, TEXT("Checkbox is doing things"));

	OnMenuSelectionEvent.Broadcast(bIsChecked, turretInformation);

}