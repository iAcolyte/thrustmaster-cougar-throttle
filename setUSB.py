Import("env")

board_config = env.BoardConfig()
board_config.update("build.hwids", [
  ["0xFACE", "0x0001"]
])
board_config.update("build.usb_product", "TM Cougar Throttle")
board_config.update("vendor", "HessForge inc")