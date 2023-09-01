#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float takedDamageValue = DamageData.DamageValue;
	CurrentHealth -= takedDamageValue;

	if (CurrentHealth <= 0) {
		if (OnDie.IsBound())
			OnDie.Broadcast(DamageData.Instigator);
	}
	else {
		if (OnDamaged.IsBound())
			OnDamaged.Broadcast(DamageData);
	}
}

int UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return (float)CurrentHealth / (float)MaxHealth;
}

void UHealthComponent::AddHealth(int AddiditionalHealthValue)
{
	CurrentHealth += AddiditionalHealthValue;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}

