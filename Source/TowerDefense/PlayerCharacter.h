#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DA_PlayerCharacterStats.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectileBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TOWERDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Player Stats, Edit in Blueprints Data Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	TObjectPtr<UDA_PlayerCharacterStats> DA_playerInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float movementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float runSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	class TSubclassOf<AProjectileBase> projectile;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components added to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* weapon;



	//These properties are assigned in the begin play by referencing the DA_playerInfo Data Asset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	FString name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float mana;

	//probably be in the weapon in the future?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float damageDelt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float jumpHeight;
};
