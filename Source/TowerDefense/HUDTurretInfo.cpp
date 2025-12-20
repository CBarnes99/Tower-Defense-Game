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

void UHUDTurretInfo::SetWidgetDefaults(FText newTurretName, UTexture2D* newTurretTexture, int newTurretCost, TSubclassOf<ATurretStatic> newTurretClass)
{
	TurretNameText->SetText(newTurretName);

	TurretImage->SetBrushFromTexture(newTurretTexture);

	FText textToFormat = FText::FromString(TEXT("Cost: {cost}"));
	FFormatNamedArguments args;
	args.Add(TEXT("cost"), FText::AsNumber(newTurretCost));
	FText formattedText = FText::Format(textToFormat, args);
	turretCost->SetText(formattedText);

	turretClass = newTurretClass;
}


void UHUDTurretInfo::SetSelectedText()
{
	if (IsTurretSelectedCheckBox->IsChecked())
	{
		IsTurretSelectedText->SetText(FText::FromString(TEXT("Selected")));
	}
	else
	{
		IsTurretSelectedText->SetText(FText::FromString(TEXT("Not Selected")));
	}
}

void UHUDTurretInfo::HandleCheckboxStateChange(bool bIsChecked)
{
	SetSelectedText();

	OnCheckboxStateChangedSignature.Broadcast(bIsChecked, turretInfo);

}