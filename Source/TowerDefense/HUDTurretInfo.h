#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDTurretInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckboxStateChangedSignature, bool, bIsChecked, UDA_TurretInfo*, turretInformation);

class UVerticalBox;
class UTextBlock;
class UImage;
class UCheckBox;
class UTextBlock;
class ACore_GameState;
class UDA_TurretInfo;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDTurretInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* TurretNameText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* TurretImage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCheckBox* IsTurretSelectedCheckBox;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* turretCost;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* IsTurretSelectedText;

	UPROPERTY(EditDefaultsOnly)
	UDA_TurretInfo* turretInfo;

	UFUNCTION(BlueprintCallable)
	void SetWidgetDefaults(UDA_TurretInfo* turretInfoDA);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCheckboxStateChangedSignature OnCheckboxStateChangedEvent;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void HandleCheckboxStateChange(bool bIsChecked);

	UFUNCTION(BlueprintCallable)
	void SetSelectedText();

	UPROPERTY(VisibleDefaultsOnly)
	ACore_GameState* gameState;
};