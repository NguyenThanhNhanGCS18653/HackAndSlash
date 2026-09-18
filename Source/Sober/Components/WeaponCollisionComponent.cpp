// WeaponCollisionComponent.cpp
#include "WeaponCollisionComponent.h"

#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"

#include "Sober/Core/SoberGameplayTags.h"

UWeaponCollisionComponent::UWeaponCollisionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UWeaponCollisionComponent::InitializeWeapon(USkeletalMeshComponent* InWeaponMesh,
    FName InTraceStartSocket, FName InTraceEndSocket)
{
    WeaponMesh = InWeaponMesh;
    TraceStartSocket = InTraceStartSocket;
    TraceEndSocket = InTraceEndSocket;
}

void UWeaponCollisionComponent::StartTrace()
{
    if (!WeaponMesh.IsValid()) return;

    bIsTracing = true;
    AlreadyHitActorsThisSwing.Empty(); // reset danh sách mỗi lần bắt đầu 1 nhát chém mới
    SetComponentTickEnabled(true);
}

void UWeaponCollisionComponent::StopTrace()
{
    bIsTracing = false;
    SetComponentTickEnabled(false);
}

void UWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsTracing)
    {
        PerformTrace();
    }
}

void UWeaponCollisionComponent::PerformTrace()
{
    if (!WeaponMesh.IsValid()) return;

    const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocket);
    const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocket);

    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());

    UKismetSystemLibrary::SphereTraceMultiForObjects(
        this, Start, End, TraceRadius,
        { UEngineTypes::ConvertToObjectType(TraceChannel) },
        false, ActorsToIgnore,
        bDebugDrawTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        HitResults, true);

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor || AlreadyHitActorsThisSwing.Contains(HitActor)) continue;

        // Chỉ tính là "trúng" nếu Actor có Ability System (tránh trace trúng tường, prop...)
        IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(HitActor);
        if (!ASCInterface || !ASCInterface->GetAbilitySystemComponent()) continue;

        AlreadyHitActorsThisSwing.Add(HitActor);

        const FVector TraceDirection = (End - Start).GetSafeNormal();
        ApplyHitEffects(HitActor, Hit.ImpactPoint, TraceDirection);
    }
}

void UWeaponCollisionComponent::ApplyHitEffects(AActor* HitActor, const FVector& HitLocation,
    const FVector& TraceDirection)
{
    FSoberGameplayTags GameplayTags = FSoberGameplayTags::Get();

    UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
    if (!SourceASC || !TargetASC) return;

    // ---- 1. Apply Damage Effect ----
    if (CurrentDamageEffect)
    {
        FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
        Context.AddSourceObject(GetOwner());
        Context.AddHitResult(FHitResult(HitActor, nullptr, HitLocation, -TraceDirection));

        FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(CurrentDamageEffect, 1.f, Context);
        if (Spec.IsValid())
        {
            Spec.Data->SetSetByCallerMagnitude(GameplayTags.SetByCaller_Damage, CurrentBaseDamage);
            SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
        }
    }

    // ---- 2. Gửi Gameplay Event kích hoạt Hit React trên Target ----
    // Payload mang theo hướng knockback (từ nguồn gây damage tới target) và lực đẩy
    FGameplayEventData EventData;
    EventData.EventTag = GameplayTags.Event_Combat_HitReact;
    EventData.Instigator = GetOwner();
    EventData.Target = HitActor;
    EventData.TargetData = FGameplayAbilityTargetDataHandle();
    EventData.EventMagnitude = CurrentKnockbackForce;

    // Nhúng hướng knockback vào ContextHandle để GA_HitReact đọc lại (xem cách lấy ở GA_HitReact bên dưới)
    FGameplayEffectContextHandle KnockbackContext = SourceASC->MakeEffectContext();
    KnockbackContext.AddOrigin(GetOwner()->GetActorLocation());
    EventData.ContextHandle = KnockbackContext;

    TargetASC->HandleGameplayEvent(GameplayTags.Event_Combat_HitReact, &EventData);
}