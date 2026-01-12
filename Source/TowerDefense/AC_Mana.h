#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Mana.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_Mana : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_Mana();

	/** Set the max mana for the parent actor
	* @param manaAmount The amount of mana you want the parent actor to have, as a float */
	UFUNCTION(BlueprintCallable)
	void SetMana(float manaAmount);

	/** Gets the current mana for the parent actor
	* @return The mana the parent actor has, as a float */
	UFUNCTION(BlueprintCallable)
	float GetMana();

	/** Gain a certain amount of mana for the parent actor
	* @param gainAmount The amount you want the parent actor to gain, as a float */
	UFUNCTION(BlueprintCallable)
	void GainMana(float gainAmount);

	/** The amount of mana you want the parent actor to use
	* @param manaCostAmount How much mana you want the action to cost
	* @return If you have enough mana for this action, as a bool */
	UFUNCTION(BlueprintCallable)
	bool SpendMana(float manaCostAmount);
	
protected:

	/** A check to see if you have enough mana, used by Spend Mana
	* @param manaCostAmount How much mana you want the action to cost
	* @return If you have enough mana for this action, as a bool */
	UFUNCTION(BlueprintCallable)
	bool HasEnoughMana(float manaCostAmount);

	/** The amount of mana the parent actor has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float mana;

	/** The max mana the parent actor has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxMana;
			
};
