// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterI.h"
#include "../components/HealthComponent.h"
#include "Engine/Engine.h"


AMyCharacterI::AMyCharacterI()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health	component"));
	if (HealthComponent) {
		HealthComponent->OnDie.AddUObject(this, &AMyCharacterI::Die);
		HealthComponent->OnDamaged.AddUObject(this, &AMyCharacterI::DamageTaked);
	}
}

void AMyCharacterI::TakeDamage_(FDamageData DamageData)
{
	if (IsValid(HealthComponent)) {
		HealthComponent->TakeDamage(DamageData);
	}

	// Формируем строку для отображения
	FString DebugText = FString::Printf(TEXT("Tacked damege! Attacker: %s, Damage: %d"), *DamageData.Gunner->GetName(), DamageData.DamageValue);

	// Отобразить сообщение на экране
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, DebugText);
}

void AMyCharacterI::DamageTaked(FDamageData damageData)
{
	// Формируем строку для отображения
	FString DebugText = FString::Printf(TEXT("Damage: %d Health: %.d"), damageData.DamageValue, HealthComponent->GetCurrentHealth());

	// Отобразить сообщение на экране
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Green, DebugText);
}

void AMyCharacterI::Die(FDamageData data)
{
	// Формируем строку для отображения
	FString DebugText = FString::Printf(TEXT("IS DEAD! Killer: %s"), *data.Gunner->GetName());

	// Отобразить сообщение на экране
	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Black, DebugText);
}