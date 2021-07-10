// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTimer = 1;

	//ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	//ProjectileMovementComponent->InitialSpeed = 500;
	//ProjectileMovementComponent->MaxSpeed = 500;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	//SetLifeSpan(ExplosionTimer);
}

void AGrenade::BeginDestroy()
{
	// @TODO: Apply radial damage
	//UE_LOG(LogTemp, Warning, TEXT("Explosion!!"));
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

