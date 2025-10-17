// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyAIController.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "DA_CharacterStats.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

	UBehaviorTree* GetBehaviourTree() const;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* EnemyBehaviorTree;

	//Assign a Data Asset in Blueprints for each of the enemys information
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TObjectPtr<UDA_CharacterStats> EnemyInfo;

};
