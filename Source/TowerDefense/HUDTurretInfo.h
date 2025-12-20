#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DA_TurretInfo.h"
#include "HUDTurretInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckboxStateChanged, bool, bIsChecked, UDA_TurretInfo*, turretInformation);

class UVerticalBox;
class UTextBlock;
class UImage;
class UCheckBox;
class UTextBlock;

//class UDA_TurretInfo;
class ATurretStatic;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDTurretInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TurretNameText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TurretImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* IsTurretSelectedCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* turretCost;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* IsTurretSelectedText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATurretStatic> turretClass;

	UPROPERTY(EditAnywhere)
	UDA_TurretInfo* turretInfo;

	UFUNCTION(BlueprintCallable)
	void SetWidgetDefaults(FText newTurretName, UTexture2D* newTurretTexture, int newTurretCost, TSubclassOf<ATurretStatic> newTurretClass);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCheckboxStateChanged OnCheckboxStateChangedSignature;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void HandleCheckboxStateChange(bool bIsChecked);

	UFUNCTION(BlueprintCallable)
	void SetSelectedText();
};