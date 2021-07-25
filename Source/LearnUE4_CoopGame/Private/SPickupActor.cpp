// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupActor.h"

#include <Components/SphereComponent.h>
#include <Components/DecalComponent.h>

// Sets default values
ASPickupActor::ASPickupActor()
{
	sphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(sphereComponent);
	sphereComponent->SetSphereRadius(75);

	decalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	decalComponent->SetupAttachment(GetRootComponent());
	decalComponent->SetRelativeRotation(FRotator(90, 0, 0));
	decalComponent->DecalSize = FVector(64, 75, 75);
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


}
