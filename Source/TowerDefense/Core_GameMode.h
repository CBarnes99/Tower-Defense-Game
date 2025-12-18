#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnerManager.h"
#include "Core_PlayerController.h"
#include "Core_GameMode.generated.h"

UCLASS()
class TOWERDEFENSE_API ACore_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	ACore_GameMode();

	virtual void BeginPlay() override;

protected:
	/**
	* @brief The spawner manager in the level
	*/
	UPROPERTY(VisibleAnywhere)
	ASpawnerManager* spawnerManager;

	/**
	* @brief The current wave the level is on, starts at 0 on begin play
	*/
	UPROPERTY(VisibleAnywhere)
	int currentWave;

	/**
	* @brief This is called when the player calls to start the next wave
	*/
	UFUNCTION(BlueprintCallable)
	void StartEnemyWave();

	/**
	* @brief The final wave in the level, is set in StartEnemyWave()
	*/
	UPROPERTY(VisibleAnywhere)
	int lastWave;

	/**
	* @brief The player controller used during combat
	*/
	UPROPERTY(VisibleAnywhere)
	ACore_PlayerController* corePlayerController;

	/**
	* @brief Checks if theres a new wave or if you've finished the last wave
	*/
	UFUNCTION(BlueprintCallable)
	void PrepareNewWave();
};
