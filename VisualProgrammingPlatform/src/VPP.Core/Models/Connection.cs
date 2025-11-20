namespace VPP.Core.Models;

public class Connection
{
    public string Id { get; } = Guid.NewGuid().ToString();
    public string SourceNodeId { get; init; } = "";
    public string SourcePortId { get; init; } = "";
    public string TargetNodeId { get; init; } = "";
    public string TargetPortId { get; init; } = "";
}
