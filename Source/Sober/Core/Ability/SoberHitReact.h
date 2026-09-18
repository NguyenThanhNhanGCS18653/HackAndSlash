#pragma once

#include "CoreMinimal.h"
#include "SoberGameplayAbility.h"
#include "SoberHitReact.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class SOBER_API USoberHitReact : public USoberGameplayAbility
{
	GENERATED_BODY()
	
public:
    USoberHitReact();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HitReact")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HitReact")
    float KnockbackZOffset = 200.f; // lực hất lên nhẹ, giống video hack-and-slash chuẩn (khựng người + lùi lại)

private:
    UPROPERTY() TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

    UFUNCTION() void OnHitReactFinished();
};
