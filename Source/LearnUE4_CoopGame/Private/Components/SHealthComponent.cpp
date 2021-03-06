// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include <Delegates/Delegate.h>
#include <Net/UnrealNetwork.h>
#include "SGameMode.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
: defaultMaxHealth(100)
, health(100)
, teamIndex(0)
{
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	health = defaultMaxHealth;
	
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
	if (Damage == 0 || health <= 0)
	{
		return;
	}

	if (DamageCauser != DamagedActor && IsSameTeam(DamagedActor, DamageCauser))
	{
		return;
	}

	// Update health clamped
	health = FMath::Clamp(health - Damage, 0.0f, defaultMaxHealth);

	// Board cast event
	OnHealthChanged.Broadcast(this, health, Damage, DamageType, InstigatedBy, DamageCauser);

	auto gameMode = GetWorld()->GetAuthGameMode<ASGameMode>();
	if (gameMode && health <= 0) // Only true in server
	{
		gameMode->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
	}
}

void USHealthComponent::Heal(float healAmount)
{
	if (healAmount <= 0 || health <= 0)
	{
		return;
	}

	health = FMath::Clamp(health + healAmount, 0.0f, defaultMaxHealth);

	OnHealthChanged.Broadcast(this, health, -healAmount, nullptr, nullptr, nullptr);
}

float USHealthComponent::Gethealth() const
{
	return health;
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float damage = health - OldHealth;

	// Board cast event in clients
	OnHealthChanged.Broadcast(this, health, damage, nullptr, nullptr, nullptr);
}

bool USHealthComponent::IsSameTeam(const AActor* actorA, const AActor* actorB)
{
	if (actorA == nullptr || actorB == nullptr)
	{
		return true; // assume same team
	}

	auto healthCompA = actorA->FindComponentByClass<USHealthComponent>();
	auto healthCompB = actorB->FindComponentByClass<USHealthComponent>();

	if (healthCompA == nullptr || healthCompB == nullptr)
	{
		return true; // assume same team
	}

	return healthCompA->teamIndex == healthCompB->teamIndex;
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, health);
}
