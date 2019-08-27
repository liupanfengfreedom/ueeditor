// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "UObject/NoExportTypes.h"
#include "ControlCenter.generated.h"
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class MessageType : uint8
{
	EMPTY,
	ASSIGNID,
	ASSIGNOK,
};
USTRUCT()
struct FMessagePackage {
	GENERATED_BODY()
		UPROPERTY()
		MessageType MT;
	UPROPERTY()
		FString PayLoad;
	FMessagePackage()
	{
		MT = MessageType::EMPTY;
		PayLoad = "";
	}
	FMessagePackage(MessageType mMT, FString mPayLoad)
	{
		MT = mMT;
		PayLoad = mPayLoad;
	}
};
/**
 * 
 */
UCLASS()
class EDITORASSITANT_API UControlCenter : public UObject
{
	GENERATED_BODY()
		UControlCenter(const FObjectInitializer& ObjectInitializer);
	static class TcpClientv * mtcp;
	static void clientexit(class FEdMode *);
	bool isconnected = false;
	FTimerHandle th;
	void thwork();
	FMessagePackage command;
	UFUNCTION()
	void OnTcpResponse(const TArray<uint8>&p, const FString & str);
	void findmeshpath(FName & path);
public:
	bool ConnectServer();
	class UWorld* world;
	void SplitString(FString str, TArray<FString>&strarray);

};
