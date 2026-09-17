#include "MeleeComboAbility.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Sober/Core/SoberGameplayTags.h"

UMeleeComboAbility::UMeleeComboAbility()
{
	AbilityTags.AddTag(FSoberGameplayTags::Get().Ability_Combo_Ground);
	FirstSectionName = "Attack1";
}

bool UMeleeComboAbility::IsComboConditionMet() const
{
	// Ground combo chỉ activate khi KHÔNG falling (đối lập với Air combo)
	const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	return Character && Character->GetCharacterMovement() &&
		!Character->GetCharacterMovement()->IsFalling();
}

FGameplayTag UMeleeComboAbility::GetComboTypeTag() const
{
	return FSoberGameplayTags::Get().Ability_Combo_Ground;
}
