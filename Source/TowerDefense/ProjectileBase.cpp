// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	collisionComponent->InitSphereRadius(20.f);
	collisionComponent->SetSimulatePhysics(false);
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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


	lifeTime = 5.f;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();	

	GetWorld()->GetTimerManager().SetTimer(lifeTimeTimerHandle, this, &AProjectileBase::destoryProjectileActor, lifeTime, false);
}

// Called every frame
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

void AProjectileBase::destoryProjectileActor()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is destroyed"), *this->GetName());
	AProjectileBase::Destroy();
}

void AProjectileBase::FireInDirection(const FVector& shootDir)
{
	projectileMovementComponent->Velocity = shootDir * projectileMovementComponent->InitialSpeed;
}

void AProjectileBase::OnHit(UPrimitiveComponent* hitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//To prevent the projectile from destorying itself when its spawned from the instigator
	if (!OtherActor || OtherActor == this || OtherActor == this->GetInstigator())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OtherActor is %s"), *OtherActor->GetName())
	if (targetClassToDestroy && OtherActor->IsA(targetClassToDestroy))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has been destroyed"), *OtherActor->GetName())
		OtherActor->Destroy();
	}
	destoryProjectileActor();
}