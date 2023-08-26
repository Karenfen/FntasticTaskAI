// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAITrigger.h"
#include "Components/BoxComponent.h"
#include "MyProjectCharacter.h"
#include "MyProjectGameMode.h"
#include "Engine/World.h"




void AMyAITrigger::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (IsValid(world)) {
		AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(world->GetAuthGameMode());

		if (IsValid(gameMode)) {
			gameMode->SetAITrigger(this);
		}
	}
}

// Sets default values
AMyAITrigger::AMyAITrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;
	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger	collider"));
	TriggerCollider->SetupAttachment(sceneComp);
	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyAITrigger::OnTriggerOverlapBegin);
}

void AMyAITrigger::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const	FHitResult& SweepResult) {

	AMyProjectCharacter* character = Cast<AMyProjectCharacter>(OtherActor);
	if (IsValid(character)) {
		if (OnActivated.IsBound()) {
			OnActivated.Broadcast(character);
		}
	}
}