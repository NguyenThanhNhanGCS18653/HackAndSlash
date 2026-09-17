#include "SoberAssetManager.h"
#include "Core/SoberGameplayTags.h"

USoberAssetManager& USoberAssetManager::Get()
{
	check(GEngine);

	USoberAssetManager* SoberAssetManager = Cast<USoberAssetManager>(GEngine->AssetManager);
	return *SoberAssetManager;
}

void USoberAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FSoberGameplayTags::InitializeNativeGameplayTags();
}
