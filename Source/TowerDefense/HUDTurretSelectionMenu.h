#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "HUDTurretSelectionMenu.generated.h"

class UCanvasPanel;
class UUniformGridPanel;
class UHUDTurretInfo;
class UDA_TurretInfo;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDTurretSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MenuCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUniformGridPanel* TurretSelectionGrid;

	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UHUDTurretInfo* Turret1;

	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UHUDTurretInfo* Turret2;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void TurretHasBeenSelected(bool bIsChecked, UDA_TurretInfo* turretInformation);

	UPROPERTY(EditDefaultsOnly)
	TArray<UDA_TurretInfo*> turretInfoForWidgetsArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDTurretInfo> turretInfoClass;

};
