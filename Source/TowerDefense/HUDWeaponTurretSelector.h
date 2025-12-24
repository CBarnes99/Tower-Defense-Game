#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWeaponTurretSelector.generated.h"

class UDA_TurretInfo;
class UHorizontalBox;
class UHUDWeaponTurretSelectorIcon;
class ACore_GameState;
class ATurretStatic;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDWeaponTurretSelector : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void GetInfoFromTurretMenu(bool bIsChecked, UDA_TurretInfo* turretInformation);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ATurretStatic> GetTurretClassFromArray(int index);

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UHorizontalBox* IconHBox;

	UPROPERTY(VisibleAnywhere)
	TArray<UDA_TurretInfo*> arrayOfTurretInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDWeaponTurretSelectorIcon> iconClass;

	UFUNCTION(BlueprintCallable)
	void UpdateWeaponTurretSelector(UDA_TurretInfo* turretInformation);

	UFUNCTION(BlueprintCallable)
	void SetCurrentTurretClass();

	UPROPERTY(EditDefaultsOnly)
	ACore_GameState* coreGameState;

};
