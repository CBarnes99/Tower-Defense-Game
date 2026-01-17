#include "HUDPlayerLost.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UHUDPlayerLost::NativeConstruct()
{
	MainMenuButton->OnPressed.AddDynamic(this, &UHUDPlayerLost::ReturnToMainMenu);
	RestartLevelButton->OnPressed.AddDynamic(this, &UHUDPlayerLost::RestartLevel);
}

void UHUDPlayerLost::ReturnToMainMenu()
{
	UE_LOG(LogTemp, Display, TEXT("ReturnToMainMenu: Return to Main Menu"));
}

void UHUDPlayerLost::RestartLevel()
{
	UE_LOG(LogTemp, Display, TEXT("RestartLevel: Restart Level"));

	FString currentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	UGameplayStatics::OpenLevel(GetWorld(), FName(currentLevelName));
}

void UHUDPlayerLost::PlayLostAnimation()
{
	if (!LostAnimation)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayLostAnimation: LOST ANIMATION NOT SET WITHIN - %s"), *this->GetName());
		return;
	}

	PlayAnimation(LostAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
}