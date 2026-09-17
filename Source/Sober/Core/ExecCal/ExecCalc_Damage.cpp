#include "ExecCalc_Damage.h"

#include "Sober/Core/SoberAttributeSet.h"
#include "Sober/Core/SoberGameplayTags.h"

struct FPoisonDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);

    FPoisonDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(USoberAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(USoberAttributeSet, AttackPower, Source, false);
    }
};

static const FPoisonDamageStatics& PoisonDamageStatics()
{
    static FPoisonDamageStatics Statics;
    return Statics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    RelevantAttributesToCapture.Add(PoisonDamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(PoisonDamageStatics().AttackPowerDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvalParams;
    EvalParams.SourceTags = SourceTags;
    EvalParams.TargetTags = TargetTags;

    float SourceAttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        PoisonDamageStatics().AttackPowerDef, EvalParams, SourceAttackPower);

    float TargetArmor = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        PoisonDamageStatics().ArmorDef, EvalParams, TargetArmor);

    // Base poison damage per tick truyền qua SetByCaller (config theo từng loại poison: yếu/mạnh)
    const float BasePoisonDamage = Spec.GetSetByCallerMagnitude(
        FSoberGameplayTags::Get().SetByCaller_Damage, false /*không warn nếu thiếu*/, 5.f /*default fallback*/);

    // Công thức: damage theo % AttackPower nguồn + base, giảm nhẹ theo Armor target
    const float RawDamage = BasePoisonDamage + (SourceAttackPower * 0.1f);
    const float MitigatedDamage = FMath::Max(RawDamage - (TargetArmor * 0.5f), 1.f); // tối thiểu 1 dmg

    // Output vào IncomingDamage (Meta Attribute) — đi qua đúng pipeline PostGameplayEffectExecute
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(
            USoberAttributeSet::GetIncomingDamageAttribute(),
            EGameplayModOp::Additive,
            MitigatedDamage));
}
