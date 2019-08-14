// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EDITORASSITANT_API UEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void AssignID(FName name, int32 num);
private:
	static void RecursiveGetDependencies(const FName& PackageName, TSet<FName>& AllDependencies);
};
