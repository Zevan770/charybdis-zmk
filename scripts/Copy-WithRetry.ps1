param (
  [Parameter(Mandatory = $true)][string]$Path,
  [Parameter(Mandatory = $true)][string]$Target,
  [int]$IntervalSeconds = 5
)

function Invoke-UntilSuccess {
  param(
    [scriptblock]$ScriptBlock,
    [int]$IntervalSeconds = 5
  )
  while ($true) {
    $result = & $ScriptBlock
    if ($result) {
      Write-Host "ok"
      return
    }
    Write-Host "."
    Start-Sleep -Seconds $IntervalSeconds
  }
}

function Copy-WithRetryCore {
  if (!(Test-Path $Path) -or !(Test-Path (Split-Path $Target -Parent))) {
    return $false
  }
  try {
    Copy-Item $Path -Destination $Target -Force
    return $true
  } catch {
    return $false
  }
}

Invoke-UntilSuccess { Copy-WithRetryCore } -IntervalSeconds $IntervalSeconds
