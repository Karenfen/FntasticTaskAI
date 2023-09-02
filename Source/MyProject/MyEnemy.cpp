// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "components/HealthComponent.h"
#include "components/GameStruct.h"


// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health	component"));
	if(HealthComponent) {
		HealthComponent->OnDie.AddUObject(this, &AMyEnemy::Die);
		HealthComponent->OnDamaged.AddUObject(this, &AMyEnemy::DamageTaked);
	}
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyEnemy::DamageTaked(FDamageData damageData)
{
	// отображение полоски HP
	// воспроизведение звука при попадании
	if (OnAttacked.IsBound()) {
		OnAttacked.Broadcast(damageData);
	}
}

void AMyEnemy::TakeDamage_(FDamageData DamageData)
{
	if (IsValid(HealthComponent)) {
		HealthComponent->TakeDamage(DamageData);
	}
}

void AMyEnemy::Die(FDamageData data)
{
	// выключаем AI
	// запускаем действия при смерти (анимации, звуки)
	if (OnDie.IsBound())
		OnDie.Broadcast();
}