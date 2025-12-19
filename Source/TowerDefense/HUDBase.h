#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ToggleTurretSelectionMenu();

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* BaseCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHUDTurretSelectionMenu* TurretSelectionMenu;

};
