#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SoberAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SOBER_API USoberAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    USoberAttributeSet();

    // ---- Vital attributes ----
    UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, Stamina)

    UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_MaxStamina)
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, MaxStamina)

    // ---- Combat resource (sẵn cho mở rộng: mana/rage sau này) ----
    UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_Stamina_Regen)
    FGameplayAttributeData StaminaRegenRate;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, StaminaRegenRate)

    // ---- Offense/Defense (dùng cho damage calculation mở rộng sau) ----
    UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_AttackPower)
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, AttackPower)

    UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_Armor)
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, Armor)

    // ---- Meta attributes: KHÔNG replicate, chỉ dùng để "chuyển tiếp" damage/heal qua GE Execution ----
    UPROPERTY(BlueprintReadOnly, Category = "Meta")
    FGameplayAttributeData IncomingDamage;
    ATTRIBUTE_ACCESSORS(USoberAttributeSet, IncomingDamage)


    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

    UFUNCTION() void OnRep_Health(const FGameplayAttributeData& Old);
    UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& Old);
    UFUNCTION() void OnRep_Stamina(const FGameplayAttributeData& Old);
    UFUNCTION() void OnRep_MaxStamina(const FGameplayAttributeData& Old);
    UFUNCTION() void OnRep_Stamina_Regen(const FGameplayAttributeData& Old);
    UFUNCTION() void OnRep_AttackPower(const FGameplayAttributeData& Old);
    UFUNCTION() void OnRep_Armor(const FGameplayAttributeData& Old);

private:
    bool bOutOfHealth = false; // tránh gọi OnDeath nhiều lần
};
