#include "WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "DA_WeaponInfo.h"
#include "AC_SpawnProjectile.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = weaponMesh;

	spawnProjectileComponent = CreateDefaultSubobject<UAC_SpawnProjectile>(TEXT("Spawn Projectile Component"));
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	spawnProjectileComponent->InitializePool(weaponStats->fireRate, weaponStats->lifetime);
	bCanFire = true;
}

FVector AWeaponBase::GetWeaponMuzzleLocation()
{
	return weaponMesh->GetSocketLocation(weaponStats->muzzleName);
}

float AWeaponBase::GetDamageDelt()
{
	return weaponStats->damageDelt;
}

float AWeaponBase::GetProjectileSpeed()
{
	return weaponStats->projectileSpeed;
}

float AWeaponBase::GetProjectileLifetime()
{
	return weaponStats->lifetime;
}

void AWeaponBase::FireProjectile(FVector fireStartLoc, FVector forwardVector)
{
	if (!bCanFire)
	{
		return;
	}

	spawnProjectileComponent->FireProjectile
	(
		fireStartLoc,
		GetWeaponMuzzleLocation(),
		forwardVector,
		GetDamageDelt(),
		GetProjectileSpeed(),
		GetProjectileLifetime()
	);
	StartWeaponFireRateCooldown();
	DisableCanFire();
}

void AWeaponBase::StartWeaponFireRateCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(fireRateHandle, this, &AWeaponBase::EnableCanFire, 1 / weaponStats->fireRate, false);
}

void AWeaponBase::EnableCanFire()
{
	bCanFire = true;
	//UE_LOG(LogTemp, Display, TEXT("Enabled Firing"));
}

void AWeaponBase::DisableCanFire()
{
	bCanFire = false;
	//UE_LOG(LogTemp, Display, TEXT("Disabled Firing"));
}
