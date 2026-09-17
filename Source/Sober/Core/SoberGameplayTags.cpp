#include "SoberGameplayTags.h"
#include "GameplayTagsManager.h"

FSoberGameplayTags FSoberGameplayTags::SoberGameplayTags;

void FSoberGameplayTags::InitializeNativeGameplayTags()
{
#pragma region Attributes / meta
	SoberGameplayTags.Attribute_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Health"),
		FString("Health Attribute Tag")
	);

	SoberGameplayTags.Attribute_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Stamina"),
		FString("Stamina Attribute Tag")
	);
#pragma endregion

#pragma region Ability
	SoberGameplayTags.Ability_Type_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Type.Attack"),
		FString("Ability_Type_Attack Tag")
	);

	SoberGameplayTags.Ability_Combo_Ground = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Combo.Ground"),
		FString("Ability_Combo_Ground Tag")
	);

	SoberGameplayTags.Ability_Combo_Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Combo.Air"),
		FString("Ability_Combo_Air Tag")
	);
#pragma endregion

#pragma region State
	SoberGameplayTags.State_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Dead"),
		FString("State Dead Tag")
	);

	SoberGameplayTags.State_Stunned = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Stunned"),
		FString("State Stunned Tag")
	);

	SoberGameplayTags.State_Attacking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Attacking"),
		FString("State_Attacking Tag")
	);

	SoberGameplayTags.State_ComboWindowOpen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.ComboWindowOpen"),
		FString("State_ComboWindowOpen Tag")
	);
#pragma endregion

#pragma region Debuff
	SoberGameplayTags.Debuff_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Poison"),
		FString("Debuff Poison Tag")
	);
#pragma endregion

#pragma region Damage Types Tags
	SoberGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type Tag")
	);

	SoberGameplayTags.Damage_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Poison"),
		FString("Poison Damage Type Tag")
	);
#pragma endregion

#pragma region Event Tags
	SoberGameplayTags.Event_Combo_WindowOpen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Combo.WindowOpen"),
		FString("Event_Combo_WindowOpen Tag")
	);

	SoberGameplayTags.Event_Combo_WindowClose = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Combo.WindowClose"),
		FString("Event_Combo_WindowClose Tag")
	);

	SoberGameplayTags.Event_Combo_Hit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Combo.Hit"),
		FString("Event_Combo_Hit Tag")
	);
#pragma endregion

#pragma region Setbycaller GE
	SoberGameplayTags.SetByCaller_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.Armor"),
		FString("SetByCaller_Armor Tag")
	);

	SoberGameplayTags.SetByCaller_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.AttackPower"),
		FString("SetByCaller_AttackPower Tag")
	);

	SoberGameplayTags.SetByCaller_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.Damage"),
		FString("SetByCaller_Damage Tag")
	);

	SoberGameplayTags.SetByCaller_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.Health"),
		FString("SetByCaller_Health Tag")
	);

	SoberGameplayTags.SetByCaller_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.MaxHealth"),
		FString("SetByCaller_MaxHealth Tag")
	);

	SoberGameplayTags.SetByCaller_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.Stamina"),
		FString("SetByCaller_Stamina Tag")
	);

	SoberGameplayTags.SetByCaller_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.MaxStamina"),
		FString("SetByCaller_MaxStamina Tag")
	);

	SoberGameplayTags.SetByCaller_StaminaCost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SetByCaller.StaminaCost"),
		FString("SetByCaller_StaminaCost Tag")
	);
#pragma endregion

#pragma region Input
	SoberGameplayTags.Input_Move = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Move"),
		FString("Input_Move Tag")
	);

	SoberGameplayTags.Input_Look = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Look"),
		FString("Input_Look Tag")
	);

	SoberGameplayTags.Input_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Jump"),
		FString("Input_Jump Tag")
	);

	SoberGameplayTags.Input_Attack_Light = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Attack.Light"),
		FString("Input_Attack_Light Tag")
	);

	SoberGameplayTags.Input_Attack_Heavy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Attack.Heavy"),
		FString("Input_Attack_Heavy Tag")
	);
#pragma endregion

	SoberGameplayTags.bInitialized = true;
}