set windows-shell := ["pwsh", "-Command"]

download:
    trash ./firmware || true
    gh run download -n firmware -D firmware

flash-left:
    ./scripts/Copy-WithRetry.ps1 ./firmware/charybdis_left-nice_nano-zmk.uf2 D:/

flash-right:
    ./scripts/Copy-WithRetry.ps1 ./firmware/charybdis_right-nice_nano-zmk.uf2 D:/

[default]
download-n-flash: download flash-left
