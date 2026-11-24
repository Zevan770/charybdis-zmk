set windows-shell := ["pwsh", "-Command"]

NICENANO_LABEL := "NICENANO"

download:
    trash ./firmware || true
    gh run download -n firmware -D firmware

flash-left:
    ./scripts/Copy-WithRetry.ps1 ./firmware/charybdis_left-nice_nano__zmk-zmk.uf2 {{ NICENANO_LABEL }}

flash-right:
    ./scripts/Copy-WithRetry.ps1 ./firmware/charybdis_right-nice_nano__zmk-zmk.uf2 {{ NICENANO_LABEL }}

push-wait:
    git push
    sleep 2
    gh run watch --compact

[default]
download-n-flash: push-wait download flash-left
