// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "SWeapon.h"

#include "Components/SHealthComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PhysicsEngine/RadialForceComponent.h>
#include <DrawDebugHelpers.h>
#include <Net/UnrealNetwork.h>

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
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 350;
	RadialForceComponent->ImpulseStrength = 21000;
	RadialForceComponent->bIgnoreOwningActor = true; // Ignore self
	RadialForceComponent->bImpulseVelChange = false; 
	RadialForceComponent->bAutoActivate = false; // Prevent component from ticking, use FireImpulse() instead

	SetReplicates(true);
	SetReplicateMovement(true);
	MinNetUpdateFrequency = 60;
	MinNetUpdateFrequency = 30;
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
	if (bExploded)
	{
		return;
	}

	if (CurrentHealth <= 0)
	{
		// Explode
		bExploded = true;

		// Server won't call this but the effect should occur on the server too
		// so this function is forced to invoke
		OnRep_Exploded();

		// Blow it self up
		if (StaticMeshComponent)
			StaticMeshComponent->AddImpulse(BoostIntensity, NAME_None, true);

		if (RadialForceComponent)
		{
			TArray<AActor*> ignoredActor;
			ignoredActor.Add(this);

			// Deals damage
			UGameplayStatics::ApplyRadialDamage(GetWorld(), 20, GetActorLocation(), RadialForceComponent->Radius, DamageType, ignoredActor, this, nullptr, true);

			// Push objects away
			RadialForceComponent->FireImpulse();

			// Debug
			if (ASWeapon::DebugWeaponDrawing > 0)
			{
				DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComponent->Radius, 12, FColor::Red, false, 1.5, 0, 1);
			}
		}
	}
}

void ASExplosiveBarrel::OnRep_Exploded()
{
	PlayEffect();
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

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASExplosiveBarrel, bExploded);
}