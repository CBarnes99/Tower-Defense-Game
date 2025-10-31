// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Health.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENSE_API UAC_Health : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Health();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	void SetHealth(float healthAmount);

	UFUNCTION(BlueprintCallable)
	void RecieveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void RecieveHealing(float healAmount);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float maxHealth;

};
