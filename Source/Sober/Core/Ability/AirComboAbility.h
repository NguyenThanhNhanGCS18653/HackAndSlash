#pragma once

#include "CoreMinimal.h"
#include "ComboAbilityBase.h"
#include "AirComboAbility.generated.h"


UCLASS()
class SOBER_API UAirComboAbility : public UComboAbilityBase
{
	GENERATED_BODY()
	
public:
    UAirComboAbility();

protected:
    virtual bool IsComboConditionMet() const override;
    virtual FGameplayTag GetComboTypeTag() const override;

    // Air combo có thể cần thêm: giữ nhân vật lơ lửng trong lúc combo (launch/hover)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AirCombo")
    bool bApplyHoverDuringCombo = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AirCombo")
    float HoverGravityScale = 0.2f;

private:
    float OriginalGravityScale = 1.f;
};
