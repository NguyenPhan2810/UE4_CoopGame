// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/SHealthComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PhysicsEngine/RadialForceComponent.h>
#include "SWeapon.h"
#include <DrawDebugHelpers.h>

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	HealthComponent = CreateDefaultSubobject<USHealthComponent>("HealthComponent");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMeshComponent->SetMassOverrideInKg(NAME_None, 30);
	SetRootComponent(StaticMeshComponent);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->Radius = 350;
	RadialForceComponent->bIgnoreOwningActor = true; // Ignore self
	RadialForceComponent->bImpulseVelChange = false; 
	RadialForceComponent->bAutoActivate = false; // Prevent component from ticking, use FireImpulse() instead

	//SetReplicates(true);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	if (MaterialNormal)
		StaticMeshComponent->SetMaterial(0, MaterialNormal);

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::HandleHealthChanged);
}

void ASExplosiveBarrel::HandleHealthChanged(USHealthComponent* OwnerHealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* CauserDamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth <= 0)
	{
		// Explode
		PlayEffect();

		if (StaticMeshComponent)
			StaticMeshComponent->AddImpulse(BoostIntensity, NAME_None, true);

		if (RadialForceComponent)
		{
			TArray<AActor*> ignoredActor;
			ignoredActor.Add(this);

			UGameplayStatics::ApplyRadialDamage(GetWorld(), 20, GetActorLocation(), RadialForceComponent->Radius, DamageType, ignoredActor, this, nullptr, true);

			RadialForceComponent->FireImpulse();

			if (ASWeapon::DebugWeaponDrawing > 0)
			{
				DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComponent->Radius, 12, FColor::Red, false, 1.5, 0, 1);
			}
		}
	}
}

void ASExplosiveBarrel::PlayEffect()
{
	if (ExplodeEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorTransform());

	if (MaterialExploded)
		StaticMeshComponent->SetMaterial(0, MaterialExploded);

	if (ExplodeSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplodeSound, GetActorLocation());
}

