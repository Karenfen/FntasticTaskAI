#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(UHealthComponent, FOnDie, AActor*)
	DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, FDamageData)

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
	int MaxHealth = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
	int CurrentHealth = 0;

public:
	FOnDie OnDie;
	FOnHealthChanged OnDamaged;

public:	
	UHealthComponent();

	void TakeDamage(FDamageData DamageData);
	int GetHealth() const;
	float GetHealthState() const;
	void AddHealth(int AddiditionalHealthValue);	
	FORCEINLINE int GetCurrentHealth() const { return CurrentHealth; };
	FORCEINLINE int GetMaxHealth() const { return MaxHealth; };
	FORCEINLINE void SetCurrentHealth(int newHealth) { CurrentHealth = newHealth; };
};
