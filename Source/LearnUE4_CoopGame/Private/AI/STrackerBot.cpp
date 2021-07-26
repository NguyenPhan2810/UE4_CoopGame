// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "Components/SHealthComponent.h"
#include "SCharacter.h"

#include <Components/StaticMeshComponent.h>
#include <NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>
#include <NavigationPath.h>
#include <DrawDebugHelpers.h>
#include <Components/SphereComponent.h>
#include <Net/UnrealNetwork.h>
#include <Components/AudioComponent.h>

// Sets default values
ASTrackerBot::ASTrackerBot()
: materialInstance(nullptr)
, bUseAccelerationChange(false)
, requiredDistanceToTarget(50)
, maxSpeed(500)
, movementForce(4000)
, explosionRadius(200)
, explosionDamage(40)
, bExploded(false)
, bExplosionSequenceStarted(false)
, selfDamageDamage(20)
, selfDamageInteral(0.3)
, currentSegmentLength(0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(meshComponent);
	meshComponent->SetCanEverAffectNavigation(false);
	meshComponent->SetSimulatePhysics(true);

	healthComponent = CreateDefaultSubobject<USHealthComponent>("HealthComponent");
	healthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleHealthChanged);

	sphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	sphereComponent->SetupAttachment(RootComponent);
	sphereComponent->SetSphereRadius(199);
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Don't let the sphere component simulate any physics
	sphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	audioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	audioComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if (rollingSound)
	{
		audioComponent->SetSound(rollingSound);
		audioComponent->Play();
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		currentSegmentEndPoint = GetActorLocation();
		currentSegmentBeginPoint = GetActorLocation();
	}
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetLocalRole() == ROLE_Authority)
	{
		if (bExplosionSequenceStarted)
		{
			meshComponent->SetAllPhysicsLinearVelocity(FVector(0, 0, 100));
		}
		else
		{
			auto distanceToTarget = (currentSegmentEndPoint - GetActorLocation()).Size();

			if (distanceToTarget < requiredDistanceToTarget || (currentSegmentEndPoint - currentSegmentBeginPoint).IsNearlyZero())
			{
				// Find new target
				currentSegmentEndPoint = GetNextPathPoint();
				currentSegmentBeginPoint = GetActorLocation();
				currentSegmentLength = (currentSegmentEndPoint - currentSegmentBeginPoint).Size();
			}
			else
			{
				// Keep moving to target
				FVector forceDirection = currentSegmentEndPoint - GetActorLocation();
				forceDirection.Normalize();
				meshComponent->AddForce(forceDirection * movementForce, NAME_None, bUseAccelerationChange);

				if (GetVelocity().Size() > maxSpeed)
				{
					auto velDirection = GetVelocity();
					velDirection.Normalize();
					meshComponent->SetAllPhysicsLinearVelocity(velDirection * maxSpeed);
				}
			}
		}
	}
}

// Already works only in server
void ASTrackerBot::HandleHealthChanged(USHealthComponent* HealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth <= 0)
	{
		Explode();
	}

	if (materialInstance == nullptr)
		materialInstance = meshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, meshComponent->GetMaterial(0));

	if (materialInstance)
		materialInstance->SetScalarParameterValue(materialParamLastTimeDamageTaken, GetWorld()->TimeSeconds);
}

void ASTrackerBot::OnRep_Exploded()
{
	PlayExplosionEffects();
}

void ASTrackerBot::OnRep_ExplosionSequenceStarted()
{
	PlayStartExplosionEffects();
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GetLocalRole() == ROLE_Authority)
	{
		auto playerPawn = Cast<ASCharacter>(OtherActor);

		if (playerPawn)
		{
			StartExplosionSequence();
		}
	}
}

void ASTrackerBot::Explode()
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (bExploded)
		return;

	bExploded = true;

	PlayExplosionEffects();

	// Deals damage
	TArray<AActor*> ignoredActor;
	ignoredActor.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), explosionDamage, GetActorLocation(), explosionRadius, damageType, ignoredActor, this, nullptr, true);

	DrawDebugSphere(GetWorld(), GetActorLocation(), explosionRadius, 12, FColor::Red, false, 1);

	// Set life span instead of destroy the actor to let the replication occurs

	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	SetLifeSpan(2);
}

void ASTrackerBot::StartExplosionSequence()
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (bExplosionSequenceStarted)
		return;

	bExplosionSequenceStarted = true;

	PlayStartExplosionEffects();

	GetWorld()->GetTimerManager().SetTimer(timerHandle_ExplosionSequence, this, &ASTrackerBot::SelfDamage, selfDamageInteral, true, 0);
}

void ASTrackerBot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, selfDamageDamage, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::PlayExplosionEffects()
{
	// Stop rolling sound
	if (audioComponent)
		audioComponent->SetPaused(true);

	if (explosionEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorTransform());

	if (explosionSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
}

void ASTrackerBot::PlayStartExplosionEffects()
{
	// Stop rolling sound
	if (audioComponent)
		audioComponent->SetPaused(true);

	// Sound effect
	if (explosionSequenceSound)
		UGameplayStatics::SpawnSoundAttached(explosionSequenceSound, RootComponent);
}

FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* player = nullptr;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			player = PlayerController->GetPawn<ACharacter>();
			break;
		}
	}

	if (player)
	{
		// Find path
		auto path = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), player);

		// Return next point excluding it self
		if (path && path->PathPoints.Num() > 0)
		{
			return path->PathPoints[1];
		}
	}

	// Failed to get next point
	return GetActorLocation();
}

void ASTrackerBot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTrackerBot, bExploded);
	DOREPLIFETIME(ASTrackerBot, bExplosionSequenceStarted);
}