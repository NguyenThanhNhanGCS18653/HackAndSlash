#pragma once

#include "CoreMinimal.h"
#include "ComboAbilityBase.h"
#include "MeleeComboAbility.generated.h"


UCLASS()
class SOBER_API UMeleeComboAbility : public UComboAbilityBase
{
	GENERATED_BODY()
	
public:
	UMeleeComboAbility();

protected:
	virtual bool IsComboConditionMet() const override;
	virtual FGameplayTag GetComboTypeTag() const override;
};
