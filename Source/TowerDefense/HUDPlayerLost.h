#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDPlayerLost.generated.h"

class UButton;
class UWidgetAnimation;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDPlayerLost : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* MainMenuButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* RestartLevelButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* LostAnimation;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayLostAnimation();

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable)
	void RestartLevel();

};
