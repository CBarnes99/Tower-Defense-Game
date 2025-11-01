// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Mana.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_Mana : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Mana();

	UFUNCTION(BlueprintCallable)
	void SetMana(float manaAmount);

	UFUNCTION(BlueprintCallable)
	float GetMana();

	UFUNCTION(BlueprintCallable)
	void GainMana(float gainAmount);

	UFUNCTION(BlueprintCallable)
	bool SpendMana(float manaCostAmount);
	
	UFUNCTION(BlueprintCallable)
	bool HasEnoughMana(float manaCostAmount);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxMana;

		
};
