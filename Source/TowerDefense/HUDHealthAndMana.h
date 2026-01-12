#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDHealthAndMana.generated.h"

class UProgressBar;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDHealthAndMana : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float currentHealth, float maxHealth);

	UFUNCTION(BlueprintCallable)
	void UpdateManaBar(float currentHealth, float maxHealth);

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* ManaBar;
};
