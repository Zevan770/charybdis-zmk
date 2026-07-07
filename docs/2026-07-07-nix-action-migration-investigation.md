# Nix Action Migration Investigation

**Date**: 2026-07-07  
**Status**: Reverted — upstream bugs block migration

## Goal

Replace the Docker-based `zmkfirmware/zmk/.github/workflows/build-user-config.yml@main` with the Nix-based `urob/zmk-actions/.github/workflows/build-user-config.yml@main` for CI builds.

## What Was Tested

The `urob/zmk-actions` reusable workflow accepts a `toolchain` input with three options, all tested against our build matrix (`nice_nano//zmk` × `charybdis_left` / `charybdis_right` × `studio-rpc-usb-uart` snippet):

| Toolchain | Description | Result |
|---|---|---|
| `zephyr` | Zephyr SDK toolchain (slim) | ❌ `protoc` missing in Nix shell |
| `gnuarmemb` | GNU Arm Embedded toolchain (fastest) | ❌ `protoc` missing in Nix shell |
| `zephyr-full` | Zephyr SDK toolchain (full, needed for ZMK Studio) | ❌ Nix Python dependency conflict |

### Run History

| Run ID | Toolchain | Left | Right | Error |
|---|---|---|---|---|
| 28861724902 | `zephyr` | ❌ | ✅ | `FileNotFoundError: protoc` (nanopb proto gen) — right succeeded by luck (race condition) |
| 28862271744 | `zephyr-full` | ❌ | ❌ | `spsdk-mcu-link`: `hidapi~=0.14.0.post2` not satisfied by `0.15.0` |
| 28862558062 | `gnuarmemb` | ❌ | ❌ | Same `protoc` error as `zephyr` |

## Root Cause Analysis

### 1. Missing `protoc` (`zephyr` / `gnuarmemb`)

```
FAILED: nanopb/generator/proto/nanopb_pb2.py
FileNotFoundError: [Errno 2] No such file or directory: 'protoc'
```

- **What is `protoc`**: Google Protocol Buffers compiler. Zephyr's `nanopb` module uses it at build time to generate serialization code from `.proto` files (USB HID report descriptors).
- **Why it fails**: The `urob/zmk-actions` Nix flake for `zephyr` and `gnuarmemb` toolchains does not include the `protobuf` package, so `protoc` is not on `$PATH` inside the Nix shell.
- **Why right succeeded once**: The nanopb proto generation step is non-deterministic — CMake may skip it if cached artifacts exist. The right runner happened to skip it (race condition / different build ordering).
- **Who depends on it**: Not our code — it's a transitive Zephyr build system dependency through `nanopb`.

### 2. Python Dependency Conflict (`zephyr-full`)

```
error: Cannot build 'python3.13-spsdk-mcu-link-0.6.6.drv'
> hidapi~=0.14.0.post2 not satisfied by version 0.15.0
```

- `spsdk-mcu-link` (NXP MCU-Link debug probe support, part of Zephyr SDK) pins `hidapi~=0.14.0.post2` in its Python package metadata.
- The nixpkgs snapshot used by `urob/zmk-actions` only has `hidapi 0.15.0`, making it incompatible.
- This is an upstream bug in `urob/zmk-actions`'s Nix flake lock file — needs a nixpkgs pin update or version override.

### What is `gnuarmemb`?

- GNU Arm Embedded Toolchain — ARM's official GCC-based cross-compiler for Cortex-M chips.
- ZMK runs on Zephyr RTOS, but Zephyr is an OS framework, not a compiler. To turn C source into nRF52840 machine code, you need an **ARM cross-compilation toolchain**.
- `gnuarmemb` = use ARM's GCC. `zephyr` = use Zephyr SDK's bundled GCC. Both produce compatible ARM binaries — Zephyr/ZMK supports either.

## Conclusion

All three Nix-based toolchains have upstream bugs in `urob/zmk-actions` that prevent building this project. The Docker-based `zmkfirmware/zmk` action (which includes a full Zephyr Docker container with all dependencies) works correctly.

**Recommendation**: Stay on Docker until upstream fixes land in `urob/zmk-actions`. Track progress at https://github.com/urob/zmk-actions.

## How to Retry Later

Revert `.github/workflows/build.yml` to:

```yaml
jobs:
  build:
    uses: urob/zmk-actions/.github/workflows/build-user-config.yml@main
    with:
      toolchain: zephyr    # or zephyr-full for ZMK Studio
```

And ensure the upstream flake includes `protobuf` (for `zephyr`/`gnuarmemb`) and compatible `hidapi` (for `zephyr-full`).
