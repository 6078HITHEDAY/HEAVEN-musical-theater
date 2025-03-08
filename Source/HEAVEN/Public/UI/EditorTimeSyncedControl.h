#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorTimeSyncedControl.generated.h"

// 编辑器时间同步控件（抽象基类，需子类实现）
UCLASS(Abstract, Blueprintable)
class HEAVEN_API UEditorTimeSyncedControl : public UUserWidget
{
	GENERATED_BODY()

public:
	// 时间轴精度（单位：秒，控制时间同步更新频率）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimeSync")
	float TimelineResolution = 0.1f;

	// 同步音频时间轴（CurrentPlaybackTime：当前音频播放时间，单位：秒）
	UFUNCTION(BlueprintCallable, Category="TimeSync")
	void SyncWithAudioTimeline(float CurrentPlaybackTime);

	// 3D轨迹可视化更新事件（蓝图可实现，TrajectoryPoints：运动轨迹点数组） 
	UFUNCTION(BlueprintImplementableEvent, Category="Visualization")
	void Update3DTrajectoryVisualization(const TArray<FVector>& TrajectoryPoints);
};