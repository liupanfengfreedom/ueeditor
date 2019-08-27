// Fill out your copyright notice in the Description page of Project Settings.

#include "ControlCenter.h"
#include "TcpClientv.h"
#include "EditorFunctionLibrary.h"
#include "JsonUtilities.h"
#include "Json.h"
#if WITH_EDITORONLY_DATA
#include "Editor/UnrealEd/Public/Editor.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#endif

class TcpClientv* UControlCenter::mtcp = nullptr;

UControlCenter::UControlCenter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{//UObject(const FObjectInitializer& ObjectInitializer)

}
bool UControlCenter::ConnectServer()
{
	if (mtcp == nullptr)
	{
		mtcp = new TcpClientv();
		isconnected = mtcp->Connecttoserver(192, 168, 1, 240, 8003);
		mtcp->OnTcpClientReceiveddata.AddDynamic(this, &UControlCenter::OnTcpResponse);
		if (isconnected)
		{

		}
#if WITH_EDITORONLY_DATA
		FEditorDelegates::EditorModeExit.AddStatic(&UControlCenter::clientexit);
#endif
		//UMyBlueprintFunctionLibrary::AddfunctiontoOnGameexitArray(&UTcpCommunicatorv1::clientexit);
		check(world);
		world->GetTimerManager().SetTimer(th, this, &UControlCenter::thwork, 0.5, true, 1);
	}
	return isconnected;
}
void UControlCenter::SplitString(FString str, TArray<FString>&strarray)
{//////?xxxxx?xxxxx?xxxxx...
	FString str1;
	FString str2;
	str.Split("?", &str1, &str2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	strarray.Insert(str2, 0);
	if (!str1.IsEmpty())
	{
		SplitString(str1, strarray);
	}
}
void UControlCenter::clientexit(FEdMode * mm)
{
	UControlCenter::mtcp->exitthread = true;
}
void UControlCenter::OnTcpResponse(const TArray<uint8>& p, const FString & str)
{
	FMessagePackage mp;
	FJsonObjectConverter::JsonObjectStringToUStruct<FMessagePackage>(str, &mp, 0, 0);
	if (mp.MT == MessageType::ASSIGNID)
	{
		command = mp;
	}
}
void UControlCenter::thwork()
{
	if (command.MT == MessageType::ASSIGNID)
	{
		command.MT = MessageType::EMPTY;
		FString message = command.PayLoad;
		TArray<FString> payloadmessage;
		SplitString(message, payloadmessage);
		//FName path = "/Game/VehicleBP/Maps/VehicleExampleMap";
		//FName path = FName(*payloadmessage[0]);
		FName path = "";
		findmeshpath(path);
		int32 MyNewInt = FCString::Atoi(*payloadmessage[1]);
		MyNewInt = 1;
		if (path.IsEqual(""))
		{

		}
		else
		{
			UEditorFunctionLibrary::AssignID(path, MyNewInt);
			FMessagePackage mp;
			mp.MT = MessageType::ASSIGNOK;
			mp.PayLoad = path.ToString();
			FString outstring;
			FJsonObjectConverter::UStructToJsonObjectString<FMessagePackage>(mp, outstring);
			mtcp->Send(outstring);
		}


	}

}
void UControlCenter::findmeshpath(FName & path)
{
	FAssetRegistryModule* AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> AssetData;
	FARFilter Filter = FARFilter();

	// Classes
	Filter.ClassNames.Add(FName("SkeletalMesh"));
	Filter.ClassNames.Add(FName("StaticMesh"));

	FString Directory = "";
	FString DirPrefix = Directory.IsEmpty() ? "/Game" : "/Game/";
	Filter.PackagePaths.Add(FName(*(DirPrefix + Directory)));

	// Flags
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	AssetRegistryModule->Get().GetAssets(Filter, AssetData);
	if (AssetData.Num() > 0)
	{
		path = AssetData[0].PackageName;
	}
}

