// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretStatic.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "EnemyCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurretStatic::ATurretStatic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	//RootComponent = turretMesh;
	turretMesh->SetupAttachment(RootComponent);

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	collisionBox->SetCollisionProfileName(TEXT("Trigger"));
	collisionBox->SetGenerateOverlapEvents(true);
	collisionBoxSize = FVector(150, 50, 50);
	collisionBox->SetBoxExtent(FVector(collisionBoxSize));
	collisionBox->SetRelativeLocation(FVector(collisionBoxSize.X, 0, 0));
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATurretStatic::OnOverlap);

	collisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh"));
	collisionMesh->SetCollisionProfileName("NoCollision", false);
	

    arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Arrow"));

    collisionBox->SetupAttachment(turretMesh);
	collisionMesh->SetupAttachment(collisionBox);
    arrowComponent->SetupAttachment(turretMesh);

	previewMeshComponent = CreateDefaultSubobject<UAC_PreviewMesh>(TEXT("Preview Mesh Component"));
}

// Called when the game starts or when spawned
void ATurretStatic::BeginPlay()
{
	Super::BeginPlay();

	damageAmount = turretStats->damageAmount;
	activeTime = turretStats->activeTime;
	rechargeTime = turretStats->rechargeTime;
	cost = turretStats->cost;

	turretActive = true;
	turretRecharging = false;

}
void ATurretStatic::OnConstruction(const FTransform& Transform)
{
	collisionMesh->SetRelativeScale3D(collisionBoxSize / 125);	
}



void ATurretStatic::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(AEnemyCharacterBase::StaticClass()) && !damagedActors.Contains(OtherActor))
    {
		if (turretActive)
		{
			turretActive = false;
			GetWorld()->GetTimerManager().SetTimer(activeAndRechargeTimerHandle, this, &ATurretStatic::disableTurret, activeTime, false);
		}

		if (GetLocalRole() == ROLE_Authority)
		{
			UE_LOG(LogTemp, Error, TEXT("%s Overlaps %s"), *OtherActor->GetName(), *this->GetName());

			UGameplayStatics::ApplyDamage(OtherActor, damageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
			damagedActors.Add(OtherActor);
		}
    }
}

void ATurretStatic::disableTurret()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is disabled"), *this->GetName());
	collisionBox->SetGenerateOverlapEvents(false);
	turretRecharging = true;
	damagedActors.Empty();
	GetWorld()->GetTimerManager().SetTimer(activeAndRechargeTimerHandle, this, &ATurretStatic::enableTurret, rechargeTime, false);
}

void ATurretStatic::enableTurret()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is enabled"), *this->GetName());
	collisionBox->SetGenerateOverlapEvents(true);
	turretRecharging = false;
	turretActive = true;
}

// Called every frame
void ATurretStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretStatic::SetMaterial(bool isPreview)
{
	if (isPreview)
	{
		turretMesh->SetMaterial(0, previewMeshComponent->outlineMaterial);
	}
	else
	{
		turretMesh->SetMaterial(0, previewMeshComponent->normalMaterial);
	}
}

