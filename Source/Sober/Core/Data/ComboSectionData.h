#pragma once

#include "Engine/DataTable.h"
#include "GameplayEffect.h"
#include "ComboSectionData.generated.h"

USTRUCT(BlueprintType)
struct SOBER_API FComboSectionData : public FTableRowBase
{
    GENERATED_BODY()

    // Tên Section trong Montage (VD: "Attack1", "Attack2")
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    FName MontageSectionName;

    // Stamina cost riêng cho đòn này — đòn sau thường nặng hơn đòn trước
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    float StaminaCost = 10.f;

    // Damage effect riêng cho đòn này (có thể khác nhau: đòn cuối combo damage cao hơn)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    TSubclassOf<UGameplayEffect> DamageEffect;

    // Base damage truyền qua SetByCaller cho DamageEffect ở trên
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    float BaseDamage = 10.f;

    // Cho phép branching: đòn này có thể dẫn tới nhiều đòn tiếp theo tùy input (light/heavy)
    // Key: Input tag ("Input.Attack.Light"), Value: tên section tiếp theo
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    TMap<FGameplayTag, FName> NextSectionByInput;

    // Nếu true, đòn này có thể bị cancel sớm hơn bằng Dodge (mở rộng sau)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
    bool bCancelable = false;
};