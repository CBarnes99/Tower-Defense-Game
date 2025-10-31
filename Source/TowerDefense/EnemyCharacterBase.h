// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyAIController.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "DA_EnemyCharacterStats.h"
#include "AC_Health.h"
#include "EnemyCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemyCharacterBase*, Enemy);

UCLASS()
class TOWERDEFENSE_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviourTree() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeathSignature OnEnemyDeathEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* enemyBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	class UAC_Health* healthComponent;


	//Assign a Data Asset in Blueprints for each of the enemys information
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	TObjectPtr<UDA_EnemyCharacterStats> enemyInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FString name;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float health;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float damageDelt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float movementSpeed;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void OnDeath();
};
