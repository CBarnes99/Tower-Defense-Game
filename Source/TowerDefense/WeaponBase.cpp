// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = weaponMesh;

	weaponMuzzleName = "Muzzle";
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	damageDelt = weaponStats->damageDelt;
	currentAmmo = weaponStats->ammoMax;
	maxAmmo = weaponStats->ammoMax;

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWeaponBase::spawnProjectile(const UCameraComponent* playerCamera)
{
	if (!projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile class is not set on Weapon Base!"));
		return;
	}

	FVector muzzleLocation = weaponMesh->GetSocketLocation(weaponMuzzleName);

	FVector targetLocation = GetTraceTargetLocation(playerCamera);

	FVector shootDirection = (targetLocation - muzzleLocation).GetSafeNormal();

	FRotator spawnRotation = shootDirection.Rotation();

	FVector spawnLocation = muzzleLocation + shootDirection * 100.0f;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	AProjectileBase* spawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(projectile, spawnLocation, spawnRotation, spawnParams);
	spawnedProjectile->FireInDirection(shootDirection);
}

FVector AWeaponBase::GetTraceTargetLocation(const UCameraComponent* playerCamera)
{
	FVector targetPos = FVector::ZeroVector;
	FHitResult hit;
	FVector traceStart = playerCamera->GetComponentLocation();
	FVector traceEnd = traceStart + playerCamera->GetForwardVector() * 100000.f;

	GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility);

	DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 2.f, 1.f);

	targetPos = hit.bBlockingHit ? hit.ImpactPoint : traceEnd;

	return targetPos;
}