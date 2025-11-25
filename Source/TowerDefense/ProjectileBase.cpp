#include "ProjectileBase.h"
#include <Kismet/GameplayStatics.h>

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	collisionComponent->InitSphereRadius(20.f);
	collisionComponent->SetSimulatePhysics(false);
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	collisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	collisionComponent->SetEnableGravity(false);

	RootComponent = collisionComponent;
	collisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	projectileMovementComponent->SetUpdatedComponent(collisionComponent);
	projectileMovementComponent->bRotationFollowsVelocity = true;
	projectileMovementComponent->ProjectileGravityScale = 0.f;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	projectileMesh->SetupAttachment(RootComponent.Get());
	projectileMesh->SetSimulatePhysics(false);
	projectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	lifeTime = 5.f;

}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();	

	//isActive = false;
	hasDefaultsBeenSet = false;

	DeactivateProjectile();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::SetDamage(float damageAmount)
{
	damageDelt = damageAmount;
}

void AProjectileBase::SetProjectileSpeed(float speed)
{
	projectileMovementComponent->InitialSpeed = speed;
	projectileMovementComponent->MaxSpeed = speed;
}

bool AProjectileBase::IsProjectileActive()
{
	return isActive; 
}

void AProjectileBase::ActivateProjectile()
{
	
	isActive = true;

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	collisionComponent->IgnoreActorWhenMoving(GetOwner(), true);

	projectileMovementComponent->Activate();

	GetWorld()->GetTimerManager().SetTimer(lifeTimeTimerHandle, this, &AProjectileBase::DeactivateProjectile, lifeTime, false);

	//UE_LOG(LogTemp, Display, TEXT("Using - %s"), *this->GetName());
}

void AProjectileBase::DeactivateProjectile()
{
	projectileMovementComponent->Deactivate();
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	if (GetWorld()->GetTimerManager().IsTimerActive(lifeTimeTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(lifeTimeTimerHandle);
	}

	isActive = false;
	//UE_LOG(LogTemp, Warning, TEXT("Deactivated! - %s"), *this->GetName());
}



void AProjectileBase::FireInDirection(const FVector& shootDir)
{
	projectileMovementComponent->Velocity = shootDir * projectileMovementComponent->InitialSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("Firing Projectile with velocity: %s"), *projectileMovementComponent->Velocity.ToString());
}

void AProjectileBase::OnHit(UPrimitiveComponent* hitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//To prevent the projectile from destorying itself when its spawned from the instigator
	if (!OtherActor || OtherActor == this || OtherActor == this->GetInstigator())
	{
		return;
	}

	//UE_LOG(LogTemp, Display, TEXT("OtherActor that projectile is colliding with is %s"), *OtherActor->GetName())
	UGameplayStatics::ApplyDamage(OtherActor, damageDelt, GetInstigator()->GetController(), this, NULL /*CHANGE THIS TO A DAMAGE TYPE LATER ON*/);

	DeactivateProjectile();
}