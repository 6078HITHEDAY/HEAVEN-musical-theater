using UnrealEngine;

namespace HEAVEN.Tools
{
    public static class AudioTool
    {
        public static void SyncWithTimeline(float currentTime)
        {
            // 调用Unreal本地方法同步音频时间轴
            NativeMethodLibrary.SyncAudioTimeline(currentTime);
        }

        public static void GenerateVisualizationData(FVector[] trajectoryPoints)
        {
            // 将轨迹数据发送到Unreal Niagara粒子系统
            using var paramCollection = new FNiagaraParameterCollection();
            paramCollection.SetVectorArray("TrajectoryPoints", trajectoryPoints);
            NiagaraSystemManager.UpdateParameters(paramCollection);
        }
    }
}