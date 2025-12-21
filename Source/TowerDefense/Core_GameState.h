#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core_GameState.generated.h"

UCLASS()
class TOWERDEFENSE_API ACore_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	float GetPlayerCurrencyAmount();
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerCurrencyAmount(bool addCurrency, float amount);

	UFUNCTION(BlueprintCallable)
	int GetMaxListSizeInWeaponTurretHud();

	UFUNCTION(BlueprintCallable)
	void SetMaxListSizeInWeaponTurretHud(int newMax);

	UFUNCTION(BlueprintCallable)
	int GetCurrentListSizeInWeaponTurretHud();

	UFUNCTION(BlueprintCallable)
	void SetCurrentListSizeInWeaponTurretHud(bool addSize);

	UFUNCTION(BlueprintCallable)
	bool IsCurrentListSizeLessThanMax();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float playerCurrencyAmount;

	UPROPERTY(EditDefaultsOnly)
	int maxListSizeInWeaponTurretHud;

	UPROPERTY(VisibleAnywhere)
	int currentListSizeInWeaponTurretHud;

};
