param (
  [Parameter(Mandatory = $true)][string]$Path,
  [Parameter(Mandatory = $true)][string]$VolumeLabel,
  [int]$IntervalSeconds = 5
)

function Get-DriveLetterByLabel
{
  param([string]$Label)
  $volume = Get-Volume -ErrorAction SilentlyContinue |
    Where-Object { $_.FileSystemLabel -eq $Label } |
    Select-Object -First 1

  if ($volume -and $volume.DriveLetter)
  {
    return "$($volume.DriveLetter):\"
  }
  return $null
}

function Invoke-UntilSuccess
{
  param(
    [scriptblock]$ScriptBlock,
    [int]$IntervalSeconds = 5
  )
  while ($true)
  {
    $result = & $ScriptBlock
    if ($result)
    {
      Write-Host "ok"
      return
    }
    Write-Host "."
    Start-Sleep -Seconds $IntervalSeconds
  }
}

function Copy-WithRetryCore
{
  $Target = Get-DriveLetterByLabel -Label $VolumeLabel
  if (-not $Target)
  { return $false 
  }

  if (!(Test-Path $Path) -or !(Test-Path $Target))
  {
    return $false
  }
  try
  {
    Copy-Item $Path -Destination $Target -Force
    return $true
  } catch
  {
    return $false
  }
}

Invoke-UntilSuccess { Copy-WithRetryCore } -IntervalSeconds $IntervalSeconds
