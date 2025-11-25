#pragma once

#include "EnemyAIController.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "DA_EnemyCharacterStats.h"
#include "AC_Health.h"
#include "E_EnemyDrop.h"
#include "EnemyCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemyCharacterBase*, Enemy);

UCLASS()
class TOWERDEFENSE_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviourTree() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeathSignature OnEnemyDeathEvent;

protected:
	virtual void BeginPlay() override;

	/**
	* @brief A pointer to the the Behaviour Tree assigned in the editor
	*/
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* enemyBehaviorTree;

	/**
	* @brief The health component so the enemies can reduce in health when damaged
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	class UAC_Health* healthComponent;

	/**
	* @brief Assign a Data Asset in Blueprints for each of the enemys information
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	TObjectPtr<UDA_EnemyCharacterStats> enemyInfo;

	//The built in take damage function
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/**
	* @brief When the enemy loses all their health, on death is called
	*/
	UFUNCTION(BlueprintCallable)
	void OnDeath();

	///**
	//* @brief The drop that the enemy is holding
	//*/
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Drop")
	//TSubclassOf<AEnemyDrop> enemyDrop;

	/**
	* @brief On death, check to see if they will drop the item
	*/
	UFUNCTION(BlueprintCallable, Category = "Drop")
	void SpawnDrop();

	/**
	* @brief A TMap of all the enums and the value you give it within the editor to determin the drop weight
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Drop")
	TMap<EEnemyDrop, float> dropWeight;
};
