#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDPlayerHud.generated.h"

class UHUDHealthAndMana;

UCLASS(Abstract)
class TOWERDEFENSE_API UHUDPlayerHud : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHUDHealthAndMana* HealthAndMana;

};
