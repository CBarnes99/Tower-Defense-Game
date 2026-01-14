#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Health.generated.h"

DECLARE_DELEGATE_TwoParams(FHealthUpdatedSigniture, float /* Current Health */, float /* Max Health */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_Health : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_Health();

	FHealthUpdatedSigniture HealthUpdatedEvent;

	/** Returns the current health of the parent actor */
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	/** Returns the Max health of the parent actor */
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	/** Sets the health for the parent actor
	* @param healthAmount The amount of health you want your actor to have, as a float */
	UFUNCTION(BlueprintCallable)
	void SetHealth(float healthAmount);

	/** Sets the health value back to the max */
	UFUNCTION(BlueprintCallable)
	void ResetHealth();

	/** Uses the built in Apply Damage function parameters to deal damage to the actor */
	UFUNCTION(BlueprintCallable)
	void RecieveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	/** Updates the health of the player when it is healed
	* @param healAmount How much you want to heal your actor, as a float */
	UFUNCTION(BlueprintCallable)
	void RecieveHealing(float healAmount);

protected:
	virtual void BeginPlay() override;

	/** The amount of health the parent actor has, as a float */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float currentHealth;

	/** The max health the parent actor has, as a float */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxHealth;

};
