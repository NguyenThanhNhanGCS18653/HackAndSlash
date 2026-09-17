// GA_ComboAbilityBase.cpp
#include "ComboAbilityBase.h"
#include "TimerManager.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "Sober/Core/SoberGameplayTags.h"
#include "Sober/Core/SoberAttributeSet.h"
#include "Sober/Core/SoberAbilitySystemComponent.h"

FOnComboSectionChanged UComboAbilityBase::OnComboSectionChanged;

UComboAbilityBase::UComboAbilityBase()
{
    FSoberGameplayTags GameplayTag = FSoberGameplayTags::Get();
    AbilityTags.AddTag(GameplayTag.Ability_Type_Attack);
    ActivationOwnedTags.AddTag(GameplayTag.State_Attacking);

    // Không cho activate nếu đang Stunned hoặc Dead
    ActivationBlockedTags.AddTag(GameplayTag.State_Stunned);
    ActivationBlockedTags.AddTag(GameplayTag.State_Dead);
}

bool UComboAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags,
    FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }
    return IsComboConditionMet();
}

void UComboAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!ComboMontage || !ComboDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Thiếu ComboMontage hoặc ComboDataTable!"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Nếu Ability này đang re-trigger từ combo trước (đã có buffered input) -> tiếp tục section mới
    // Nếu là lần đầu activate -> bắt đầu từ FirstSectionName
    if (CurrentSectionName.IsNone() || !bComboWindowOpen)
    {
        CurrentSectionName = FirstSectionName;
    }

    PlayCurrentSection();
}

void UComboAbilityBase::PlayCurrentSection()
{
    FComboSectionData* SectionData = GetCurrentSectionData();
    if (!SectionData)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Không tìm thấy Section '%s' trong DataTable!"),
            *GetName(), *CurrentSectionName.ToString());
        EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfo(), true, true);
        return;
    }

    if (!TryApplyStaminaCost(*SectionData))
    {
        // Không đủ stamina -> huỷ ability, không chơi animation
        EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfo(), true, true);
        return;
    }

    bComboWindowOpen = false;
    bInputBufferedThisSection = false;
    BufferedInputTag = FGameplayTag();

    MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, NAME_None, ComboMontage, 1.f, CurrentSectionName);

    MontageTask->OnCompleted.AddDynamic(this, &UComboAbilityBase::OnMontageCompleted);
    MontageTask->OnInterrupted.AddDynamic(this, &UComboAbilityBase::OnMontageInterrupted);
    MontageTask->OnCancelled.AddDynamic(this, &UComboAbilityBase::OnMontageCancelled);
    MontageTask->OnBlendOut.AddDynamic(this, &UComboAbilityBase::OnMontageBlendOut);
    MontageTask->ReadyForActivation();

    SetupWindowListeners();

    ApplyDamageEffect(*SectionData);

    OnComboSectionChanged.Broadcast(GetAvatarActorFromActorInfo(), CurrentSectionName);
}

void UComboAbilityBase::SetupWindowListeners()
{
    FSoberGameplayTags GameplayTag = FSoberGameplayTags::Get();

    WindowOpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, GameplayTag.Event_Combo_WindowOpen, nullptr, false, false);
    WindowOpenTask->EventReceived.AddDynamic(this, &UComboAbilityBase::OnWindowOpenEvent);
    WindowOpenTask->ReadyForActivation();

    WindowCloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, GameplayTag.Event_Combo_WindowClose, nullptr, false, false);
    WindowCloseTask->EventReceived.AddDynamic(this, &UComboAbilityBase::OnWindowCloseEvent);
    WindowCloseTask->ReadyForActivation();
}

void UComboAbilityBase::OnWindowOpenEvent(FGameplayEventData Payload)
{
    FSoberGameplayTags GameplayTag = FSoberGameplayTags::Get();

    bComboWindowOpen = true;
    GetSoberASC()->AddLooseGameplayTag(GameplayTag.State_ComboWindowOpen);

    // Safety net: nếu vì lý do gì đó WindowClose không bắn (animation bị cắt...), tự reset sau X giây
    GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this,
        &UComboAbilityBase::OnComboResetTimeout, MaxComboResetDelay, false);
}

void UComboAbilityBase::OnWindowCloseEvent(FGameplayEventData Payload)
{
    FSoberGameplayTags GameplayTag = FSoberGameplayTags::Get();
    
    bComboWindowOpen = false;
    GetSoberASC()->RemoveLooseGameplayTag(GameplayTag.State_ComboWindowOpen);
    GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);

    // Nếu có input buffer trong lúc window mở -> chuyển sang section tiếp theo NGAY khi window đóng
    if (bInputBufferedThisSection)
    {
        FComboSectionData* CurrentData = GetCurrentSectionData();
        if (CurrentData)
        {
            const FName* NextSection = CurrentData->NextSectionByInput.Find(BufferedInputTag);
            if (NextSection)
            {
                CurrentSectionName = *NextSection;
                PlayCurrentSection(); // re-trigger ngay, không đợi montage cũ hoàn toàn kết thúc
                return;
            }
        }
    }
    // Không có input buffer hợp lệ -> để Montage tự chạy hết, OnMontageCompleted sẽ reset combo
}

void UComboAbilityBase::BufferComboInput(FGameplayTag Tag)
{
    // Chỉ nhận input nếu đang trong window mở — đây chính là "combo timing"
    if (bComboWindowOpen)
    {
        bInputBufferedThisSection = true;
        BufferedInputTag = Tag;
    }
    // Ngoài window: input bị bỏ qua (có thể mở rộng thêm "early buffer" nếu muốn dễ tính hơn cho người chơi)
}

FComboSectionData* UComboAbilityBase::GetCurrentSectionData() const
{
    if (!ComboDataTable) return nullptr;
    return ComboDataTable->FindRow<FComboSectionData>(CurrentSectionName, TEXT("ComboLookup"), false);
}


void UComboAbilityBase::ApplyDamageEffect(const FComboSectionData& SectionData)
{
    if (!SectionData.DamageEffect) return;

    // Lưu ý: đây là nơi CHỈ apply lên bản thân nếu cần buff/cost.
    // Damage lên ĐỊCH thực tế nên làm qua overlap/trace event riêng (Weapon Collision Component)
    // rồi gọi ApplyGameplayEffectSpecToTarget ở đó — không apply damage lên chính mình ở đây.
    // Phần này để placeholder cho hook mở rộng khi bạn làm Weapon Trace System.
}

void UComboAbilityBase::OnMontageCompleted()
{
    ResetCombo();
    EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfo(), true, false);
}

void UComboAbilityBase::OnMontageBlendOut()
{
    // BlendOut xảy ra trước Completed một chút — không end ability ở đây để tránh xung đột với re-trigger
}

void UComboAbilityBase::OnMontageInterrupted()
{
    ResetCombo();
    EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfo(), true, true);
}

void UComboAbilityBase::OnMontageCancelled()
{
    ResetCombo();
    EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfo(), true, true);
}

void UComboAbilityBase::OnComboResetTimeout()
{
    // Window mở quá lâu không đóng (edge case) -> ép reset để tránh combo bị "kẹt"
    ResetCombo();
}

void UComboAbilityBase::ResetCombo()
{
    FSoberGameplayTags GameplayTag = FSoberGameplayTags::Get();

    CurrentSectionName = NAME_None;
    bComboWindowOpen = false;
    bInputBufferedThisSection = false;
    BufferedInputTag = FGameplayTag();
    GetSoberASC()->RemoveLooseGameplayTag(GameplayTag.State_ComboWindowOpen);
    GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
}

bool UComboAbilityBase::TryApplyStaminaCost(const FComboSectionData& SectionData)
{
    if (!StaminaCostEffectClass) return true; // không config cost -> cho qua

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    const USoberAttributeSet* AttrSet = ASC->GetSet<USoberAttributeSet>();
    if (AttrSet && AttrSet->GetStamina() < SectionData.StaminaCost)
    {
        return false;
    }

    FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(StaminaCostEffectClass, GetAbilityLevel(), Context);
    if (Spec.IsValid())
    {
        Spec.Data->SetSetByCallerMagnitude(FSoberGameplayTags::Get().SetByCaller_StaminaCost, -SectionData.StaminaCost);
        ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
    }
    return true;
}

void UComboAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled)
{
    GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

