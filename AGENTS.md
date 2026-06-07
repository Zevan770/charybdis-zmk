# charybdis-zmk

BastardKB Charybdis 分体键盘的 ZMK 固件用户配置仓库。

## 构建目标

- **Board**: `nice_nano` (nice!nano v2, nRF52840)
- **Shields**: `charybdis_left`, `charybdis_right`
- **Snippet**: `studio-rpc-usb-uart` (ZMK Studio)
- **外部驱动**: [badjeff/zmk-pmw3610-driver](https://github.com/badjeff/zmk-pmw3610-driver) (PMW3610 轨迹球)

## 目录结构与职责

```
├── build.yaml         # CI 构建矩阵 (board/shield/snippet)
├── config/
│   ├── west.yml       # Zephyr manifest — 定义 ZMK 和驱动模块版本
│   ├── charybdis.conf           # 用户 Kconfig (共享, 适用左右两侧)
│   ├── charybdis_left.conf      # 用户 Kconfig (左手专用, Kconfig merge 叠加)
│   ├── charybdis.keymap         # 用户键位映射 (共享)
│   ├── charybdis.json           # 键位布局 JSON (ZMK Studio / Keymap Editor)
│   └── boards/shields/charybdis/
│       ├── charybdis.zmk.yml    # Shield 元数据
│       ├── Kconfig.shield       # Shield 符号定义
│       ├── Kconfig.defconfig    # Shield 默认配置 (键盘名/分体角色)
│       ├── charybdis.conf       # Shield Kconfig 默认值 (低优先级)
│       ├── charybdis_left.conf  # Shield 左手 Kconfig 默认值
│       ├── charybdis_right.conf # Shield 右手 Kconfig 默认值
│       ├── charybdis.dtsi       # 共享 Devicetree (矩阵变换/kscan/分体)
│       ├── charybdis-layouts.dtsi  # 物理布局定义
│       ├── charybdis_left.overlay   # 左手 DT overlay
│       └── charybdis_right.overlay  # 右手 DT overlay (SPI/PMW3610)
└── .github/workflows/build.yml  # CI 工作流
```

## Kconfig 合并优先级

ZMK 将多个 Kconfig 片段**叠加合并** (不是替代)。后加载的同名符号覆盖先前的值。

**左手构建 (charybdis_left) 加载顺序:**
1. Board `_defconfig` — nice_nano 硬件默认 (最低优先级)
2. `prj.conf` — ZMK 框架默认
3. `boards/shields/charybdis/charybdis_left.conf` — shield 左手默认
4. `charybdis.conf` — 用户共享配置
5. `charybdis_left.conf` — 用户左手专用配置
6. Snippet `.conf` — ZMK Studio 片段 (最高优先级)

## PMW3610 驱动迁移记录 (Zephyr 4.1)

badjeff 驱动针对 `main`/`zmk-0.4` 分支有**破坏性变更**:

| 旧符号 (Zephyr 3.5) | 新符号/状态 (Zephyr 4.1) |
|---|---|
| `CONFIG_PMW3610_ALT=y` | `CONFIG_PMW3610_ALT=y` (不变) |
| `CONFIG_PMW3610_CPI=2000` | 移至 DT `cpi = <2000>;` |
| `CONFIG_PMW3610_CPI_DIVIDOR=4` | 已移除 |
| `CONFIG_PMW3610_ORIENTATION_90=y` | 已移除, 改用 DT `swap-xy;` + `invert-x;`/`invert-y;` |
| `CONFIG_PMW3610_SNIPE_CPI=800` | 已移除, 改用 layer-based input-listener |
| `CONFIG_PMW3610_SNIPE_CPI_DIVIDOR=4` | 已移除 |
| `CONFIG_PMW3610_SCROLL_TICK=20` | 已移除, 改用 layer-based input-listener |
| `CONFIG_PMW3610_SCROLL_CPI=200` | 已移除 |
| `CONFIG_PMW3610_INVERT_X=y` | `CONFIG_PMW3610_ALT_INVERT_X=y` |
| `CONFIG_PMW3610_INVERT_SCROLL_Y=n` | 已移除 |
| `CONFIG_PMW3610_RUN_DOWNSHIFT_TIME_MS=3264` | `CONFIG_PMW3610_ALT_RUN_DOWNSHIFT_TIME_MS=3264` |
| `CONFIG_PMW3610_REST1_SAMPLE_TIME_MS=20` | `CONFIG_PMW3610_ALT_REST1_SAMPLE_TIME_MS=20` |
| `CONFIG_PMW3610_POLLING_RATE_125_SW=y` | 已移除, 等效用 `CONFIG_PMW3610_ALT_REPORT_INTERVAL_MIN=8` |
| `CONFIG_PMW3610_ALT_SMART_ALGORITHM=y` | `CONFIG_PMW3610_ALT_SMART_ALGORITHM=y` (不变) |
| Compatible string `pixart,pmw3610` | `pixart,pmw3610-alt` |

## 版本对齐状态

| 组件 | 当前 | 目标 |
|---|---|---|
| `west.yml` ZMK revision | `main` | Zephyr 4.1 |
| `.github/workflows/build.yml` 引用 | `@v0.3.0` | 需更新到 `@v0.4.0` (或改为 Docker 方式) |
| `build.yaml` board | `nice_nano` | Zephyr 4.1 需 `nice_nano//zmk` |

## 已知问题

1. `BT_BUF_EVT_RX_COUNT` 需 `>` `BT_BUF_ACL_TX_COUNT` (Zephyr 4.1 BLE 新约束)
2. `PMW3610_*` Kconfig 符号需迁移到 `PMW3610_ALT_*` 前缀
3. `charybdis_right.overlay` 中的 compatible string 需改为 `pixart,pmw3610-alt`
