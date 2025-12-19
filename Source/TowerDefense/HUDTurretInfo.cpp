
#include "HUDTurretInfo.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHUDTurretInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetSelectedText();

	IsTurretSelectedCheckBox->OnCheckStateChanged.AddDynamic(this, &UHUDTurretInfo::HandleCheckboxStateChange);
}

void UHUDTurretInfo::SetWidgetDefaults(FText newTurretName, UTexture2D* newTurretTexture/*, TSubclassOf<ATurretStatic> turret*/)
{
	TurretNameText->SetText(newTurretName);

	TurretImage->SetBrushFromTexture(newTurretTexture);

	//turretToSpawn = turret;
}

void UHUDTurretInfo::SetSelectedText()
{
	if (IsTurretSelectedCheckBox->IsChecked())
	{
		IsTurretSelectedText->SetText(FText::FromString(TEXT("Turret Is Selected")));
	}
	else
	{
		IsTurretSelectedText->SetText(FText::FromString(TEXT("Not Selected")));
	}
}

void UHUDTurretInfo::HandleCheckboxStateChange(bool bIsChecked)
{
	SetSelectedText();

	OnCheckboxStateChangedSignature.Broadcast(bIsChecked);
}