#include "HUDTurretInfo.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Core_GameState.h"

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
	if (gameState == nullptr)
	{
		AGameStateBase* gameStateBase = UGameplayStatics::GetGameState(GetWorld());
		gameState = Cast<ACore_GameState>(gameStateBase);

		if (!gameState)
		{
			UE_LOG(LogTemp, Error, TEXT("GameState is not casted correctly within HUDTurretInfo!!"));
			return;
		}
	}

	if (bIsChecked)
	{
		if (gameState->IsCurrentListSizeLessThanMax())
		{
			gameState->SetCurrentListSizeInWeaponTurretHud(true);
			OnCheckboxStateChangedSignature.Broadcast(bIsChecked, turretInfo);
		}
		else
		{
			IsTurretSelectedCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
			UE_LOG(LogTemp, Warning, TEXT("List Size Max Has Been Reached - %d"), gameState->GetCurrentListSizeInWeaponTurretHud());
			//SET UP MODAL FOR MAX LIST SIZE REACHED TO LET THE PLAYER KNOW THEY CANT ADD ANYMORE!
		}

	}
	else
	{
		gameState->SetCurrentListSizeInWeaponTurretHud(false);
		OnCheckboxStateChangedSignature.Broadcast(bIsChecked, turretInfo);
	}
		

	SetSelectedText();

}