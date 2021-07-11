// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenade.h"
#include <Components/MeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "SWeapon.h"

// Sets default values
ASGrenade::ASGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTimer = 1; // second

	MeshComponent = CreateDefaultSubobject<UMeshComponent>("StaticMesh");
	SetRootComponent(MeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->InitialSpeed = 1000;
	ProjectileMovementComponent->MaxSpeed = 1000;

}

// Called when the game starts or when spawned
void ASGrenade::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &ASGrenade::Explode, ExplosionTimer, false);
}

void ASGrenade::Explode()
{
	TArray<AActor*> ignoredActor;
	ignoredActor.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), 20, GetActorLocation(), 200,
		DamageType, ignoredActor, this, GetOwner()->GetInstigatorController(), true, ECC_Visibility);

	if (ASWeapon::DebugWeaponDrawing > 0)
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), 200, 12, FLinearColor::Red, 1, 1);


	if (ExplodeEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation(), FVector(2, 2, 2));

	Destroy();
}

// Called every frame
void ASGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGrenade::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//Explode();
}

