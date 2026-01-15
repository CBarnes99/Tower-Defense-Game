#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UDA_PlayerCharacterStats;
class AWeaponBase;
class UAC_Health;
class UAC_Mana;
class UCameraComponent;
class USpringArmComponent;
class AWeaponBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdatedSigniture, float, /*Curent Health*/ float /*Max Health*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnManaUpdatedSigniture, float, /*Curent Mana*/ float /*Max Mana*/)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeathStateSigniture, bool, bIsPlayerDead);

UCLASS()
class TOWERDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FOnHealthUpdatedSigniture OnHealthUpdatedEvent;

	FOnManaUpdatedSigniture OnManaUpdatedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathStateSigniture OnPlayerDeathStateEvent;

	/** Player information from a data asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
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
	FVector GetCameraLocation();

	/** An Actor Component that controlls the health of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UAC_Health* healthComponent;

	/** An Actor Component that controlls the mana of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UAC_Mana* manaComponent;

	/** When the attack action is called by the Core_PlayerController, it fires a projectile from the weapon */
	UFUNCTION(BlueprintCallable)
	void AttackAction();

	/** When the player recieves healing, call this function */
	UFUNCTION(BlueprintCallable)
	void ReceiveHealing(float healAmount);

	/** When the player recieves healing, call this function */
	UFUNCTION(BlueprintCallable)
	void ReceiveMana(float manaAmount);

protected:
	virtual void BeginPlay() override;

	/** Camera Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* camera;

	/** Spring Arm Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* springArm;

	/** This is the socket name on the skeleton, this is where the weapon will attach to on the skeleton */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName weaponSocket;

	/** The weapons class the character holds and fires */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponBase> weaponClass;

	/** On begin play, this function spawns the weapon above and equips it to the socket of the skeleton that has the name of weaponSocket */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon();

	/** The defualt takeDamage function */
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** When the player recieves healing, broardcast healing recieved delegate */
	UFUNCTION(BlueprintCallable)
	void ReceiveHealingDelegate(float currentHealth, float maxHealth);

	/** When the player recieves mana, broardcast mana recieved delegate */
	UFUNCTION(BlueprintCallable)
	void ReceiveManaDelegate(float currentMana, float maxMana);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead;
};
