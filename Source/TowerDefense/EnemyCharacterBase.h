#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "E_EnemyDrop.h"
#include "EnemyCharacterBase.generated.h"

class UBehaviourTree;
class UDA_EnemyCharacterStats;
class UAC_Health;
class AEnemyAIController;
class UAIPerceptionStimuliSourceComponent;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathBlueprintSignature, AEnemyCharacterBase*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDisabledBlueprintSignature);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemyCharacterBase* /* Enemy */);
DECLARE_DELEGATE(FOnDisableAIControllerSignature);
DECLARE_DELEGATE_TwoParams(FOnSpawnEnemyDropSignature, EEnemyDrop, /*The drop type to spawn*/ FVector /* The location the drop is moved to */);

UCLASS()
class TOWERDEFENSE_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Bound in the Spawner manager to keep track of how many enemies are left in the round */
	FOnEnemyDeathSignature OnEnemyDeathEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeathBlueprintSignature OnEnemyDeathBlueprintEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDisabledBlueprintSignature OnEnemyDisabledBlueprintEvent;

	/** When the enemy has been defeated, DisableEnemy() is called and this Event is called */
	FOnDisableAIControllerSignature OnDisableAIControllerEvent;

	/** When the enemy has been defeated, drop is moved to the location of the enemy, bound within the spawner manager */
	FOnSpawnEnemyDropSignature OnSpawnEnemyDropEvent;

	/** Sets the Path Node Locations
	* @param nodeLocations The Locations of the path nodes you want the enemy to follow, as a TArray<FVector> */
	UFUNCTION(BlueprintCallable)
	void SetPathNodeLocations(TArray<FVector> nodeLocations);

	/** Gets the Path Node Location, called UBTTask_SetPathNodeLocation in the the Behaviour Tree
	* @return Gets a Path Node Location, as a FVector */
	UFUNCTION(BlueprintCallable)
	FVector GetNextPathNodeLocation();

	/** Gets the closest Path Node Location, called UBTTask_SetPathNodeLocation in the the Behaviour Tree
	* @return Gets a Path Node Location, as a FVector */
	UFUNCTION(BlueprintCallable)
	FVector GetClosestPathNodeLocation();

	/** Gets the behaviour tree that is assined to this actor 
	* @return Returns the Behaviour Tree, as a UBehaviorTree* */
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviourTree() const;

	/** When the enemy loses all their health or has reached the defending base, on death is called */
	UFUNCTION(BlueprintCallable)
	void OnDeath();

	/** Disables the enemy ready to be pooled */
	UFUNCTION(BlueprintCallable, Category = "Pooling")
	void DisableEnemy();

	/** Enables the enemy and removes from the pool ready to be used */
	UFUNCTION(BlueprintCallable, Category = "Pooling")
	void EnableEnemy();

	/** Gets if the enemy has been disabled or not 
	* @return bIsDisabled true = enemy is disabled, false = enemy is enabled, as a bool */
	UFUNCTION(BlueprintCallable, Category = "Pooling")
	bool GetIsEnemyDisabled();

	/** Called to Enable or Disable the enemy perception component
	* @param bDeativatePerception true = Deactivates perception, false = Activates perception */
	UFUNCTION(BlueprintCallable)
	void DisableOrEnablePerceptionComponent(bool bDeativatePerception);

	/** Gets the amount of damage the enemy will deal to the player
	* @return A damage value, as a float */
	UFUNCTION(BlueprintCallable)
	float GetEnemyDamageAmount();

	/** When the enemy dies, ths is called from within enemy animation blueprint */
	UFUNCTION(BlueprintCallable)
	void OnDeathAnimationStarted();

protected:
	virtual void BeginPlay() override;

	/** A pointer to the the Behaviour Tree assigned in the editor */
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* enemyBehaviorTree;

	/** The enemy AI Controller */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	AEnemyAIController* enemyAIController;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pooling")
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
