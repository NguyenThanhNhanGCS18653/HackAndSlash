#pragma once

#include "CoreMinimal.h"
#include "SoberGameplayAbility.h"
#include "Sober/Core/Data/ComboSectionData.h"
#include "ComboAbilityBase.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnComboSectionChanged, AActor* /*InstigatorActor*/, FName /*SectionName*/);

UCLASS(Abstract)
class SOBER_API UComboAbilityBase : public USoberGameplayAbility
{
	GENERATED_BODY()
	
public:
    UComboAbilityBase();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility, bool bWasCancelled) override;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags,
        const FGameplayTagContainer* TargetTags,
        FGameplayTagContainer* OptionalRelevantTags) const override;

    static FOnComboSectionChanged OnComboSectionChanged;
protected:
    // ---- Config chung — set trong BP subclass (BP_GA_GroundCombo / BP_GA_AirCombo) ----
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    UAnimMontage* ComboMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    UDataTable* ComboDataTable; // Row = FComboSectionData

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    FName FirstSectionName = "Attack1";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    TSubclassOf<UGameplayEffect> StaminaCostEffectClass;

    // Thời gian tối đa chờ input sau khi WindowOpen trước khi tự reset combo (an toàn nếu WindowClose bị miss)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    float MaxComboResetDelay = 1.5f;

    // ---- Runtime state ----
    FName CurrentSectionName;
    bool bComboWindowOpen = false;
    bool bInputBufferedThisSection = false;
    FGameplayTag BufferedInputTag;
    FTimerHandle ComboResetTimerHandle;

    UPROPERTY() TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
    UPROPERTY() TObjectPtr<UAbilityTask_WaitGameplayEvent> WindowOpenTask;
    UPROPERTY() TObjectPtr<UAbilityTask_WaitGameplayEvent> WindowCloseTask;

    // ---- Virtual hooks — subclass Ground/Air override để khác biệt điều kiện ----
    virtual bool IsComboConditionMet() const { return true; }
    virtual FGameplayTag GetComboTypeTag() const PURE_VIRTUAL(UGA_ComboAbilityBase::GetComboTypeTag, return FGameplayTag(););

    // ---- Core flow ----
    void PlayCurrentSection();
    void SetupWindowListeners();
    bool TryApplyStaminaCost(const FComboSectionData & SectionData);
    void ApplyDamageEffect(const FComboSectionData & SectionData);
    FComboSectionData* GetCurrentSectionData() const;
    void ResetCombo();

    // ---- Callback ----
    UFUNCTION() void OnMontageCompleted();
    UFUNCTION() void OnMontageInterrupted();
    UFUNCTION() void OnMontageCancelled();
    UFUNCTION() void OnMontageBlendOut();
    UFUNCTION() void OnWindowOpenEvent(FGameplayEventData Payload);
    UFUNCTION() void OnWindowCloseEvent(FGameplayEventData Payload);
    UFUNCTION() void OnComboResetTimeout();

public:
    // Gọi từ PlayerController/Character khi người chơi bấm input trong lúc Ability đang chạy
    UFUNCTION(BlueprintCallable, Category = "Combo")
    void BufferComboInput(FGameplayTag Tag);
};
