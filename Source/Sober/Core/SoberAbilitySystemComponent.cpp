#include "SoberAbilitySystemComponent.h"

#include "SoberGameplayTags.h"

#include "Ability/ComboAbilityBase.h"

void USoberAbilitySystemComponent::SetupTagListeners()
{
	const FSoberGameplayTags& GameplayTags = FSoberGameplayTags::Get();

	RegisterGameplayTagEvent(GameplayTags.State_Stunned, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &USoberAbilitySystemComponent::OnStunTagChanged);

	RegisterGameplayTagEvent(GameplayTags.State_Dead, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &USoberAbilitySystemComponent::OnDeadTagChanged);
}

void USoberAbilitySystemComponent::OnStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		// Cancel toàn bộ ability đang chạy có tag "Ability.Type.Action" khi bị stun
		FGameplayTagContainer CancelTags;
		CancelTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Type.Action")));
		CancelAbilities(&CancelTags);
	}
}

void USoberAbilitySystemComponent::OnDeadTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		CancelAllAbilities();
	}
}

bool USoberAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTag)
{
	FGameplayTagContainer Container;
	Container.AddTag(AbilityTag);
	return TryActivateAbilitiesByTag(Container);
}

bool USoberAbilitySystemComponent::HasMatchingTagSafe(FGameplayTag Tag) const
{
	return Tag.IsValid() && HasMatchingGameplayTag(Tag);
}

void USoberAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (!InputTag.IsValid()) return;

    for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
    {
        const USoberGameplayAbility* AbilityCDO = Cast<USoberGameplayAbility>(Spec.Ability);
        if (!AbilityCDO || AbilityCDO->GetInputTag() != InputTag)
        {
            continue;
        }

        if (Spec.IsActive())
        {
            // Ability đang chạy (VD: combo đang giữa chừng) -> buffer input thay vì activate lại
            TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
            for (UGameplayAbility* ActiveInstance : Instances)
            {
                if (UComboAbilityBase* ComboAbility = Cast<UComboAbilityBase>(ActiveInstance))
                {
                    ComboAbility->BufferComboInput(InputTag);
                }
            }
        }
        else
        {
            // Chưa active -> thử activate bình thường (sẽ tự check CanActivateAbility, bao gồm IsComboConditionMet)
            TryActivateAbility(Spec.Handle);
        }
    }
}

void USoberAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!InputTag.IsValid()) return;

    // Chỗ này để trống cho ability dạng "hold" sau này (VD: charge attack, block giữ phím)
    // Pattern: tìm Spec theo InputTag tương tự trên, gọi 1 hàm ảo OnInputReleased() trên Ability nếu cần
}
