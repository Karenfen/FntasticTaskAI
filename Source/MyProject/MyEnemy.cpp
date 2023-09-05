// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "components/HealthComponent.h"
#include "components/GameStruct.h"
#include "Engine/Engine.h"


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
	// ����������� ������� HP
	// ��������������� ����� ��� ���������
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

void AMyEnemy::Attack()
{
	if (!GetWorldTimerManager().IsTimerActive(AttackTimerHandle)) {
		// ��������� ������ ��� �����������
		FString DebugText = FString::Printf(TEXT("Attacker: %s, Damage: %d"), *GetName(), attackDamage);

		// ���������� ��������� �� ������
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Blue, DebugText);

		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, attackTimeout, false);

        // �������� �������������� � ����������� ������� ���������
        FVector StartLocation = GetActorLocation();
        FRotator PlayerRotation = GetActorRotation();
        FVector EndLocation = StartLocation + (PlayerRotation.Vector() * attackRange);

        // ��������� ��� ����
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); // ���������� ��������� ��� ��������

        // ���������� ������������ ����
        if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, CollisionParams)){

			IDamageTaker* hitActor = Cast<IDamageTaker>(HitResult.GetActor());
			if (hitActor != nullptr) {
				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);

				FDamageData damageData;
				damageData.DamageValue = attackDamage;
				damageData.Gunner = this;

				hitActor->TakeDamage_(damageData);

				return;
			}
        }

		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1, 0, 1);
	}
}

void AMyEnemy::Die(FDamageData data)
{
	// ��������� AI
	// ��������� �������� ��� ������ (��������, �����)
	if (OnDie.IsBound())
		OnDie.Broadcast();
}