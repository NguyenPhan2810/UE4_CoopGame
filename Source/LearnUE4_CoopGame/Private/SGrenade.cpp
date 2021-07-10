// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenade.h"
#include <Components/MeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ASGrenade::ASGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTimer = 1; // second

	MeshComponent = CreateDefaultSubobject<UMeshComponent>("StaticMesh");

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->InitialSpeed = 500;
	ProjectileMovementComponent->MaxSpeed = 500;
}

// Called when the game starts or when spawned
void ASGrenade::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(ExplosionTimer);
}

// Called every frame
void ASGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGrenade::BeginDestroy()
{
	//UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
}

