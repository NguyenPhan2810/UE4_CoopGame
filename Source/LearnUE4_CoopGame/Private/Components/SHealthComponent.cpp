// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include <Delegates/Delegate.h>
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	DefaultMaxHealth = 100;
	Health = DefaultMaxHealth;

	SetIsReplicated(true);
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultMaxHealth;

	
	// Subcribe it self to take damage event of the owner
	auto owner = GetOwner();
	if (owner)
	{
		// Only hook up if this is on server side
		if (GetOwnerRole() == ROLE_Authority)
			owner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage == 0 || Health <= 0)
	{
		return;
	}

	// Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultMaxHealth);

	// Board cast event
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("Health changed: %f"), Health);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}
