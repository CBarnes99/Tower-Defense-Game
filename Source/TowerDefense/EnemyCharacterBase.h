#pragma once

#include "EnemyAIController.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Bound in the Spawner manager to keep track of how many enemies are left in the round
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeathSignature OnEnemyDeathEvent;

	/**
	* @brief Sets the Path Node Locations
	* @param nodeLocations The Locations of the path nodes you want the enemy to follow, as a TArray<FVector>
	*/
	UFUNCTION(BlueprintCallable)
	void SetPathNodeLocations(TArray<FVector> nodeLocations);

	/**
	* @brief Gets the Path Node Locations
	* @return Gets a Path Node Location, as a FVector
	* @param index The index
	*/
	UFUNCTION(BlueprintCallable)
	FVector GetNextPathNodeLocation();

protected:
	virtual void BeginPlay() override;

	/**
	* @brief A pointer to the the Behaviour Tree assigned in the editor
	*/
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* enemyBehaviorTree;

	/**
	* @brief An array of vectors that is the location for the paths nodes for the AI to follow
	*/
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TArray<FVector> pathNodeLocations;

	/**
	* @brief An index of what node the ai is moving towards
	*/
	UPROPERTY(VisibleAnywhere, Category = "AI")
	int pathNodeIndex;

	/**
	* @brief Increaces the Path Node Index
	*/
	UFUNCTION(BlueprintCallable)
	void IncreasePathNodeIndex();

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
