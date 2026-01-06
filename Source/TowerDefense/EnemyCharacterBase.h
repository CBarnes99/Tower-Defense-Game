#pragma once

//#include "EnemyAIController.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
//#include "DA_EnemyCharacterStats.h"
//#include "AC_Health.h"
#include "E_EnemyDrop.h"
#include "EnemyCharacterBase.generated.h"

class UBehaviourTree;
//class AEnemyAIController;
class UDA_EnemyCharacterStats;
class UAC_Health;
//class EE_EnemyDrop;
class AAIController;
class UAIPerceptionStimuliSourceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemyCharacterBase*, Enemy);
DECLARE_DELEGATE(FOnDisableSignature);

UCLASS()
class TOWERDEFENSE_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Bound in the Spawner manager to keep track of how many enemies are left in the round */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeathSignature OnEnemyDeathEvent;

	FOnDisableSignature OnDisableEvent;

	/** Sets the Path Node Locations
	* @param nodeLocations The Locations of the path nodes you want the enemy to follow, as a TArray<FVector> */
	UFUNCTION(BlueprintCallable)
	void SetPathNodeLocations(TArray<FVector> nodeLocations);

	/** Gets the Path Node Locations
	* @param index The index
	* @return Gets a Path Node Location, as a FVector */
	UFUNCTION(BlueprintCallable)
	FVector GetNextPathNodeLocation();

	/** Gets the behaviour tree that is assined to this actor 
	* @return Returns the Behaviour Tree, as a UBehaviorTree* */
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviourTree() const;

	/** When the enemy loses all their health or has reached the defending base, on death is called */
	UFUNCTION(BlueprintCallable)
	void OnDeath();

	/** Disables the enemy ready to be pooled */
	UFUNCTION(BlueprintCallable)
	void DisableEnemy();

	/** Enables the enemy and removes from the pool ready to be used */
	UFUNCTION(BlueprintCallable)
	void EnableEnemy();

	/** Gets if the enemy has been disabled or not 
	* @return bIsDisabled, as a bool */
	UFUNCTION(BlueprintCallable)
	bool GetIsEnemyDisabled();

protected:
	virtual void BeginPlay() override;

	/** A pointer to the the Behaviour Tree assigned in the editor */
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* enemyBehaviorTree;

	/** The enemy AI Controller */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	AAIController* enemyAIController;

	/** AI Component so the enemy can use AIPerception from the AI Controller */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	/** An array of vectors that is the location for the paths nodes for the AI to follow */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TArray<FVector> pathNodeLocations;

	/** An index of what node the ai is moving towards */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	int pathNodeIndex;

	/** Increaces the Path Node Index */
	UFUNCTION(BlueprintCallable)
	void IncreasePathNodeIndex();

	/** The health component so the enemies can reduce in health when damaged */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UAC_Health* healthComponent;

	/** A check to see if the enemy is disabled and in the pool within the enemy spawner */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDisabled;

	/** Assign a Data Asset in Blueprints for each of the enemys information */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	TObjectPtr<UDA_EnemyCharacterStats> enemyInfo;

	//The built in take damage function
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** On death, check to see if they will drop the item */
	UFUNCTION(BlueprintCallable, Category = "Drop")
	void SpawnDrop();

	/** A TMap of all the enums and the value you give it within the editor to determin the drop weight */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Drop")
	TMap<EEnemyDrop, float> dropWeight;
};
