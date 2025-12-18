#pragma once

#include "Core_PlayerController.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core_HUD.generated.h"

UCLASS()
class TOWERDEFENSE_API ACore_HUD : public AHUD
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	ACore_PlayerController* corePlayerController;

	//UPROPERTY(VisibleDefaultsOnly)
	FInputModeGameOnly inputModeGameOnly;

	UFUNCTION(BlueprintCallable)
	void SetFocusToGame();

};
