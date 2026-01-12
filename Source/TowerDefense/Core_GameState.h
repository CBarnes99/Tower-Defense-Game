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

	/** A Delegete that is called when the player currency amount has been updated */
	UPROPERTY(BlueprintAssignable)
	FPlayerCurrencyUpdatedSigniture PlayerCurrencyUpdatedEvent;

	/** Called when you need to recieve the amount of currecy the player currently has to spend 
	* @return The amount of currency the player has, as a float */
	UFUNCTION(BlueprintCallable)
	float GetPlayerCurrencyAmount();
	
	/** @brief Called when you need to update the amount of currency the player has 
	* @param addCurrency A Check for if you want to add or subract currency, true = add, false = subtract 
	* @param amount The amount of currency that is changeing */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerCurrencyAmount(bool addCurrency, float amount);

	/** The amount of turrets the player can have access to at once on the selection hotbar
	* @return The amount of turrets, as an int */
	UFUNCTION(BlueprintCallable)
	int GetMaxListSizeInWeaponTurretHud();

	/** Set the new max size of the amount of turrets the player can have access to at once on the selection hotbar 
	* @param newMax The new max amount of turrets the player can hold on their hotbar, as an int */
	UFUNCTION(BlueprintCallable)
	void SetMaxListSizeInWeaponTurretHud(int newMax);

	/** Gets the current max amount of turrets the player has acces to
	* @return The max size, as an int */
	UFUNCTION(BlueprintCallable)
	int GetCurrentListSizeInWeaponTurretHud();

	/** Update the curret list size the player currently has by 1
	* @param addSize true = increase size by 1, false = decrease size by 1 */
	UFUNCTION(BlueprintCallable)
	void UpdateCurrentListSizeInWeaponTurretHud(bool addSize);

	/** A check to see if the current list size is less than the max list size
	* @return Is the current list size less than the max, as a bool */
	UFUNCTION(BlueprintCallable)
	bool IsCurrentListSizeLessThanMax();

protected:

	virtual void BeginPlay() override;

	/** The amount of currecy the player currently has, as a float */
	UPROPERTY(EditDefaultsOnly)
	float playerCurrencyAmount;

	/** The max amount of turrets the player can have on their hotbar, as an int */
	UPROPERTY(EditDefaultsOnly)
	int maxListSizeInWeaponTurretHud;

	/** The current amount of turrets the player has on their hotbar, as an int */
	UPROPERTY(VisibleAnywhere)
	int currentListSizeInWeaponTurretHud;
};
