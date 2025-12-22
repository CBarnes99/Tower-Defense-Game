#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "HUDTurretSelectionMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMenuSelectionSignature, bool, bIsChecked, UDA_TurretInfo*, turretInformation);

class UCanvasPanel;
class UUniformGridPanel;
class UHUDTurretInfo;
class UDA_TurretInfo;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDTurretSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCanvasPanel* MenuCanvasPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UUniformGridPanel* TurretSelectionGrid;

	UFUNCTION(BlueprintCallable)
	void SetUpWidget(TArray<UDA_TurretInfo*> turretInfoArray);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMenuSelectionSignature OnMenuSelectionEvent;

protected:

	//virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void TurretHasBeenSelected(bool bIsChecked, UDA_TurretInfo* turretInformation);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDTurretInfo> turretInfoClass;

};
