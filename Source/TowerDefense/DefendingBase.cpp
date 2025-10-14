// Fill out your copyright notice in the Description page of Project Settings.


#include "DefendingBase.h"

// Sets default values
ADefendingBase::ADefendingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	base->SetupAttachment(RootComponent.Get());

}

// Called when the game starts or when spawned
void ADefendingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefendingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

