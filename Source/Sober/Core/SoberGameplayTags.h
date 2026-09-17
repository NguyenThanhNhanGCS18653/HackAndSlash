#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/* SoberGameplayTags

	Singleton containing Native Gameplay Tags.
*/

struct FSoberGameplayTags
{
public:
	static const FSoberGameplayTags& Get()
	{
		if (!SoberGameplayTags.bInitialized)
		{
			InitializeNativeGameplayTags();
		}
		return SoberGameplayTags;
	}
	static void InitializeNativeGameplayTags();

#pragma region Attributes / meta
	FGameplayTag Attribute_Health;
	FGameplayTag Attribute_Stamina;
#pragma endregion

#pragma region Ability
	FGameplayTag Ability_Type_Attack;
	FGameplayTag Ability_Combo_Ground;
	FGameplayTag Ability_Combo_Air;
#pragma endregion

#pragma region State
	FGameplayTag State_Dead;
	FGameplayTag State_Stunned;
	FGameplayTag State_Attacking;
	FGameplayTag State_ComboWindowOpen;
#pragma endregion

#pragma region Debuff
	FGameplayTag Debuff_Poison;
#pragma endregion

#pragma region Damage Types Tags
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Poison;
#pragma endregion

#pragma region Event Tags
	FGameplayTag Event_Combo_WindowOpen;
	FGameplayTag Event_Combo_WindowClose;
	FGameplayTag Event_Combo_Hit;
#pragma endregion

#pragma region Setbycaller GE
	FGameplayTag SetByCaller_Health;
	FGameplayTag SetByCaller_MaxHealth;
	FGameplayTag SetByCaller_Stamina;
	FGameplayTag SetByCaller_MaxStamina;
	FGameplayTag SetByCaller_AttackPower;
	FGameplayTag SetByCaller_Armor;
	FGameplayTag SetByCaller_Damage;
	FGameplayTag SetByCaller_StaminaCost;
#pragma endregion

#pragma region Input
	FGameplayTag Input_Move;
	FGameplayTag Input_Look;
	FGameplayTag Input_Jump;

	FGameplayTag Input_Attack_Light;
	FGameplayTag Input_Attack_Heavy;
#pragma endregion
protected:
	bool bInitialized = false;
private:
	static FSoberGameplayTags SoberGameplayTags;
};