#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core_GameState.generated.h"

class ATurretStatic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerCurrencyUpdatedSigniture, float, playerCurrencyAmount);

UCLASS()
class TOWERDEFENSE_API ACore_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FPlayerCurrencyUpdatedSigniture PlayerCurrencyUpdatedEvent;

	UFUNCTION(BlueprintCallable)
	float GetPlayerCurrencyAmount();
	
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerCurrencyAmount(bool addCurrency, float amount);

	UFUNCTION(BlueprintCallable)
	int GetMaxListSizeInWeaponTurretHud();

	UFUNCTION(BlueprintCallable)
	void SetMaxListSizeInWeaponTurretHud(int newMax);

	UFUNCTION(BlueprintCallable)
	int GetCurrentListSizeInWeaponTurretHud();

	UFUNCTION(BlueprintCallable)
	void IncreaseCurrentListSizeInWeaponTurretHud(bool addSize);

	UFUNCTION(BlueprintCallable)
	bool IsCurrentListSizeLessThanMax();

	UFUNCTION(BlueprintCallable)
	void SetCurrentTurretClass(TSubclassOf<ATurretStatic> turretClass);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ATurretStatic> GetCurrentTurretClass();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float playerCurrencyAmount;

	UPROPERTY(EditDefaultsOnly)
	int maxListSizeInWeaponTurretHud;

	UPROPERTY(VisibleAnywhere)
	int currentListSizeInWeaponTurretHud;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ATurretStatic> currentTurretClass;

};
