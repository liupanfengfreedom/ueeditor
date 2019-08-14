// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorFunctionLibrary.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"
void UEditorFunctionLibrary::AssignID(FName targetname, int32 num)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TSet<FName>AllDependencies;
	TArray<UPackage*>packagetosave;
	AllDependencies.Add(targetname);
	RecursiveGetDependencies(targetname, AllDependencies);
	for (auto DependsIt = AllDependencies.CreateConstIterator(); DependsIt; ++DependsIt)
	{
		TArray<FAssetData>AssetDataList;
		AssetRegistryModule.Get().GetAssetsByPackageName(*DependsIt, AssetDataList);
		UPackage*mp = AssetDataList[0].GetPackage();
		TArray<int32> ar1;
		ar1.AddUnique(num);
		mp->SetChunkIDs(ar1);
		mp->SetDirtyFlag(true);
		packagetosave.Add(mp);
	}
	FEditorFileUtils::PromptForCheckoutAndSave(packagetosave,false,false);
}
void UEditorFunctionLibrary::RecursiveGetDependencies(const FName& PackageName, TSet<FName>& AllDependencies)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FName> Dependencies;
	AssetRegistryModule.Get().GetDependencies(PackageName, Dependencies);

	for (auto DependsIt = Dependencies.CreateConstIterator(); DependsIt; ++DependsIt)
	{
		if (!AllDependencies.Contains(*DependsIt))
		{
			// @todo Make this skip all packages whose root is different than the source package list root. For now we just skip engine content.
			const bool bIsEnginePackage = (*DependsIt).ToString().StartsWith(TEXT("/Engine"));
			const bool bIsScriptPackage = (*DependsIt).ToString().StartsWith(TEXT("/Script"));
			if (!bIsEnginePackage && !bIsScriptPackage)
			{
				AllDependencies.Add(*DependsIt);
				RecursiveGetDependencies(*DependsIt, AllDependencies);
			}
		}
	}
}