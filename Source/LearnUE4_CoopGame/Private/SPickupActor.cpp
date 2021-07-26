// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupActor.h"

#include "SPowerupActor.h"
#include <Components/SphereComponent.h>
#include <Components/DecalComponent.h>

// Sets default values
ASPickupActor::ASPickupActor()
: powerupInstance(nullptr)
, cooldownDuration(1)
, spawnLocationOffset(0, 0, 20)
{
	sphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(sphereComponent);
	sphereComponent->SetSphereRadius(75);

	decalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	decalComponent->SetupAttachment(GetRootComponent());
	decalComponent->SetRelativeRotation(FRotator(90, 0, 0));
	decalComponent->DecalSize = FVector(64, 75, 75);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_Authority)
		Respawn();
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (powerupInstance)
		{
			powerupInstance->ActivatePowerup();
			powerupInstance = nullptr;

			// Respawn powerup
			GetWorldTimerManager().SetTimer(timerHandle_RespawnTimer, this, &ASPickupActor::Respawn, cooldownDuration);
		}
	}
}

void ASPickupActor::Respawn()
{
	if (powerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"Your powerupClass is nullptr in %s, please assign it!", *GetName());
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto spawnTransform = GetTransform();
	spawnTransform.AddToTranslation(spawnLocationOffset);

	powerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(powerupClass, spawnTransform, spawnParams);
}
