#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SoberGameplayAbility.generated.h"

class ABaseCharacter;
class USoberAbilitySystemComponent;

UCLASS()
class SOBER_API USoberGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    USoberGameplayAbility();

protected:
    // Helper truy cập nhanh, tránh lặp cast ở mọi ability con
    UFUNCTION(BlueprintPure, Category = "Sober|Ability")
    USoberAbilitySystemComponent* GetSoberASC() const;

    UFUNCTION(BlueprintPure, Category = "Sober|Ability")
    ABaseCharacter* GetOwningBaseCharacter() const;

    // Input tag để BaseCharacter/PlayerController map Enhanced Input -> Ability này
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sober|Input")
    FGameplayTag InputTag;

public:
    FGameplayTag GetInputTag() const { return InputTag; }
};
