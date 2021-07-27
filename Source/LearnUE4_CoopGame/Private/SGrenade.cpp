// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenade.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <PhysicsEngine/RadialForceComponent.h>
#include "SWeapon.h"
#include "../LearnUE4_CoopGame.h"

// Sets default values
ASGrenade::ASGrenade()
: damageAmount(100)
, ExplodeOnImpact(true)
, ExplosionTimer(1) // second
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(MeshComponent);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetMassOverrideInKg(NAME_None, 1);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 300;
	RadialForceComponent->ImpulseStrength = 15000;
	RadialForceComponent->bIgnoreOwningActor = true; // Ignore self
	RadialForceComponent->bImpulseVelChange = false;
	RadialForceComponent->bAutoActivate = false; // Prevent component from ticking, use FireImpulse() instead
}

// Called when the game starts or when spawned
void ASGrenade::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &ASGrenade::Explode, ExplosionTimer, false);
}

void ASGrenade::Explode()
{
	if (RadialForceComponent)
	{
		TArray<AActor*> ignoredActor;
		ignoredActor.Add(this);

		UGameplayStatics::ApplyRadialDamage(GetWorld(), damageAmount, GetActorLocation(), RadialForceComponent->Radius,
			DamageType, ignoredActor, this, GetOwner()->GetInstigatorController());

		RadialForceComponent->FireImpulse();

		if (ASWeapon::DebugWeaponDrawing > 0)
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComponent->Radius, 12, FLinearColor::Red, 1.5, 1);

		if (ExplodeEffect)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation(), FVector(2, 2, 2));

		if (ExplodeSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplodeSound, GetActorLocation());
	}
	Destroy();
}

// Called every frame
void ASGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGrenade::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Ignore self
	if (ExplodeOnImpact && Other != this)
		Explode();
}

