#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DA_PlayerCharacterStats.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponBase.h"
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

	UFUNCTION(BlueprintCallable)
	float& GetMovementSpeed();

	UFUNCTION(BlueprintCallable)
	float& GetRunSpeed();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	AWeaponBase* equippedWeapon;

	UFUNCTION(BlueprintCallable)
	FRotator GetCameraRotation() const;

	UFUNCTION(BlueprintCallable)
	FVector GetCameraForwardVector() const;

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetPlayerCamera() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components added to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* springArm;

	//This is the socket name on the skeleton, if the socket name changes, can be edited in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName weaponSocket;

	//These properties are assigned in the begin play by referencing the DA_playerInfo Data Asset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	FString name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float jumpHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float movementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float runSpeed;

	//Set the class of the weapon in the BP Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class TSubclassOf<AWeaponBase> weaponClass;

	//On begin play, this function spawns the weapon above and equips it to the right hand socket of the skeleton
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
