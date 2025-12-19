#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretStatic.h"
#include "HUDTurretInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckboxStateChanged, bool, bIsChecked);

class UOverlay;
class UVerticalBox;
class UTextBlock;
class UImage;
class UCheckBox;
class UTextBlock;

class UDA_TurretInfo;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDTurretInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (ExposeOnSpawn = true))
	UDA_TurretInfo* turretInfo;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* WidgetOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TurretNameText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TurretImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* IsTurretSelectedCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* IsTurretSelectedText;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	TSubclassOf<ATurretStatic> turretToSpawn;*/

	UFUNCTION(BlueprintCallable)
	void SetWidgetDefaults(FText newTurretName, UTexture2D* newTurretTexture/*, TSubclassOf<ATurretStatic> turret*/);

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCheckboxStateChanged OnCheckboxStateChangedSignature;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void HandleCheckboxStateChange(bool bIsChecked);

	UFUNCTION(BlueprintCallable)
	void SetSelectedText();
};