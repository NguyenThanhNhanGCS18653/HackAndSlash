#include "SoberAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

#include "Sober/Core/SoberGameplayTags.h"

USoberAttributeSet::USoberAttributeSet()
{
    /*InitHealth(100.f);
    InitMaxHealth(100.f);
    InitStamina(100.f);
    InitMaxStamina(100.f);
    InitStaminaRegenRate(10.f);
    InitAttackPower(10.f);
    InitArmor(0.f);*/
}

void USoberAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(USoberAttributeSet, Armor, COND_None, REPNOTIFY_Always);
}

void USoberAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    AActor* TargetActor = Data.Target.GetAvatarActor();

    // ---- Damage pipeline ----
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        const float LocalDamage = GetIncomingDamage();
        SetIncomingDamage(0.f); // reset meta attribute ngay

        if (LocalDamage > 0.f)
        {
            const float OldHealth = GetHealth();
            SetHealth(FMath::Clamp(OldHealth - LocalDamage, 0.f, GetMaxHealth()));

            if (GetHealth() <= 0.f && !bOutOfHealth)
            {
                bOutOfHealth = true;
            }
        }
        return;
    }
}

// OnRep functions — GAMEPLAYATTRIBUTE_REPNOTIFY xử lý prediction key đúng chuẩn
void USoberAttributeSet::OnRep_Health(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, Health, Old);
}
void USoberAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, MaxHealth, Old);
}

void USoberAttributeSet::OnRep_Stamina(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, Stamina, Old);
}
void USoberAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, MaxStamina, Old);
}

void USoberAttributeSet::OnRep_Stamina_Regen(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, StaminaRegenRate, Old);
}

void USoberAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, AttackPower, Old);
}

void USoberAttributeSet::OnRep_Armor(const FGameplayAttributeData& Old)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoberAttributeSet, Armor, Old);
}