#include "AirComboAbility.h"
#include "GameFramework/Character.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Sober/Core/SoberGameplayTags.h"

UAirComboAbility::UAirComboAbility()
{
	AbilityTags.AddTag(FSoberGameplayTags::Get().Ability_Combo_Air);
	FirstSectionName = "AirAttack1";
	MaxComboResetDelay = 1.0f; // air combo nên có window ngắn hơn ground
}

bool UAirComboAbility::IsComboConditionMet() const
{
	const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	return Character && Character->GetCharacterMovement() &&
		Character->GetCharacterMovement()->IsFalling();
}

FGameplayTag UAirComboAbility::GetComboTypeTag() const
{
	return FSoberGameplayTags::Get().Ability_Combo_Air;
}
