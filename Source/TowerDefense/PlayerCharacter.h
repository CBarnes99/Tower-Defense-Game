#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DA_PlayerCharacterStats.h"
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
	TObjectPtr<UDA_PlayerCharacterStats> DA_PlayerInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float RunSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components added to player
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float Mana;

	//probably be in the weapon?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float DamageDelt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float JumpHeight;
};
