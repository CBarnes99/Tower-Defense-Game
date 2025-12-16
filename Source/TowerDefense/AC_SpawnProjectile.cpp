#include "AC_SpawnProjectile.h"
#include "DrawDebugHelpers.h"

UAC_SpawnProjectile::UAC_SpawnProjectile()
{
	PrimaryComponentTick.bCanEverTick = false;

	poolSize = 20;
}

void UAC_SpawnProjectile::BeginPlay()
{
	Super::BeginPlay();
	amountInPool = 0;

	InitializePool();
}

void UAC_SpawnProjectile::InitializePool()
{
	if (!projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("No Projectile Class Within - %s"), *this->GetOwner()->GetName());
	}

	for (int i = 0; i < poolSize; i++)
	{
	/*	FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetOwner();
		spawnParams.Instigator = GetOwner()->GetInstigator();
		AProjectileBase* pooledProjectile = GetWorld()->SpawnActor<AProjectileBase>(projectile, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

		FString lable = FString::Printf(TEXT("%s - %d"), *projectile->GetName(), i + 1);
		pooledProjectile->SetActorLabel(lable);
		projectilePool.Add(pooledProjectile);

		amountInPool++;*/

		spawnProjectile();

		//UE_LOG(LogTemp, Error, TEXT("%s owner = %s"), *this->GetName(), *this->GetOwner()->GetName());
	}
	UE_LOG(LogTemp, Warning, TEXT("There are %d projectiles pooled"), amountInPool);

}

void UAC_SpawnProjectile::spawnProjectile()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = GetOwner()->GetInstigator();
	AProjectileBase* pooledProjectile = GetWorld()->SpawnActor<AProjectileBase>(projectile, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

	FString lable = FString::Printf(TEXT("%s - %d"), *projectile->GetName(), amountInPool + 1);
	pooledProjectile->SetActorLabel(lable);
	projectilePool.Add(pooledProjectile);

	amountInPool++;
}

void UAC_SpawnProjectile::FireProjectile(FVector traceStartLocation, FVector weaponMuzzleLocation, FVector actorForwardVector, float damageDelt, float projectileSpeed)
{
	if (!projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile class is not set in AC_SpawnProjectile within %s!"), *GetOwner()->GetName());
		return;
	}

	FVector targetLocation = GetTraceTargetLocation(traceStartLocation, actorForwardVector);

	FVector shootDirection = (targetLocation - weaponMuzzleLocation).GetSafeNormal();

	FRotator spawnRotation = shootDirection.Rotation();

	FVector spawnLocation = weaponMuzzleLocation + shootDirection * 10.0f;


	//FOR DEBUGGING, SPAWNS IN THE PROJECTILE INSTEAD OF FROM THE POOL
	/*FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = GetOwner()->GetInstigator();
	AProjectileBase* currentProjectile = GetWorld()->SpawnActor<AProjectileBase>(projectile, spawnLocation, spawnRotation, spawnParams);*/

	AProjectileBase* currentProjectile = GetInactiveProjectile();

	if (!currentProjectile)
	{
		//return;
		TRACE_BOOKMARK(TEXT("SpawnedProjectile"));
		spawnProjectile();
		currentProjectile = GetInactiveProjectile();
		poolSize++;
		UE_LOG(LogTemp, Warning, TEXT("Added another projectile to the pool for - %s. There are %d projectiles within the pool"), *GetOwner()->GetName(), poolSize);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Using - %s"), *currentProjectile->GetName());

	currentProjectile->SetActorLocation(spawnLocation);
	currentProjectile->SetActorRotation(spawnRotation);

	if (!currentProjectile->hasDefaultsBeenSet)
	{
		currentProjectile->SetDamage(damageDelt);
		currentProjectile->SetProjectileSpeed(projectileSpeed);
		currentProjectile->hasDefaultsBeenSet = true;
	}
	currentProjectile->FireInDirection(shootDirection);
	currentProjectile->ActivateProjectile();

	DrawDebugSphere(GetWorld(), targetLocation, 15.f, 12, FColor::Green, false, 2.f);
	//UE_LOG(LogTemp, Display, TEXT("Velocity = %s"), *currentProjectile->projectileMovementComponent->Velocity.ToString());

}

AProjectileBase* UAC_SpawnProjectile::GetInactiveProjectile()
{
	for (AProjectileBase* p : projectilePool)
	{
		if (!p->IsProjectileActive()) return p;
	}
	//UE_LOG(LogTemp, Error, TEXT("No Inactive Projectiles Within %s. Create A Larger Pool Size"), *this->GetOwner()->GetName());
	return nullptr;
}

FVector UAC_SpawnProjectile::GetTraceTargetLocation(FVector traceStartLocation, FVector actorForwardVector)
{
	FVector targetPos = FVector::ZeroVector;
	FHitResult hit;
	FVector traceEnd = traceStartLocation + actorForwardVector * 5000.f;

	GetWorld()->LineTraceSingleByChannel(hit, traceStartLocation, traceEnd, ECC_Pawn);

	DrawDebugLine(GetWorld(), traceStartLocation, traceEnd, FColor::Red, false, 2.f, 1.f);

	targetPos = hit.bBlockingHit ? hit.ImpactPoint : traceEnd;

	return targetPos;
}
