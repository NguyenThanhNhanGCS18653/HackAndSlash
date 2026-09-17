#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "SoberAbilitySystemComponent.generated.h"

UCLASS()
class SOBER_API USoberAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    // Gọi 1 lần khi Character init xong — bind toàn bộ GameplayTag event tập trung ở đây
    void SetupTagListeners();

    // Wrapper tiện dùng ở BaseCharacter/Ability thay vì gọi thẳng ASC raw API
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool TryActivateAbilityByTag(FGameplayTag AbilityTag);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool HasMatchingTagSafe(FGameplayTag Tag) const;

    // Gọi từ PlayerCharacter khi Input Action Started/Triggered
    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);

protected:
    //virtual void AbilityActorInfoSet();

private:
    void OnStunTagChanged(const FGameplayTag Tag, int32 NewCount);
    void OnDeadTagChanged(const FGameplayTag Tag, int32 NewCount);
};