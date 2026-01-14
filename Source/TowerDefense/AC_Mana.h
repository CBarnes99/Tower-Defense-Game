#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Mana.generated.h"

DECLARE_DELEGATE_TwoParams(FManaUpdatedSigniture, float /* Current Mana */, float /* Max Mana */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_Mana : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_Mana();

	/** When mana is updated, this is called */
	FManaUpdatedSigniture ManaUpdatedEvent;

	/** Set the max mana for the parent actor
	* @param manaAmount The amount of mana you want the parent actor to have, as a float */
	UFUNCTION(BlueprintCallable)
	void SetMana(float manaAmount);

	/** Set how long it takes to fully recharge the mana
	* @param rechargeRate The time it takes to fully recharge the mana, as a float */
	UFUNCTION(BlueprintCallable)
	void SetRechargeRate(float rechargeRate);

	/** Gets the current mana for the parent actor
	* @return The currentMana the parent actor has, as a float */
	UFUNCTION(BlueprintCallable)
	float GetMana();

	/** Gets the max mana for the parent actor
	* @return The max mana the parent actor has, as a float */
	UFUNCTION(BlueprintCallable)
	float GetMaxMana();

	/** Gain a certain amount of mana for the parent actor
	* @param gainAmount The amount you want the parent actor to gain, as a float */
	UFUNCTION(BlueprintCallable)
	void GainMana(float gainAmount);

	/** The amount of mana you want the parent actor to use
	* @param manaCostAmount How much mana you want the action to cost
	* @return If you have enough mana for this action, as a bool */
	UFUNCTION(BlueprintCallable)
	bool SpendMana(float manaCostAmount);
	
	/** A check to see if you have enough mana, used by Spend Mana
	* @param manaCostAmount How much mana you want the action to cost
	* @return If you have enough mana for this action, as a bool */
	UFUNCTION(BlueprintCallable)
	bool HasEnoughMana(float manaCostAmount);

protected:
	/** The amount of mana the parent actor has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float currentMana;

	/** The max mana the parent actor has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxMana;

	/** The recharge rate for the mana. The max time it takes to fully recharge the mana */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float manaRechargeRate;
	
	FTimerHandle RechargeTimerHandle;

	/** Call to start or stop the mana recharge
	* @param bStartTimer ture = start recharging mana, false = stop recharging mana */
	UFUNCTION(BlueprintCallable)
	void RechargeManaTimer(bool bStartTimer);

	/** How many times a second is the is the timer called, the higher the value, the smoother the mana increase */
	UPROPERTY(EditAnywhere)
	float rechargeFrequency;
};
