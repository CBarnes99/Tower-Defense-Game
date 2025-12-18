#include "Core_HUD.h"

void ACore_HUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	corePlayerController = Cast<ACore_PlayerController>(playerController);
	SetFocusToGame();
	
}

void ACore_HUD::SetFocusToGame()
{
	corePlayerController->SetInputMode(inputModeGameOnly);
}
