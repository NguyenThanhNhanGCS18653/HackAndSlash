#include "OverlayWidgetController.h"

#include "Sober/Core/SoberAttributeSet.h"
#include "Sober/Core/Ability/ComboAbilityBase.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    if (!SoberAttributeSet) return;

    OnHealthChanged.Broadcast(SoberAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(SoberAttributeSet->GetMaxHealth());
    OnStaminaChanged.Broadcast(SoberAttributeSet->GetStamina());
    OnMaxStaminaChanged.Broadcast(SoberAttributeSet->GetMaxStamina());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    if (!SoberAttributeSet) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoberAttributeSet->GetHealthAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoberAttributeSet->GetMaxHealthAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoberAttributeSet->GetStaminaAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoberAttributeSet->GetMaxStaminaAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);

    UComboAbilityBase::OnComboSectionChanged.AddUObject(this, &UOverlayWidgetController::ComboSectionUpdated);
}

void UOverlayWidgetController::ComboSectionUpdated(AActor* InstigatorActor, FName SectionName)
{
	//if (InstigatorActor != OwningCharacter) return;

	CachedComboCount = SectionName.IsNone() ? 0 : CachedComboCount + 1;
	OnComboSectionChanged.Broadcast(CachedComboCount, SectionName);
}
