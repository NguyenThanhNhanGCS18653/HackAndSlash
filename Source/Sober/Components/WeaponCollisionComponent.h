#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffect.h"
#include "WeaponCollisionComponent.generated.h"

class USkeletalMeshComponent;
class UAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOBER_API UWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UWeaponCollisionComponent();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartTrace();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopTrace();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void InitializeWeapon(USkeletalMeshComponent* InWeaponMesh, FName InTraceStartSocket, FName InTraceEndSocket);

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    TSubclassOf<UGameplayEffect> CurrentDamageEffect;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    float CurrentBaseDamage = 10.f;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    float CurrentKnockbackForce = 400.f;

protected:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float TraceRadius = 15.f;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Pawn;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    bool bDebugDrawTrace = false;

private:
    TWeakObjectPtr<USkeletalMeshComponent> WeaponMesh;
    FName TraceStartSocket;
    FName TraceEndSocket;

    bool bIsTracing = false;
    TSet<TWeakObjectPtr<AActor>> AlreadyHitActorsThisSwing; // tránh trúng 1 actor nhiều lần trong cùng 1 nhát chém

    void PerformTrace();
    void ApplyHitEffects(AActor* HitActor, const FVector& HitLocation, const FVector& TraceDirection);
};
